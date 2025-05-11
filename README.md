# ESP32 Temperature Monitoring System with LCD, LEDs, and BLE

This project implements a temperature monitoring and control system using an ESP32, DS18B20 temperature sensor, I2C LCD, multiple status LEDs, and BLE broadcasting. It simulates a heater system that turns on when temperature is below target and responds dynamically to changing temperature states.


# Features

-  Monitors temperature using DS18B20 sensor
-  LED indicators for:
  - Low temperature
  - Target reached
  - Overheating
-  Simulated heater with LED blinking
-  Displays system status and temperature on 16x2 I2C LCD
- Sends temperature over BLE for mobile viewing
-  Detects temperature trend: Increasing, Decreasing, Stabilizing

# Hardware Requirements

- ESP32 development board
- DS18B20 digital temperature sensor
- 4x LEDs (Blue, Yellow, Red, Orange)
- 1x 220Ω resistor per LED
- 16x2 I2C LCD display
- Breadboard and jumper wires
- Optional: BLE scanner app (e.g., nRF Connect)

#Pin Configuration

| Component         | ESP32 GPIO |
|------------------|------------|
| DS18B20 Data      | GPIO 2     |
| LOW_TEMP_LED (Blue)    | GPIO 4     |
| TARGET_TEMP_LED (Yellow) | GPIO 16    |
| OVERHEAT_LED (Red)      | GPIO 17    |
| HEATER_LED (Orange)     | GPIO 5     |
| I2C LCD SDA        | GPIO 21    |
| I2C LCD SCL        | GPIO 22    |

> Note: Connect DS18B20 with a 4.7kΩ pull-up resistor between VCC and data line.

# Temperature Thresholds

| State            | Temperature Range          | LCD Message        | Heater LED |
|------------------|----------------------------|--------------------|------------|
| Low              | `< 50°C`                   | `Heating`          | Blinking   |
| Target Reached   | `≈ 50°C ±1°C`              | `Target Reached`   | OFF        |
| Overheat         | `> 70°C`                   | `Overheat`         | OFF        |
| Idle             | `Exactly at target`        | `Idle`             | OFF        |
| In-Between       | `50°C < Temp < 70°C`       | Red LED Blinking   | Varies     |

LCD also shows temperature trends:
-  `Increasing`
-  `Decreasing`
- `Stabilizing`

# BLE Broadcast

- Device advertises as `ESP32_TempMonitor`
- Sends temperature value every 1 second via BLE characteristics
- Use any BLE scanner app to view real-time temperature

# Getting Started

1. **Clone the Repository**
   ```bash
   git clone https://github.com/your-username/esp32-temp-monitor.git
   cd esp32-temp-monitor
2. Install Required Arduino Libraries

  OneWire

  DallasTemperature

  LiquidCrystal_I2C

  BLE library (comes built-in with ESP32 core)

3.Upload to ESP32

  Open sketch.ino in Arduino IDE

  Choose ESP32 board and correct COM port

  Upload the code
