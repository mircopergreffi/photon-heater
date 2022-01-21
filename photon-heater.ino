
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <PID_v1.h>

#include "Heater.h"
#include "Fan.h"
#include "NTCSensor.h"
#include "History.h"

AsyncWebServer server(80);

bool configChanged = false;

File file;

Heater heater;
Fan fan;
NTCSensor sensorHeater;
NTCSensor sensorAir;
const char * sensorNames[] = {"Heater","Air","Fan"};
History<30, 3> history(sensorNames);

double heaterPower, setpointTemp = 0, heaterTemp, airTemp;
double fanManualSpeed, fanMode = 0; // 0: auto, 1: manual
PID controller(&heaterTemp, &heaterPower, &setpointTemp, 0.0, 0.0, 0.0, P_ON_E, DIRECT);

// StaticJsonDocument<256> historyDoc;

bool loadFromJson(StaticJsonDocument<2048> &doc)
{
	heater.loadFromJson(doc["heater"]);
	fan.loadFromJson(doc["fan"]);
	sensorHeater.loadFromJson(doc["heater"]["sensor"]);
	sensorAir.loadFromJson(doc["air_sensor"]);
	controller.SetTunings(doc["control"]["p"].as<float>(), doc["control"]["i"].as<float>(), doc["control"]["d"].as<float>());
	controller.SetOutputLimits(0.0, 1.0);
	controller.SetSampleTime(1000);
	controller.SetMode(AUTOMATIC);
	return true;
}

bool connectToWiFi(StaticJsonDocument<2048> &doc)
{
	// Connect to Wi-Fi
	const char * ssid = doc["wifi"]["ssid"];

	if (WiFi.status() == WL_CONNECTED)
		if (WiFi.SSID().compareTo(ssid) == 0)
			return true;

	const char * hostname = doc["wifi"]["hostname"];
	WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
	WiFi.setHostname(hostname);
	
	const char * pass = doc["wifi"]["password"];

	WiFi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}
	
	// Print ESP32 Local IP Address
	Serial.println(WiFi.localIP());

	return true;
}

bool checkForUpdates(StaticJsonDocument<2048> &doc)
{
	return true;
}

bool readConfigFile()
{
	StaticJsonDocument<2048> doc;

	File file = SPIFFS.open("/config.json", "r");
	if (!file)
	{
		Serial.println("Failed to open config file");
		return false;
	}
	
	size_t size = file.size();
	Serial.print("Size cred file: ");
	Serial.println(size);
	if (size > 2048) {
		Serial.println("Config file size is too large");
		return false;
	}
	
	auto error = deserializeJson(doc, file);
	if (error)
	{
		Serial.println("deserializeJson() failed with code ");
		Serial.println(error.c_str());
		return false;
	}

	if (!loadFromJson(doc))
		return false;
	if (!connectToWiFi(doc))
		return false;
	if (!checkForUpdates(doc))
		return false;

	file.close();
	return true;
}

void setup() {

	// Serial port for debugging purposes
	Serial.begin(115200);

	// Initialize SPIFFS
	if(!SPIFFS.begin(true)){
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	// Read Config
	if (!readConfigFile())
	{
		return;
	}
	
	// Route for root / web page
	server.on("/", HTTP_GET, [](AsyncWebServerRequest* request){
		request->send(SPIFFS, "/index.html");
	});

	server.on("/history.json", HTTP_GET, [](AsyncWebServerRequest* request){
		unsigned long fromTimestamp = 0;
		if (request->hasParam("timestamp")){
			fromTimestamp = request->getParam("timestamp")->value().toInt();
		}
		AsyncResponseStream* response = request->beginResponseStream("application/json");
		serializeJson(history.getJson(fromTimestamp), *response);
		request->send(response);

		// AsyncResponseStream* response = request->beginResponseStream("application/json");
		// serializeJson(historyDoc, *response);
		// request->send(response);
	});

	server.onRequestBody([](AsyncWebServerRequest* request, uint8_t *data, size_t len, size_t index, size_t total){
		if (request->url() == "/config.json")
		{
			if (!index)
			{
				file = SPIFFS.open("/config.json", FILE_WRITE);
				if (!file)
				{
					Serial.println("Error opening file config.json");
					request->send(500, "Error opening file config.json");
				}
			}
			for (size_t i = 0; i<len; i++)
			{
				file.write(data[i]);
			}
			if (index + len == total)
			{
				configChanged = true;
				file.close();
			}
		}
	});

	server.on("/set", HTTP_GET, [](AsyncWebServerRequest* request){
		if (request->hasParam("fanSpeed"))
		{
			fanManualSpeed = request->getParam("fanSpeed")->value().toFloat();
			Serial.print("Manual Fan Speed set to: ");
			Serial.println(fanManualSpeed);
			request->send(200, "plain/text", "speed-set");
			return;
		}
		if (request->hasParam("fanMode"))
		{
			String mode = request->getParam("fanMode")->value();
			if (mode.compareTo("auto"))
				fanMode = 0;
			else if (mode.compareTo("manual"))
				fanMode = 1;
			else
			{
				request->send(400, "plain/text", "invalid-fan-mode");
			}
			Serial.print("Fan mode set to: ");
			Serial.println(mode);
			request->send(200, "plain/text", "mode-set");
			return;
		}
		if (request->hasParam("temperature"))
		{
			setpointTemp = request->getParam("temperature")->value().toFloat();
			Serial.print("Temperature set to: ");
			Serial.println(setpointTemp);
			request->send(200, "plain/text", "temperature-set");
		}
		request->send(400, "plain/text", "nothing-set");
	});

	server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request){
		AsyncResponseStream* response = request->beginResponseStream("application/json");
		StaticJsonDocument<256> doc;
		doc["fan"]["speed"] = fanManualSpeed;
		doc["fan"]["mode"] = fanMode == 0 ? "auto" : "manual";
		doc["temperature"] = setpointTemp;
		serializeJson(doc, *response);
		request->send(response);
	});
	
	server.serveStatic("/", SPIFFS, "/");

	// Start server
	server.begin();
}

void loop() {
	if (configChanged)
	{
		readConfigFile();
		configChanged = false;
	}

	float heaterTemp = sensorHeater.readValue();
	float airTemp = sensorAir.readValue();

	if (fanMode == 0)
	{
		if (heaterTemp - airTemp >= 5)
			fan.setSpeed(1.0);
		else
			fan.setSpeed(0.0);
	}
	else
	{
		fan.setSpeed(fanManualSpeed);
	}

	controller.Compute();
	heater.setPower(heaterPower);

	float values[] = {heaterTemp, airTemp, fan.getSpeed()};
	history.push(values);

	
	// historyDoc["timestamps"][0] = millis();
	// historyDoc["Heater"][0] = heaterTemp;
	// historyDoc["Air"][0] = airTemp;
	// historyDoc["Fan"][0] = fan.getSpeed();
	
	delay(1000);
}
