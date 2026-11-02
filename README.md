# ResQBot – Arduino-Based Disaster Response Robot

<p align="center">
  <strong>An Arduino-based prototype for hazard detection, environmental monitoring, and emergency response support.</strong>
</p>

<p align="center">
  <img src="images/IMG-20261022-WA0151.jpg" width="30%" alt="ResQBot prototype – photograph 1">
  <img src="images/IMG-20261022-WA0152.jpg" width="30%" alt="ResQBot prototype – photograph 2">
  <img src="images/IMG-20261022-WA0153.jpg" width="30%" alt="ResQBot prototype – photograph 3">
</p>

---

## 1. Project Overview

**ResQBot** is an Arduino UNO-based disaster response robot prototype designed to monitor environmental conditions and identify potential hazards.

The system integrates multiple sensors to detect flame, gas, temperature, humidity, and nearby obstacles. It provides alerts through a buzzer and LED indicators while displaying sensor readings and system status on an OLED display.

An emergency switch controls a servo-operated mechanism intended to open and close an emergency kit compartment.

The project demonstrates how embedded systems and sensor-based automation can contribute to disaster-response support applications.

## 2. Project Objectives

- Detect potential fire hazards using a flame sensor.
- Monitor gas levels using an MQ-2 gas sensor.
- Measure temperature and humidity using a DHT11 sensor.
- Detect nearby obstacles using an ultrasonic sensor.
- Provide visual and audible alerts when hazards are detected.
- Display sensor readings and system status on an OLED screen.
- Control an emergency kit mechanism using a servo motor and switch.

## 3. Key Features

| Feature | Description |
|---|---|
| Flame Detection | Detects flame using a digital flame sensor output. |
| Gas Monitoring | Reads gas sensor values from the MQ-2 module. |
| Temperature Monitoring | Measures surrounding temperature. |
| Humidity Monitoring | Measures relative humidity. |
| Obstacle Detection | Measures distance using an HC-SR04 ultrasonic sensor. |
| Audible Alert | Activates the buzzer when a configured danger condition is detected. |
| LED Indication | Red LED indicates danger; green LED indicates safe status. |
| OLED Display | Displays sensor readings and overall system status. |
| Emergency Kit Mechanism | Uses a switch-controlled servo motor to open and close the mechanism. |

## 4. Hardware Components

| Component | Quantity |
|---|---:|
| Arduino UNO | 1 |
| DHT11 Temperature and Humidity Sensor | 1 |
| MQ-2 Gas Sensor Module | 1 |
| Flame Sensor Module | 1 |
| HC-SR04 Ultrasonic Sensor | 1 |
| SG90 Servo Motor | 1 |
| 128 × 64 SSD1306 OLED Display | 1 |
| Buzzer | 1 |
| Red LED | 1 |
| Green LED | 1 |
| Emergency Switch | 1 |
| Robot Chassis and Connecting Wires | As required |

## 5. Pin Configuration

The following table describes the pin assignments used in the Arduino program.

| Component | Arduino Pin |
|---|---|
| DHT11 Data | D2 |
| SG90 Servo Signal | D3 |
| HC-SR04 Trigger | D4 |
| HC-SR04 Echo | D5 |
| Flame Sensor Digital Output | D6 |
| Buzzer | D7 |
| Red LED | D8 |
| Green LED | D9 |
| Emergency Switch | D10 |
| MQ-2 Analog Output | A0 |
| OLED SDA | A4 |
| OLED SCL | A5 |

**Power connections:** Sensors and modules are connected to the appropriate supply voltage and common ground, according to their specifications.

## 6. Working Principle

ResQBot continuously reads data from its connected sensors and evaluates the readings against configured conditions.

### Flame Detection
The flame sensor's digital output is monitored. The program treats a LOW signal as flame detected and activates the danger response.

### Gas Detection
The MQ-2 sensor provides an analog reading. The program compares this reading against a configured threshold of 400.

### Temperature and Humidity Monitoring
The DHT11 sensor measures the surrounding temperature and relative humidity. These readings are displayed on the OLED screen.

### Obstacle Detection
The HC-SR04 sensor measures the distance to nearby objects. An obstacle distance of 100 cm or less is treated as a danger condition.

### Alert System
The buzzer and LED indicators respond to the overall alarm condition:

- **Red LED:** Danger condition detected.
- **Green LED:** No configured danger condition detected.
- **Buzzer:** Activates when flame, gas, or obstacle danger is detected.

### Emergency Kit Mechanism
The emergency switch controls the servo motor:

- Switch ON: Servo moves to 90°.
- Switch OFF: Servo returns to 0°.

The emergency switch operates independently and does not activate the buzzer.

### OLED Display
The OLED display presents two information pages:

1. Sensor readings – temperature, humidity, gas, flame status, and distance.
2. System status – fire, gas, obstacle, emergency kit, and overall alarm status.

## 7. Software and Libraries

**Development environment:** Arduino IDE

The project uses the following Arduino libraries:

- `DHT.h` – DHT sensor communication.
- `Servo.h` – Servo motor control.
- `Wire.h` – I2C communication.
- `U8g2lib.h` – OLED display control.

Install the required libraries through the Arduino IDE Library Manager before compiling the sketch.

## 8. Repository Structure

```text
ResQBot-Disaster-Response-Robot/
│
├── README.md
│
├── ResQBot_810lines_Flame_Buzzer_Fixed.ino
│
└── images/
    ├── IMG-20261022-WA0151.jpg
    ├── IMG-20261022-WA0152.jpg
    └── IMG-20261022-WA0153.jpg
```

## 9. How to Run the Project

1. Download or clone this repository.
2. Open the `.ino` file using Arduino IDE.
3. Install the required libraries.
4. Connect the components according to the pin configuration.
5. Select **Arduino UNO** as the board.
6. Select the correct serial/COM port.
7. Verify and upload the program.
8. Open the Serial Monitor at 9600 baud to observe sensor readings and system status.

## 10. Applications

Potential application areas for further development include:

- Fire hazard monitoring demonstrations.
- Gas leakage detection prototypes.
- Environmental monitoring.
- Obstacle detection and alert systems.
- Educational robotics and embedded-system projects.
- Disaster-response technology research and prototyping.

## 11. Limitations and Future Improvements

This is an educational prototype and is not a certified disaster-response or life-safety device.

Possible future improvements include:

- Wireless communication for remote monitoring.
- GPS-based location tracking.
- Live video monitoring.
- Improved sensor calibration and validation.
- Autonomous navigation.
- Remote control and emergency notification.
- A more robust emergency supply mechanism.
- Improved power management and battery monitoring.

The MQ-2 threshold used in this prototype is an experimental analog value, not a calibrated gas concentration in ppm. Sensor readings and alert behavior should be validated under controlled conditions.

## 12. Project Gallery

The photographs of the ResQBot prototype are available in the [`images`](images/) directory.

## 13. Author

**Project:** ResQBot – Arduino-Based Disaster Response Robot  
**Repository:** [ResQBot-Disaster-Response-Robot](https://github.com/alanabrahambinu/ResQBot-Disaster-Response-Robot)

Developed as an embedded systems and robotics prototype.

---

<p align="center">
  <strong>ResQBot – Technology for Safer and Smarter Emergency Response</strong>
</p>
