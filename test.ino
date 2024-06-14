#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

// WiFi credentials
const char* ssid = "WISMA BERIMAN 3";
const char* password = "ALVIN0713";

// Server URL
const char* serverName = "http://192.168.54.213:2200";

// Define the DHT sensor type and pin
#define DHTTYPE DHT11
#define DHT_PIN 27

DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  // Start serial communication
  Serial.begin(115200);
  delay(10);

  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  // Wait between measurements
  delay(2000);

  // Read temperature and humidity from DHT sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if any reads failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print sensor readings
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Prepare JSON payload
  String jsonPayload = "{\"temperature\":";
  jsonPayload += temperature;
  jsonPayload += ",\"humidity\":";
  jsonPayload += humidity;
  jsonPayload += "}";

  // Send data to the server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
