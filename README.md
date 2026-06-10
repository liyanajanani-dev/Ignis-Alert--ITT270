
# Ignis Alert System (IoT Fire Detection)

## Description
Ignis Alert is an IoT-based safety system designed to read the temp, detect fire, smoke, movement, and abnormal sound using sensors.

The system provides early warning through buzzer alerts, improving safety in residential environments.

---

## Features
- Fire and smoke detection
- Motion detection using IR sensor
- Sound detection
- Buzzer alert system
- Fire extinguisher using water pump

---

## Project Structure
- Safe state     : Green Led On
- Warning state  : Yellow Led On, Buzzer On
- Emergency      : Red Led On, Buzzer on
- Critical Alert : Red Led On, Buzzer On, Water pump On

- Safe state     : Temperature below 25C (project was conducted in ac environment)
- Warning state  : Temperature above 25C, Smoke detected
- Emergency      : Temperature above 25C, Smoke detected, Fire detected
- Critical Alert : Temperature above 25C, Smoke detected, Fire detected, Motion and sound detected

final/       -> Final working system
prototype/   -> Early development version
versions/    -> Improved versions
testing/     -> Individual sensor testing
assets/      -> Images and screenshots

## Software Used
- WOKWI
- ARDUINO IDE
- BLYNK

## Hardware Used
- NODEMCU-ESP32
- Breadboard
- Jumper Wire
- DHT11 Temp Sensor
- IR motion sensor
- Flame sensor
- MQ2 Smoke sensor
- Microphone sound detector
- LCD 16X2 12C Display
- Relay Module
- Water Pump
- LED (Red, Yellow, Blue)
- Active Buzzer

---

## Screenshot

assets/Screenshot 2025-12-08 184727.png
assets/Planning_Screenshot 2026-01-29.png
assets/Back_Image 2026-01-29 at 17.46.30.jpeg
assets/Front_Image 2026-01-29 at 17.44.36.jpeg

---

## How to Run
1. Open `.ino` file using Arduino IDE
2. Connect sensors to Arduino
3. Upload code to board
4. Monitor output via Serial Monitor

---

## Notes
This project demonstrates the full development lifecycle:
- Testing individual components
- Building prototypes
- Improving versions
- Delivering a final working system

---

## Author
- Liyana Janani