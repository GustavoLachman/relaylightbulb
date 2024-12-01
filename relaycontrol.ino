#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Define your Wi-Fi credentials
const char* ssid = "######";        // Replace with your SSID
const char* password = "######";   // Replace with your Wi-Fi password

// Define your Telegram bot token
#define BOT_TOKEN "######"  // Replace with your bot token

// Define the chat ID to interact with the bot (you can use any chat with your bot)
#define CHAT_ID "######"  // Replace with your Telegram chat ID

// LED pins
const int ledPin1 = D2;  // LED 1 on pin D2
const int ledPin2 = D3;  // LED 2 on pin D3
bool ledState1 = false;  // Variable for LED 1 state
bool ledState2 = false;  // Variable for LED 2 state

// NTP time configuration (Cuiabá time, UTC-4)
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", -4 * 3600, 60000);  // UTC-4 for Cuiabá

WiFiClientSecure client;  // Secure communication client
UniversalTelegramBot bot(BOT_TOKEN, client);  // Creates the bot object

void setup() {
  Serial.begin(115200);

  pinMode(ledPin1, OUTPUT);  // Set LED 1 pin as output
  pinMode(ledPin2, OUTPUT);  // Set LED 2 pin as output

  // Connect to Wi-Fi
  connectWiFi();

  // Start the NTP client to synchronize time
  timeClient.begin();

  // Delay of 2 seconds before starting to respond to commands
  delay(2000);
}

void loop() {
  // Check Wi-Fi connection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Lost Wi-Fi connection! Trying to reconnect...");
    connectWiFi();  // Attempt to reconnect to Wi-Fi
  }

  timeClient.update();  // Update time from NTP server

  int currentHour = timeClient.getHours();  // Get the current hour (24-hour format)
  Serial.print("Current hour: ");
  Serial.println(currentHour);

  // Check if the time is between 6 PM and midnight (Cuiabá time)
  if (currentHour >= 18 && currentHour < 24) {
    if (!ledState1) {
      digitalWrite(ledPin1, HIGH);  // Turn on LED 1
      ledState1 = true;
      Serial.println("LED 1 ON (automatic)");
    }
    if (!ledState2) {
      digitalWrite(ledPin2, HIGH);  // Turn on LED 2
      ledState2 = true;
      Serial.println("LED 2 ON (automatic)");
    }
  } else {
    if (ledState1) {
      digitalWrite(ledPin1, LOW);  // Turn off LED 1
      ledState1 = false;
      Serial.println("LED 1 OFF (automatic)");
    }
    if (ledState2) {
      digitalWrite(ledPin2, LOW);  // Turn off LED 2
      ledState2 = false;
      Serial.println("LED 2 OFF (automatic)");
    }
  }
}

// Function to connect to Wi-Fi
void connectWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connected to Wi-Fi!");
}
