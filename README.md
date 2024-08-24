# STIJ5014: Distributed Systems (Assignment 3)

### Lecturer: Suwannit Chareen Chit a/l Sop Chit

### Course Code : **STIJ5014**

| Name                      | Matric Number |
|---------------------------|---------------|
| Eswari a/p Somu            | 832067        |

## Title: Temperature, Humidity and Heat Monitoring System
This project is part of a Distributed Computing course assignment and involves developing an environmental monitoring system using the NodeMCU (ESP8266), DHT22 sensor, 
and other components. The system collects temperature and humidity data, calculates statistics, and sends notifications via Telegram, while also providing a web interface 
for real-time data monitoring.

## Features
Real-Time Data Monitoring: Collects temperature and humidity data using a DHT22 sensor.
Statistical Calculations: Calculates and stores the minimum, maximum, and average values of the collected data.
Persistent Storage: Stores calculated statistics in EEPROM to ensure data persistence.
Alert Notifications: Sends Telegram alerts when temperature thresholds are exceeded.
Web Interface: Displays real-time data and statistical calculations on a user-friendly web page.

##Components Used
Microcontroller: NodeMCU (ESP8266)
Sensor: DHT22 for temperature and humidity measurements
Storage: EEPROM for storing statistical data
Wi-Fi Module: Integrated in the NodeMCU for connecting to the internet
Output:
      Telegram for sending notifications
      Web interface hosted on the NodeMCU for displaying data

##Installation
#Hardware Requirements
NodeMCU (ESP8266)
DHT22 Sensor
Jumper Wires

#Software Requirements
Arduino IDE
ESP8266 Board Package installed in Arduino IDE
Libraries:
DHT.h
EEPROM.h
ESP8266WiFi.h
UniversalTelegramBot.h
