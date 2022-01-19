
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "Heater.h"
#include "Fan.h"
#include "NTCSensor.h"
#include "TemperatureHistory.h"

AsyncWebServer server(80);

Heater heater;
Fan fan;
NTCSensor sensorHeater;
NTCSensor sensorAir;
String sensorNames[] = {"Heater","Air"};
TemperatureHistory<60,2> temperatureHistory(sensorNames);


bool loadFromJson(StaticJsonDocument<2048> &doc)
{
	heater.loadFromJson(doc["heater"]);
	fan.loadFromJson(doc["fan"]);
	sensorHeater.loadFromJson(doc["heater"]["sensor"]);
	sensorAir.loadFromJson(doc["air_sensor"]);
	return true;
}

bool connectToWiFi(StaticJsonDocument<2048> &doc)
{
	// Connect to Wi-Fi
	const char * ssid = doc["wifi"]["ssid"];

	if (WiFi.status() == WL_CONNECTED)
		if (WiFi.SSID().compareTo(ssid) == 0)
			return true;

	Serial.println(pass);

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
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/index.html");
	});

	server.on("/history.json", HTTP_GET, [](AsyncWebServerRequest *request){
		unsigned long fromTimestamp = 0;
		AsyncResponseStream * response = request->beginResponseStream("application/json");
		serializeJson(temperatureHistory.getJson(fromTimestamp), *response);
		request->send(response);
	});
	
	server.serveStatic("/", SPIFFS, "/");

	// Start server
	server.begin();
}

void loop() {
	temperatureHistory.push({1.0, 2.0});
	delay(1000);
}
