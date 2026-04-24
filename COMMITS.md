# Commit History — NKTg Ground Vehicle

Conventional commit messages for the full repository.
Copy and paste each block when setting up the repo from scratch,
or use as a reference for future commits.

---

## Initial setup

```
chore: initialize repository structure

Add top-level directories: build, firmware, hardware, web, system,
validation, media. Add MIT LICENSE.
```

---

## README

```
docs: add README with project overview and quick start

Cover what NKTg is, full hardware BOM with prices, 4-step quick start,
NKTg control logic table, repo layout, and operating layer summary.
```

---

## Firmware

```
feat(firmware): add Config.h with all tunable constants

Centralize sensor calibration, timing, NKTg thresholds, motor PWM
values, and pin assignments. All other firmware files include only
this header — no magic numbers elsewhere.
```

```
feat(firmware): add NKTg_Engine.h — core math and state classifier

Implement computeNKTg() returning NKTgResult {n1, n2}.
Add VehicleState enum and classifyState() using DANGER_N1/N2
from Config.h.
```

```
feat(firmware): add Sensors.h — MPU6050, HX711, encoder management

initSensors() initializes I2C, load cell with tare, and encoder pins.
readMass() guards against HX711 not-ready and enforces MIN_MASS.
computeVelocity() drains encoderCount atomically and returns m/s.
```

```
feat(firmware): add MotorControl.h — BTS7960 LEDC PWM interface

initMotors() sets up two LEDC channels at 5 kHz, 8-bit resolution.
driveMotor() maps VehicleState to PWM: 0 / 90 / 180.
```

```
feat(firmware): add NKTg_ESP32_Controller.ino — main control loop

100 Hz control loop: read sensors → computeNKTg → classifyState → driveMotor.
20 Hz telemetry loop: send "x,v,m,dm_dt\n" over USB Serial.
Encoder ISR in IRAM. Startup handshake prints READY.
```

---

## Hardware documentation

```
docs(hardware): add BOM.md with component list and substitutions

List 10 components with quantities, USD prices, and purchase sources.
Include substitution table for BTS7960 → L298N and battery alternatives.
```

```
docs(hardware): add wiring_diagram.md with ASCII schematic and pin tables

Cover all four modules: MPU6050, HX711, encoders, BTS7960.
Add power architecture diagram and four critical wiring warnings.
```

---

## Build guide

```
docs(build): add build_guide.md — step-by-step assembly A to Z

Nine steps: chassis assembly, load cell mounting, ESP32 placement,
wiring, Config.h update, firmware flash, load cell calibration,
dashboard connection, threshold tuning.
Include expected results table.
```

---

## System documentation

```
docs(system): add control_loop.md — 4-layer architecture and timing

ASCII diagram of all four layers with data types at each boundary.
Timing budget table. Decision tree pseudocode. Rationale for 100 Hz.
```

```
docs(system): add data_flow.md — serial format and system diagram

Document CSV format "x,v,m,dm_dt\n", field definitions with units,
startup handshake, and full end-to-end data flow ASCII diagram.
```

---

## Validation

```
docs(validation): add real_test.md — hardware test results

Four test scenarios: normal run, load added, load removed, emergency brake.
Measured loop frequencies table. Load cell calibration results table.
```

---

## Web dashboard

```
feat(web): add NKTg_System.html — real-time Web Serial dashboard

Dark industrial UI with live NKTg_1 / NKTg_2 canvas charts.
Four metric cards (x, v, m, dm/dt). Vehicle state panel with color coding.
Event log with timestamped entries. Responsive layout for mobile.
Requires Chrome or Edge (Web Serial API).
```

---

## Future commits reference

```
fix(firmware): correct velocity sign for reverse encoder direction
fix(firmware): clamp dm_dt spike on load cell first-read after tare
feat(firmware): add right encoder support for differential drive
feat(web): add threshold slider controls to dashboard
feat(web): add CSV export button for recorded NKTg sessions
docs(validation): add video link for scenario 4 emergency brake test
chore: add .gitignore for Arduino build artifacts and OS files
```
