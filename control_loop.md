# Control Loop Architecture

## Four-Layer Design

```
┌──────────────────────────────────────────────────────────────┐
│  Layer 1 — Sensing (100 Hz)                                  │
│  Encoder → v, x                                              │
│  HX711 Load Cell → m, dm/dt                                  │
│  MPU6050 → acceleration cross-check                          │
└───────────────────────────┬──────────────────────────────────┘
                            │ raw: x, v, m, dm_dt
                            ▼
┌──────────────────────────────────────────────────────────────┐
│  Layer 2 — NKTg Compute (100 Hz)                             │
│  NKTg_1 = x · (m · v)                                        │
│  NKTg_2 = (dm/dt) · (m · v)                                  │
└───────────────────────────┬──────────────────────────────────┘
                            │ NKTgResult {n1, n2}
                            ▼
┌──────────────────────────────────────────────────────────────┐
│  Layer 3 — Classification & Motor Control (100 Hz)           │
│  classifyState() → NORMAL / REDUCE_SPEED / EMERGENCY_STOP    │
│  driveMotor()   → PWM signal → BTS7960 → DC motors           │
└───────────────────────────┬──────────────────────────────────┘
                            │ (parallel, non-blocking)
                            ▼
┌──────────────────────────────────────────────────────────────┐
│  Layer 4 — Web Telemetry (20 Hz)                             │
│  USB Serial → NKTg_System.html → live charts                 │
└──────────────────────────────────────────────────────────────┘
```

## Timing Budget

| Layer | Rate | Period | Task |
|-------|------|--------|------|
| 1–3 | 100 Hz | 10 ms | Read sensors + compute NKTg + drive motors |
| 4 | 20 Hz | 50 ms | Send CSV line over USB Serial |

Layers 1–3 and Layer 4 run in the same `loop()` using non-blocking `millis()` checks — no RTOS required.

## Decision Tree

```
Every 10 ms:
  ├─ Read encoder → v, x
  ├─ Read load cell → m, dm_dt
  ├─ Compute NKTg_1 = x * (m * v)
  ├─ Compute NKTg_2 = dm_dt * (m * v)
  └─ Classify:
       NKTg_1 > 500 ?
         YES → EMERGENCY_STOP  (PWM = 0, both channels)
         NO  →
           NKTg_2 < -50 ?
             YES → REDUCE_SPEED  (PWM = 90, forward only)
             NO  → NORMAL        (PWM = 180, forward only)
```

## Why 100 Hz?

At a typical vehicle speed of 0.5 m/s, the vehicle travels 5 mm per control cycle (10 ms).
This is fine enough to catch instability events before they propagate into mechanical failure.
The load cell limits practical sensing to ~80 Hz; the extra headroom in the 100 Hz loop ensures the motor response is never the bottleneck.
