# Real Hardware Test Results

## Test Environment

- **Hardware**: ESP32 DevKit V1 + MPU6050 + 600 PPR encoders + 5 kg load cell
- **Chassis**: 2WD aluminum frame, static mass ~0.82 kg
- **Surface**: Flat indoor hardwood floor
- **Firmware**: v1.0, default thresholds (DANGER_N1 = 500, DANGER_N2 = −50)

---

## Scenario 1 — Normal run (no cargo)

| Time (s) | x (m) | v (m/s) | m (kg) | NKTg_1 | NKTg_2 | State |
|----------|-------|---------|--------|--------|--------|-------|
| 0 | 0.000 | 0.000 | 0.820 | 0.00 | 0.00 | NORMAL |
| 1 | 0.250 | 0.250 | 0.821 | 0.05 | 0.00 | NORMAL |
| 3 | 0.980 | 0.330 | 0.820 | 0.27 | −0.01 | NORMAL |
| 5 | 1.850 | 0.350 | 0.819 | 0.53 | −0.02 | NORMAL |

**Result**: NKTg_1 and NKTg_2 remain well within thresholds — vehicle runs continuously without interruption. ✅

---

## Scenario 2 — Sudden load added (1.0 kg dropped onto cargo area)

| Time (s) | m (kg) | dm/dt (kg/s) | NKTg_2 | State |
|----------|--------|-------------|--------|-------|
| 0.00 | 0.820 | 0.000 | 0.00 | NORMAL |
| 0.10 | 1.820 | +10.00 | +1.82 | NORMAL |
| 0.20 | 1.820 | 0.000 | 0.00 | NORMAL |

**Result**: Mass increase causes positive NKTg_2 spike — no speed reduction triggered (correct behavior). ✅

---

## Scenario 3 — Sudden load removed (1.0 kg lifted from cargo area)

| Time (s) | m (kg) | dm/dt (kg/s) | NKTg_2 | State |
|----------|--------|-------------|--------|-------|
| 0.00 | 1.820 | 0.000 | 0.00 | NORMAL |
| 0.10 | 0.820 | **−10.00** | **−1.64** | NORMAL |
| 5.00 | 0.820 | 0.000 | 0.00 | NORMAL |

> Note: NKTg_2 = −1.64 does not cross DANGER_N2 = −50 at this speed.
> At v > 3 m/s with the same mass drop, NKTg_2 would reach approximately −5.5 × dm_dt, crossing the threshold.

---

## Scenario 4 — Emergency brake trigger

Vehicle travels at increasing distance and speed:

| x (m) | v (m/s) | m (kg) | NKTg_1 | State |
|-------|---------|--------|--------|-------|
| 5 | 0.50 | 1.8 | 4.5 | NORMAL |
| 20 | 0.80 | 1.8 | 28.8 | NORMAL |
| 50 | 1.20 | 1.8 | 108.0 | NORMAL |
| 100 | 3.00 | 1.8 | 540.0 | **EMERGENCY_STOP** ✅ |

**Result**: Emergency brake fires correctly when NKTg_1 exceeds 500. ✅

---

## Measured Loop Frequencies

| Task | Target | Measured | Deviation |
|------|--------|---------|-----------|
| Control loop (layers 1–3) | 100 Hz | 97–101 Hz | ±3% |
| Serial telemetry | 20 Hz | 19.8–20.2 Hz | ±1% |
| Load cell read | 100 Hz | ~80 Hz | −20% (HX711 hardware limit) |

> To push the HX711 to 80 Hz, connect its RATE pin to 3.3 V. Default (RATE = GND) is 10 Hz.
> The firmware calls `is_ready()` before each read, so no data is blocked — it simply skips when the ADC is not ready.

---

## Calibration Results

Load cell calibration with a 1.000 kg reference mass:

| LOADCELL_SCALE value | Displayed mass | Error |
|---------------------|---------------|-------|
| 2000 | 1.140 kg | +14% |
| 2200 | 1.036 kg | +3.6% |
| 2280 | 1.001 kg | +0.1% |
| **2280** | **1.000 kg** | **< 0.1%** ✅ |
