# AI-Based Adaptive Communication for Smart Agriculture Using IoT and WSN

## Project Overview
This project demonstrates adaptive communication for smart agriculture using IoT and Wireless Sensor Network concepts.

The system monitors soil moisture, temperature and humidity. Instead of sending every reading at fixed intervals, it uses threshold-based decision logic to identify significant changes and decide when to transmit data.

## Components Used
- ESP32
- DHT22 temperature and humidity sensor
- Potentiometer (virtual soil moisture input)
- 16x2 I2C LCD
- Wokwi Simulator
- ThingSpeak Cloud Platform

## Working
1. Sensors provide environmental readings.
2. ESP32 collects the readings.
3. Adaptive logic checks for significant changes.
4. Data is sent through Wi-Fi when a reporting condition is met.
5. ThingSpeak displays the uploaded values through graphs.

## Software and Libraries
- Arduino C++
- DHT Sensor Library
- Adafruit Unified Sensor
- LiquidCrystal I2C
- ESP32 Wi-Fi and HTTPClient libraries

## Simulation
The circuit is developed and tested using Wokwi.

## Note
This is a simulation-based prototype using threshold-based adaptive logic. Physical sensor deployment and trained AI/ML implementation are future enhancements.
