# Build Guide — NKTg Ground Vehicle (A to Z)

## Prerequisites

Purchase all components listed in [`hardware/BOM.md`](../hardware/BOM.md) before starting.
Shipping from Shopee / Lazada typically takes 2–5 days.

Tools needed: soldering iron, multimeter, M3 screwdriver, ruler or measuring tape.

---

## Step 1 — Assemble the chassis

1. Mount both 12V DC gear motors onto the 2WD or 4WD chassis frame using the included hardware.
2. Press rubber wheels onto motor axles.
3. **Measure the actual wheel circumference** with a tape measure and record the value — you will enter it in `Config.h` as `WHEEL_CIRCUMFERENCE`.
4. Attach both 600 PPR encoders to the motor axles (left and right).

---

## Step 2 — Mount the load cell

1. Fix the 5 kg load cell at the center of the chassis — this position minimizes tilt error.
2. Connect load cell wires to the HX711 module (follow color coding on the HX711 silkscreen: E+, E−, A+, A−).
3. Position the HX711 board close to the ESP32 to keep DT/SCK wires short.

---

## Step 3 — Mount the ESP32 and modules

- Secure the ESP32 DevKit V1 to the chassis with M3 standoffs or hot glue.
- Mount the MPU6050 flat at the geometric center of the chassis so it measures true vehicle acceleration — avoid mounting near motors.

---

## Step 4 — Wire everything

Follow the full pin table in [`hardware/wiring_diagram.md`](../hardware/wiring_diagram.md).

Quick reference:

| Module | Signal | ESP32 pin |
|--------|--------|-----------|
| MPU6050 | SDA | GPIO 21 |
| MPU6050 | SCL | GPIO 22 |
| HX711 | DT | GPIO 4 |
| HX711 | SCK | GPIO 5 |
| Left encoder | A | GPIO 34 |
| Left encoder | B | GPIO 35 |
| Right encoder | A | GPIO 36 |
| Right encoder | B | GPIO 39 |
| BTS7960 | RPWM | GPIO 25 |
| BTS7960 | LPWM | GPIO 26 |
| BTS7960 | EN | GPIO 27 |

**Power rails:**
- MPU6050 VCC → ESP32 3.3 V
- HX711 VCC → ESP32 3.3 V
- BTS7960 B+ → battery 12 V positive
- All GND → common ground (ESP32 GND, BTS7960 GND, battery negative)

> ⚠️ GPIO 34–39 are input-only on ESP32. Do not use them as outputs.

---

## Step 5 — Update Config.h

Open `firmware/NKTg_ESP32_Controller/Config.h` and set:

```cpp
#define WHEEL_CIRCUMFERENCE   0.XXXf   // your measured value in meters
#define LOADCELL_SCALE        2280.0f  // adjust during calibration (Step 7)
```

---

## Step 6 — Flash the firmware

1. Open `firmware/NKTg_ESP32_Controller/NKTg_ESP32_Controller.ino` in Arduino IDE 2.x.
2. Install required libraries via **Tools → Manage Libraries**:
   - `MPU6050` by Electronic Cats
   - `HX711` by Bogdan Necula
3. Select board: **ESP32 Dev Module**
4. Select the correct COM port.
5. Set upload speed to **921600**.
6. Click **Upload**.
7. Open Serial Monitor at **115200 baud** — you should see `READY` printed once.

---

## Step 7 — Calibrate the load cell

```
1. Power on the vehicle with nothing on the cargo area.
2. Open Serial Monitor — confirm m reads near 0.000 kg.
3. Place a known weight on the vehicle (e.g., a 1.000 kg bottle of water).
4. Read the m value from Serial Monitor.
5. If the reading is wrong, adjust LOADCELL_SCALE in Config.h:
      new_scale = current_scale * (actual_mass / displayed_mass)
6. Re-flash and repeat until the reading shows 1.000 kg.
```

---

## Step 8 — Open the web dashboard

1. Open `web/NKTg_System.html` in Chrome or Edge (Web Serial is not supported in Firefox).
2. Click **Connect ESP32**.
3. Select the ESP32 COM port from the browser dialog.
4. The dashboard starts plotting NKTg_1 and NKTg_2 as soon as data arrives.

---

## Step 9 — Test and tune thresholds

Run the vehicle on a flat surface and observe the NKTg plots.
Tune thresholds in `Config.h` to match your vehicle's actual mass and speed:

```cpp
#define DANGER_N1    500.0f   // raise if braking triggers too early
#define DANGER_N2   -50.0f   // lower (more negative) to reduce sensitivity to load change
#define NORMAL_SPEED  180    // adjust for your motor's rated speed
```

Re-flash after each change. No other files need editing.

---

## Expected results after full assembly

| Behavior | Trigger | Action |
|----------|---------|--------|
| Normal run | NKTg_1 and NKTg_2 within bounds | PWM = 180, vehicle moves |
| Sudden load loss | NKTg_2 drops below −50 | PWM reduced to 90 |
| Far travel at high speed | NKTg_1 exceeds 500 | Emergency brake, PWM = 0 |
| Web dashboard | Every 50 ms via USB Serial | Live chart updates |
