
# ESP32 Health Monitor (IoT Project)

An IoT-based real-time health monitoring system using ESP32 and multiple biomedical sensors.

## Sensors Used
- MAX30102 (Heart Rate + SpO2 + IR)
- ECG Sensor
- DS18B20 (Body Temperature)
- DHT11 (Room Temperature + Humidity)

## Features
- Real-time monitoring via Blynk IoT
- Heart Rate, SpO2, ECG waveform
- Body and room temperature
- Humidity monitoring

## Virtual Pins
| Parameter | Pin |
|------------|-----|
| Heart Rate | V0 |
| SpO2 | V1 |
| Body Temp | V2 |
| Room Temp | V3 |
| ECG | V4 |
| IR | V5 |
| Humidity | V6 |

## Security Note
WiFi credentials and Blynk token are stored in `secrets.h` and excluded using `.gitignore`.
