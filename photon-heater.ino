
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "Hardware.h"

AsyncWebServer server(80);
Hardware mHardware;
bool configChanged = false;

File file;

bool connectToWiFi(JsonDocument &doc)
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
	for (int i=0; i<20; i++) {
		if (WiFi.status() == WL_CONNECTED)
		{
			// Print ESP32 Local IP Address
			Serial.println(WiFi.localIP());
			return true;
		}
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}

	const char * ap_ssid = doc["wifi"]["ap_ssid"];
	const char * ap_pass = doc["wifi"]["ap_password"];

	WiFi.softAP(ap_ssid, ap_pass);
	Serial.println(WiFi.softAPIP());
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
	
	mHardware.loadFromJson(doc);
	
	if (!connectToWiFi(doc))
		return false;

	file.close();
	return true;
}

void setup() {

	// Serial port for debugging purposes
	Serial.begin(115200);

	// Initialize SPIFFS
	if(!SPIFFS.begin(true))
	{
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	// Read Config
	if (!readConfigFile())
		return;
	
	// Route for root / web page
	server.on("/", HTTP_GET, [](AsyncWebServerRequest* request){
		request->send(SPIFFS, "/index.html");
	});

	server.on("/history.json", HTTP_GET, [](AsyncWebServerRequest* request){
		unsigned long fromTimestamp = 0;
		if (request->hasParam("timestamp"))
			fromTimestamp = request->getParam("timestamp")->value().toInt();
		AsyncResponseStream* response = request->beginResponseStream("application/json");
		DynamicJsonDocument doc(4096);
		mHardware.populateHistoryJson(doc, fromTimestamp);
		// Serial.print("JsonDocument (history.json) using: ");
		// Serial.print(doc.memoryUsage());
		// Serial.println(" bytes");
		serializeJson(doc, *response);
		request->send(response);
		doc.clear();
	});

	server.on("/history.tsv", HTTP_GET, [](AsyncWebServerRequest* request){
		unsigned long fromTimestamp = 0;
		if (request->hasParam("timestamp"))
			fromTimestamp = request->getParam("timestamp")->value().toInt();
		AsyncResponseStream* response = request->beginResponseStream("text/tab-separated-values");
		mHardware.printHistoryTo(*response, fromTimestamp, '\t');
		request->send(response);
	});

	server.onRequestBody([](AsyncWebServerRequest* request, uint8_t *data, size_t len, size_t index, size_t total){
		if (request->url() == "/config.json")
		{
			if (configChanged)
			{
				request->send(500);
			}
			else
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
		}
	});

	server.on("/set", HTTP_GET, [](AsyncWebServerRequest* request){
		if (request->hasParam("heater"))
		{
			String heater = request->getParam("heater")->value();
			if (heater == "on")
			{
				mHardware.setHeaterOn(true);
				request->send(200, "plain/text", "heater-on");
			}
			else if (heater == "off")
			{
				mHardware.setHeaterOn(false);
				request->send(200, "plain/text", "heater-off");
			}
			else
			{
				request->send(400, "plain/text", "invalid-heater-mode");
			}
		}
		else if (request->hasParam("fanSpeed"))
		{
			mHardware.setFanManualSpeed(request->getParam("fanSpeed")->value().toFloat());
			Serial.print("Manual Fan Speed set to: ");
			Serial.println(mHardware.getStatus().fanManualSpeed);
			request->send(200, "plain/text", "speed-set");
		}
		else if (request->hasParam("fanMode"))
		{
			String mode = request->getParam("fanMode")->value();
			if (mode.compareTo("auto") == 0)
			{
				mHardware.setFanMode(FAN_AUTO);
				request->send(200, "plain/text", "fan-auto");
			}
			else if (mode.compareTo("manual") == 0)
			{
				mHardware.setFanMode(FAN_MANUAL);
				request->send(200, "plain/text", "fan-manual");
			}
			else
			{
				request->send(400, "plain/text", "invalid-fan-mode");
			}
		}
		else if (request->hasParam("temperature"))
		{
			mHardware.setTemperature(request->getParam("temperature")->value().toFloat());
			Serial.print("Temperature set to: ");
			Serial.println(mHardware.getStatus().airSetpoint);
			request->send(200, "plain/text", "temperature-set");
		}
		request->send(400, "plain/text", "nothing-set");
	});

	server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request){
		AsyncResponseStream* response = request->beginResponseStream("application/json");
		StaticJsonDocument<256> doc;
		doc["fan"]["speed"] = mHardware.getStatus().fanManualSpeed;
		doc["fan"]["mode"] = mHardware.getStatus().fanMode == FAN_AUTO ? "auto" : "manual";
		doc["temperature"] = mHardware.getStatus().airSetpoint;
		doc["heater"] = mHardware.getStatus().heater ? "on" : "off";
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

	mHardware.run();
}
