#define BLYNK_TEMPLATE_ID "TMPL6P3JBWan_"
#define BLYNK_TEMPLATE_NAME "Jarak bak sampah"
#define BLYNK_AUTH_TOKEN "w2EjqwP_M2JTETPAi4BRBmdhFSgyfEER"

#include <WiFi.h>
#include <WiFiMulti.h>
#include <BlynkSimpleEsp32.h>
#include "CTBot.h"

WiFiMulti wifiMulti;

const char* wifiSSID[] = {"Kelompok 3", "Ganjar Pesanggrahan", "kelompok5iot"};
const char* wifiPassword[] = {"12345678", "ganjar1216", "tidakterhingga"};
const int numWiFi = sizeof(wifiSSID) / sizeof(wifiSSID[0]);
String token = "6572144207:AAHALukRIEakZ_9Rrvabz6R9hc_R-3qA-Rk";
const int id = 1266437903;

CTBot myBot;

#define TRIGGER_PIN 12
#define ECHO_PIN 13

long duration;
int distance;

void setup() {
  Serial.begin(115200);
  delay(10);

  // Setup WiFi connections
  for (int i = 0; i < numWiFi; i++) {
    wifiMulti.addAP(wifiSSID[i], wifiPassword[i]);
  }

  Serial.println("Scanning for available networks...");
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }

  // Connect to Wi-Fi using wifiMulti (connects to the SSID with the strongest connection)
  Serial.println("Connecting Wifi...");
  if (wifiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myBot.setTelegramToken(token);

  Blynk.begin(BLYNK_AUTH_TOKEN, wifiSSID[0], wifiPassword[0]);
}

void loop() {
  Blynk.run();

  digitalWrite(TRIGGER_PIN, LOW);
  delay(100);
  digitalWrite(TRIGGER_PIN, HIGH);
  delay(100);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.0343) / 2;

  Blynk.virtualWrite(V0, distance);

  if (distance < 15) {
    myBot.sendMessage(id, "TONG SAMPAH FULL");
    delay(3000);
    Serial.println("FULL");
  } else if (distance > 15) {
    Serial.println("KOSONG");
  }

  delay(1000);  // Add delay for stability
}
