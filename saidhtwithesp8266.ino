#include <ESP8266WiFi.h>
#include "DHT.h"


const char* ssid = "MSB";
const char* password = "onetwothreefour";

#define DHTPIN D4      
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();

  // Connect to WiFi
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("ESP8266 IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available(); 

  if (client) {
    Serial.println("New Client Connected.");
    String request = client.readStringUntil('\r');
    
    client.flush();

    
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    
    String html = "<!DOCTYPE html><html>";
    html += "<head><meta http-equiv='refresh' content='5'/>";
    html += "<title>ESP8266 Temp & Humidity</title></head>";
    html += "<body><h1>ESP8266 Web Server</h1>";
    html += "<p>Temperature: " + String(temperature) + " &deg;C</p>";
    html += "<p>Humidity: " + String(humidity) + " %</p>";
    html += "</body></html>";

    
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();
    client.println(html);
    

    client.stop();
    Serial.println("Client Disconnected.");
  }
}