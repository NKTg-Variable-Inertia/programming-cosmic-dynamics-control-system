# Wiring Diagram — NKTg Ground Vehicle

## System Overview

```
                     ┌──────────────────────┐
                     │    ESP32 DevKit V1   │
                     │                      │
  MPU6050 ──SDA─────►│ GPIO 21              │
           ──SCL─────►│ GPIO 22              │
                     │                      │
  HX711   ──DT──────►│ GPIO 4               │
           ──SCK─────►│ GPIO 5               │
                     │                      │
  Left encoder ──A──►│ GPIO 34  (input only)│
               ──B──►│ GPIO 35  (input only)│
                     │                      │
  Right encoder ──A─►│ GPIO 36  (input only)│
                ──B─►│ GPIO 39  (input only)│
                     │                      │
  BTS7960 ◄──RPWM───│ GPIO 25              │
          ◄──LPWM───│ GPIO 26              │
          ◄──EN─────│ GPIO 27              │
                     └──────────────────────┘
```

---

## Detailed Pin Tables

### MPU6050 → ESP32

| MPU6050 Pin | ESP32 | Notes |
|-------------|-------|-------|
| VCC | 3.3 V | **Do not use 5 V** — MPU6050 is 3.3 V only |
| GND | GND | Common ground |
| SDA | GPIO 21 | I2C data |
| SCL | GPIO 22 | I2C clock |
| AD0 | GND | Sets I2C address to 0x68 |
| INT | (unused) | Optional — not needed for this firmware |

### HX711 → ESP32

| HX711 Pin | ESP32 | Notes |
|-----------|-------|-------|
| VCC | 3.3 V | |
| GND | GND | |
| DT | GPIO 4 | Serial data out |
| SCK | GPIO 5 | Serial clock |

> To increase HX711 sample rate from 10 Hz to 80 Hz, pull the RATE pin HIGH (connect to 3.3 V).

### Left Encoder → ESP32

| Encoder Pin | ESP32 | Notes |
|-------------|-------|-------|
| A (signal) | GPIO 34 | Interrupt pin — RISING edge |
| B (direction) | GPIO 35 | Read inside ISR |
| VCC | 3.3 V | |
| GND | GND | |

### Right Encoder → ESP32

| Encoder Pin | ESP32 |
|-------------|-------|
| A (signal) | GPIO 36 |
| B (direction) | GPIO 39 |
| VCC | 3.3 V |
| GND | GND |

### BTS7960 → ESP32 + Power

| BTS7960 Pin | Connect to | Notes |
|-------------|-----------|-------|
| RPWM | GPIO 25 | Forward PWM |
| LPWM | GPIO 26 | Reverse PWM |
| R_EN, L_EN | GPIO 27 | Enable — pulled HIGH at startup |
| VCC (logic) | ESP32 5 V | Logic supply |
| B+ (motor) | Battery 12 V (+) | High-current motor power — direct from battery |
| B− (motor) | Common GND | |
| Motor A+ / A− | Left motor | |
| Motor B+ / B− | Right motor | |

---

## Power Architecture

```
Battery 12 V ──────────────────────────► BTS7960 B+
                │
                └──► Step-down 5 V ────► ESP32 VIN
                                    │
                                    └──► ESP32 3.3 V reg ──► MPU6050, HX711, Encoders

Common GND: battery (−), BTS7960 (−), ESP32 GND — all connected together
```

---

## Critical Warnings

1. **GPIO 34, 35, 36, 39 are input-only.** They have no internal pull-up in hardware mode — use `INPUT_PULLUP` in software.
2. **Never power MPU6050 or HX711 from 5 V.** Both are 3.3 V devices; overvoltage will damage them.
3. **Common ground is mandatory.** ESP32 GND, BTS7960 GND, and battery negative must share a single ground point. Floating grounds cause erratic sensor readings.
4. **Motor power is separate from logic power.** Battery 12 V connects directly to BTS7960, not through the ESP32.
