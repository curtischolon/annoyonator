#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define BUZZER_PIN 4 // GPIO pin where buzzer is connected
#define MAX_NUM_BUZZES 5 // Max number of times buzzer will buzz per activation
#define WAKE_HOUR 22 // Hour to start allowing buzzer activation (24-hour clock)
#define SLEEP_HOUR 6 // Hour to stop allowing buzzer activation (24-hour clock)

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 60 * 60);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  // Set up time client
  timeClient.begin();
  timeClient.setTimeOffset(3600); // Additional offset in seconds
  
  Serial.println("Setup complete!");
}

void loop() {
  timeClient.update();
  int currentHour = timeClient.getHours(); // Get current hour (24-hour clock)
  
  // Check if it's within allowed time range
  if (currentHour >= WAKE_HOUR || currentHour < SLEEP_HOUR) {
    int numBuzzes = random(1, MAX_NUM_BUZZES+1); // Randomize number of buzzes
    
    for (int i = 0; i < numBuzzes; i++) {
      digitalWrite(BUZZER_PIN, HIGH); // Buzzer on
      delay(500);
      digitalWrite(BUZZER_PIN, LOW); // Buzzer off
      delay(500);
    }
    
    int interval = random(60000, 120000); // Randomize buzz interval between 1-2 mins
    delay(interval);
  } else {
    delay(60000); // Wait for 1 min before checking again
  }
}
