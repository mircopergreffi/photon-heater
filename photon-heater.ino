
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

#include "Heater.h"

DynamicJsonDocument doc(2048);
AsyncWebServer server(80);

Heater heater;
NTCSensor sensorHeater, sensorAir;

bool readConfigFile()
{
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

	heater(doc);
	sensorHeater(doc["heater_sensor"]);
	sensorAir(doc[""]);
	
	// Connect to Wi-Fi
	const char * ssid = doc["wifi_ssid"];
	const char * pass = doc["wifi_password"];
	WiFi.begin(ssid, pass);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}
	
	// Print ESP32 Local IP Address
	Serial.println(WiFi.localIP());
	
	server.serveStatic("/", SPIFFS, "/");

	// Route for root / web page
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/index.html");
	});
	
	// Start server
	server.begin();
}

void loop() {
	
}
