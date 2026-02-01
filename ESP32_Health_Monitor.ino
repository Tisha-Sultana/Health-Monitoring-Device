
/****************************************************
  ESP32 Health Monitor
  MAX30102 + ECG + DHT11 + DS18B20
  GitHub Safe Version (credentials removed)
****************************************************/

#include <WiFi.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>

#include "MAX30105.h"
#include "heartRate.h"

#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

#include "secrets.h"

/*************** PINS ****************/
#define DS18B20_PIN 4
#define DHTPIN 5
#define DHTTYPE DHT11
#define ECG_PIN 34
#define LO_PLUS 32
#define LO_MINUS 33

char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

MAX30105 particleSensor;
DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(DS18B20_PIN);
DallasTemperature ds18b20(&oneWire);

unsigned long lastReading = 0;
const unsigned long READ_INTERVAL = 1000;

float beatsPerMinute;
int beatAvg;

const float ADC_REF = 3.3;
const int ADC_MAX = 4095;
const int ECG_SMOOTH_WINDOW = 8;
float ecgBuffer[ECG_SMOOTH_WINDOW];
int ecgBufIdx = 0;
bool ecgBufFull = false;
float ecgBaseline = 0.0;
const float BASELINE_ALPHA = 0.02;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  dht.begin();
  ds18b20.begin();
  Wire.begin(21, 22);

  particleSensor.begin(Wire, I2C_SPEED_FAST);
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x1F);
  particleSensor.setPulseAmplitudeIR(0x1F);

  pinMode(ECG_PIN, INPUT);
  pinMode(LO_PLUS, INPUT);
  pinMode(LO_MINUS, INPUT);
}

void loop() {
  Blynk.run();
  long irValue = particleSensor.getIR();

  if (millis() - lastReading > READ_INTERVAL) {

    Blynk.virtualWrite(V5, irValue);
    Blynk.virtualWrite(V0, beatAvg);

    int spo2 = 97;
    Blynk.virtualWrite(V1, spo2);

    ds18b20.requestTemperatures();
    float tempC = ds18b20.getTempCByIndex(0);
    if (tempC != DEVICE_DISCONNECTED_C) {
      float tempF = tempC * 9 / 5 + 32;
      Blynk.virtualWrite(V2, tempF);
    }

    float roomTemp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(roomTemp)) Blynk.virtualWrite(V3, roomTemp);
    if (!isnan(humidity)) Blynk.virtualWrite(V6, humidity);

    int raw = analogRead(ECG_PIN);
    float ecgVolts = (float)raw / ADC_MAX * ADC_REF;
    Blynk.virtualWrite(V4, ecgVolts);

    lastReading = millis();
  }
}
