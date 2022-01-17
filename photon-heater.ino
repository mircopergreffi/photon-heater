
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>

JsonObject & config;
AsyncWebServer server(80);

bool readConfigFile()
{
	File f = SPIFFS.open("/config.json", "r");
	if (!f)
	{
		Serial.println("Failed to open config file");
		return false;
	}
	
	size_t size = configFile.size();
	if (size > 4096) {
		Serial.println("Config file size is too large");
		return false;
	}
	
	StaticJsonBuffer<256> jsonBuffer;

	config = jsonBuffer.parseObject(f);
	if (!config.success())
	{
		Serial.println(F("Failed to read file, using default configuration"));
		return false;
	}

	f.close();
	return true;
}

void setup() {
	
	// Serial port for debugging purposes
	Serial.begin(115200);
	
	// Serial port for debugging purposes
	Serial.begin(115200);

	// Initialize SPIFFS
	if(!SPIFFS.begin(true)){
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	// Read Config
	readConfigFile();
	
	// Connect to Wi-Fi
	WiFi.begin(config["wifi_ssid"], config["wifi_password"]);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Connecting to WiFi..");
	}

	// Print ESP32 Local IP Address
	Serial.println(WiFi.localIP());
	
	// Route for root / web page
	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/web-client/index.html", "text/html");
	});
	
	// Route to load style.css file
	server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/web-client/style.css", "text/css");
	});

	// Route to load script.js file
	server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/web-client/script.js", "text/javascript");
	});
	
	// Route to load config.json file
	server.on("/config.json", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/config.json", "text/css");
	});

	// Route to save config.json file
	server.on("/config.json", HTTP_POST, [](AsyncWebServerRequest *request){
		
	});

	// Start server
	server.begin();
}

void loop() {
	
}
