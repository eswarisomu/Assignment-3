#include <ESP8266WiFi.h>              // Library to manage Wi-Fi connection on ESP8266
#include <WiFiClientSecure.h>         // Library to handle secure Wi-Fi connections (HTTPS)
#include <ESP8266WebServer.h>         // Web server to host the interface on the ESP8266
#include <UniversalTelegramBot.h>     // Library to interact with the Telegram Bot API
#include <EEPROM.h>                   // EEPROM library for storing non-volatile data
#include "DHT.h"                      // DHT sensor library for temperature and humidity
#include "MainPage.h"                 // Web page content (HTML, CSS, JS)

// Defines the type of DHT sensor being used (DHT22 or DHT11)
#define DHTTYPE DHT22  
const int DHTPin = 5;  // DHT sensor pin connected to GPIO5 (D1)

// Wi-Fi credentials
const char* ssid = "Your_SSID"; // WiFi SSID
const char* password = "Your_PASSWORD"; // WiFi password

// Telegram Bot Token and Chat ID
String botToken = "Your_Bot_Token";  // Replace with your bot token
String chatID = "Your_Chat_ID";  // Replace with your chat ID

// Declare the web server running on port 80 (default HTTP port)
ESP8266WebServer server(80);

// Initialize DHT sensor
DHT dht(DHTPin, DHTTYPE);

// Secure Wi-Fi client for Telegram Bot communication
WiFiClientSecure client;

// Initialize Telegram Bot object
UniversalTelegramBot bot(botToken, client);

// Variables to hold min, max, and sum values for temperature, humidity, and heat index
float tempMin, tempMax, tempSum;
float humidityMin, humidityMax, humiditySum;
float heatIndexMin, heatIndexMax, heatIndexSum;

// Variable to count the number of samples taken
int sampleCount = 0;

// Variables for timing calculations
unsigned long previousMillis = 0;
const long interval = 60000;  // Interval for measurements (1 minutes)

// Function to reset calculation variables to their initial state
void resetCalculations() {
  tempMin = 100;          // Initialize tempMin to a high value
  tempMax = -100;         // Initialize tempMax to a low value
  tempSum = 0;            // Reset temperature sum

  humidityMin = 100;      // Initialize humidityMin to a high value
  humidityMax = 0;        // Initialize humidityMax to a low value
  humiditySum = 0;        // Reset humidity sum

  heatIndexMin = 100;     // Initialize heatIndexMin to a high value
  heatIndexMax = -100;    // Initialize heatIndexMax to a low value
  heatIndexSum = 0;       // Reset heat index sum

  sampleCount = 0;        // Reset sample count
}

// Function to save calculated values (min, max, average) to EEPROM
void saveToEEPROM() {
  EEPROM.begin(512);  // Initialize EEPROM with a size of 512 bytes
  // Store temperature min, max, and average values
  EEPROM.put(0, tempMin);
  EEPROM.put(4, tempMax);
  EEPROM.put(8, tempSum / sampleCount);  // Average temperature

  // Store humidity min, max, and average values
  EEPROM.put(12, humidityMin);
  EEPROM.put(16, humidityMax);
  EEPROM.put(20, humiditySum / sampleCount);  // Average humidity

  // Store heat index min, max, and average values
  EEPROM.put(24, heatIndexMin);
  EEPROM.put(28, heatIndexMax);
  EEPROM.put(32, heatIndexSum / sampleCount);  // Average heat index

  EEPROM.commit();  // Commit changes to EEPROM
}

// Function to initialize EEPROM with default values if no valid data is present
void initializeEEPROM() {
  EEPROM.begin(512);

  float testValue;
  EEPROM.get(0, testValue);  // Read a test value from EEPROM to check validity
  if (isnan(testValue) || testValue < -100 || testValue > 100) {
    resetCalculations();  // If data is invalid, reset calculations
    saveToEEPROM();       // Save default values to EEPROM
  }
}

// Function to calculate min, max, and sum for temperature, humidity, and heat index
void calculateMinMaxAvg(float temperature, float humidity, float heatIndex) {
  // Calculate min and max temperature
  if (temperature < tempMin) tempMin = temperature;
  if (temperature > tempMax) tempMax = temperature;
  tempSum += temperature;  // Add current temperature to the sum

  // Calculate min and max humidity
  if (humidity < humidityMin) humidityMin = humidity;
  if (humidity > humidityMax) humidityMax = humidity;
  humiditySum += humidity;  // Add current humidity to the sum

  // Calculate min and max heat index
  if (heatIndex < heatIndexMin) heatIndexMin = heatIndex;
  if (heatIndex > heatIndexMax) heatIndexMax = heatIndex;
  heatIndexSum += heatIndex;  // Add current heat index to the sum

  sampleCount++;  // Increment sample count
}

// Function to send a message to Telegram Bot
void sendTelegramMessage(String message) {
  // Connect to Telegram server over HTTPS
  if (client.connect("api.telegram.org", 443)) {
    String url = "/bot" + botToken + "/sendMessage?chat_id=" + chatID + "&text=" + message;
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: api.telegram.org\r\n" +
                 "Connection: close\r\n\r\n");

    // Wait for the server response and handle it
    while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") break;
    }
    client.stop();  // Close connection
    Serial.println("Message sent to Telegram successfully");
  } else {
    Serial.println("Failed to connect to Telegram server");
  }
}

