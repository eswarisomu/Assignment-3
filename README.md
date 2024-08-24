# STIJ5014: Distributed Systems (Assignment 3)

### Lecturer: Suwannit Chareen Chit a/l Sop Chit

### Course Code : **STIJ5014**

| Name                      | Matric Number |
|---------------------------|---------------|
| Eswari a/p Somu            | 832067        |

## Title: Temperature, Humidity and Heat Monitoring System
This project is part of a Distributed Computing course assignment and involves developing an temperature, humidity and heat monitoring system using the NodeMCU (ESP8266), DHT11 sensor, 
and other components. The system collects temperature and humidity data, calculates statistics, and sends notifications via Telegram, while also providing a web interface 
for real-time data monitoring.

## Features
Real-Time Data Monitoring: Collects temperature and humidity data using a DHT11 sensor. <br/>
Statistical Calculations: Calculates and stores the minimum, maximum, and average values of the collected data. <br/>
Persistent Storage: Stores calculated statistics in EEPROM to ensure data persistence.  <br/>
Alert Notifications: Sends Telegram alerts when temperature thresholds are exceeded. <br/>
Web Interface: Displays real-time data and statistical calculations on a user-friendly web page. <br/>

## Components Used
Microcontroller: NodeMCU (ESP8266) <br/>
Sensor: DHT22 for temperature and humidity measurements <br/>
Storage: EEPROM for storing statistical data <br/>
Wi-Fi Module: Integrated in the NodeMCU for connecting to the internet <br/>
Output: <br/>
      Telegram for sending notifications <br/>
      Web interface hosted on the NodeMCU for displaying data <br/>

## Installation
## Hardware Requirements
NodeMCU (ESP8266) <br/>
DHT22 Sensor <br/>
Jumper Wires <br/>

## Software Requirements
Arduino IDE <br/>
ESP8266 Board Package installed in Arduino IDE <br/>
Libraries: <br/>
DHT.h <br/> 
EEPROM.h <br/>
ESP8266WiFi.h <br/>
UniversalTelegramBot.h <br/>

## Presentation

You can watch our presentation on YouTube via the following link:(https://www.youtube.com/watch?v=8fTk1dIVOdM)

## Arduino Code

The Arduino code used for the project has been uploaded and can be found [here](https://github.com/eswarisomu/ASIGNMENT-2---STIJ5014-DISTRIBUTED-SYSTEMS/blob/main/Assignment2.ino).
