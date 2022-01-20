
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

Heater heater;
Fan fan;
NTCSensor sensorHeater;
NTCSensor sensorAir;
const char * sensorNames[] = {"Heater","Air","Fan"};
History<60, 3> history(sensorNames);

double heaterPower, setpointTemp = 0, heaterTemp, airTemp;
double fanManualSpeed, fanMode = 0; // 0: auto, 1: manual
PID controller(heaterTemp, heaterPower, setpointTemp, 0.0, 0.0, 0.0, DIRECT);

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
	});

	server.on("/set", HTTP_GET, [](AsyncWebServerRequest* request){
		if (request->hasParam("fanSpeed"))
			fanManualSpeed = request->getParam("fanSpeed")->value().toFloat();
		if (request->hasParam("fanMode"))
		{
			String mode = request->getParam("fanMode")->value();
			if (mode.compareTo("auto"))
				fanMode = 0;
			else if (mode.compareTo("manual"))
				fanMode = 1;
			else
				request->send(400, "invalid fan mode");
		}
		if (request->hasParam("temperature"))
			setpointTemp = request->getParam("temperature")->value().toFloat();
		request->send(200);
	});
	
	server.serveStatic("/", SPIFFS, "/");

	// Start server
	server.begin();
}

void loop() {
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
	delay(1000);
}