// HTTP route handler for the root ("/") URL
void handleRoot() {
  String s = MAIN_page;  // Load main page HTML content
  server.send(200, "text/html", s);  // Send response to client with status 200 (OK)
}

// HTTP route handler for reading temperature from the DHT sensor
void handleDHT11Temperature() {
  float temperature = dht.readTemperature();  // Read temperature
  String temperatureValue = String(temperature);  // Convert temperature to string
  server.send(200, "text/plain", temperatureValue);  // Send temperature as a response

  if (!isnan(temperature)) {  // Check if the temperature reading is valid
    Serial.print("Temperature: ");
    Serial.println(temperature);

    // If temperature exceeds threshold, send a Telegram notification
    if (temperature >= 30.0) {
      String message = "Warning! Temperature has reached " + String(temperature) + "°C";
      sendTelegramMessage(message);
    }
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }
}

// HTTP route handler for reading humidity from the DHT sensor
void handleDHT11Humidity() {
  float humidity = dht.readHumidity();  // Read humidity
  String humidityValue = String(humidity);  // Convert humidity to string
  server.send(200, "text/plain", humidityValue);  // Send humidity as a response

  if (!isnan(humidity)) {  // Check if the humidity reading is valid
    Serial.print("Humidity: ");
    Serial.println(humidity);
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }
}

// HTTP route handler for calculating and reading heat index
void handleDHT11HeatIndex() {
  float temperature = dht.readTemperature();  // Read temperature
  float humidity = dht.readHumidity();  // Read humidity
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);  // Calculate heat index

  String heatIndexValue = String(heatIndex);  // Convert heat index to string
  server.send(200, "text/plain", heatIndexValue);  // Send heat index as a response

  if (!isnan(heatIndex)) {  // Check if the heat index calculation is valid
    Serial.print("Heat Index: ");
    Serial.println(heatIndex);
  } else {
    Serial.println("Failed to calculate Heat Index!");
  }
}

// HTTP route handler to display the calculation results (min, max, avg)
void handleCalculationResults() {
  EEPROM.begin(512);  // Initialize EEPROM to read saved values
  float tempAvg, humidityAvg, heatIndexAvg;

  // Retrieve saved values from EEPROM
  EEPROM.get(0, tempMin);
  EEPROM.get(4, tempMax);
  EEPROM.get(8, tempAvg);

  EEPROM.get(12, humidityMin);
  EEPROM.get(16, humidityMax);
  EEPROM.get(20, humidityAvg);

  EEPROM.get(24, heatIndexMin);
  EEPROM.get(28, heatIndexMax);
  EEPROM.get(32, heatIndexAvg);

  // Format the results for display on the web page
  String results = "Temperature - Min: " + String(tempMin, 2) + "°C, Max: " + String(tempMax, 2) + "°C, Avg: " + String(tempAvg, 2) + "°C <br>" +
                   "Humidity - Min: " + String(humidityMin, 2) + "%, Max: " + String(humidityMax, 2) + "%, Avg: " + String(humidityAvg, 2) + "% <br>" +
                   "Heat Index - Min: " + String(heatIndexMin, 2) + "°C, Max: " + String(heatIndexMax, 2) + "°C, Avg: " + String(heatIndexAvg, 2) + "°C <br>";
  server.send(200, "text/plain", results);  // Send the formatted results to the web page
}

// Setup function runs once at the beginning
void setup(void) {
  Serial.begin(115200);  // Start serial communication for debugging
  delay(500);  // Short delay

  dht.begin();  // Initialize the DHT sensor
  delay(500);

  EEPROM.begin(512);  // Initialize EEPROM
  initializeEEPROM();  // Check if EEPROM has valid data, otherwise initialize

  WiFi.begin(ssid, password);  // Connect to Wi-Fi network
  Serial.print("Connecting to WiFi");

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Print the connection details
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Define routes for the web server
  server.on("/", handleRoot);  // Main page
  server.on("/readTemperature", handleDHT11Temperature);  // Temperature
  server.on("/readHumidity", handleDHT11Humidity);  // Humidity
  server.on("/readHeatIndex", handleDHT11HeatIndex);  // Heat index
  server.on("/readCalculationResults", handleCalculationResults);  // Calculation results
  server.begin();  // Start the web server
  Serial.println("HTTP server started");

  client.setInsecure();  // Disable certificate validation for secure client (not recommended for production)
}

// Main loop function
void loop(void) {
  server.handleClient();  // Handle incoming web server requests

  unsigned long currentMillis = millis();  // Get the current time in milliseconds
  if (currentMillis - previousMillis >= interval) {  // Check if it's time to take a measurement
    previousMillis = currentMillis;  // Update the previous time

    // Read temperature, humidity, and heat index
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

    // If readings are valid, calculate min, max, avg and save to EEPROM
    if (!isnan(temperature) && !isnan(humidity) && !isnan(heatIndex)) {
      calculateMinMaxAvg(temperature, humidity, heatIndex);  // Calculate values
      saveToEEPROM();  // Save the calculated values to EEPROM
      resetCalculations();  // Reset values for the next interval
    }
  }
}
