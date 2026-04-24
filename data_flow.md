# Data Flow

## Serial Telemetry Format

The ESP32 sends one line every 50 ms (20 Hz) over USB Serial at 115200 baud:

```
x,v,m,dm_dt\n
```

Example output:

```
0.000,0.000,1.250,0.0000
0.012,0.250,1.248,-0.0020
0.031,0.380,1.245,-0.0030
0.056,0.350,1.245,0.0000
```

## Field Definitions

| Field | Unit | Decimal places | Source |
|-------|------|---------------|--------|
| `x` | meters (m) | 3 | Accumulated from encoder pulses |
| `v` | m/s | 3 | Encoder count / dt |
| `m` | kg | 3 | HX711 load cell |
| `dm_dt` | kg/s | 4 | Finite difference: (m_curr − m_prev) / dt |

## Startup Handshake

On successful initialization, the ESP32 sends exactly one line before telemetry begins:

```
READY
```

`NKTg_System.html` waits for this string before starting chart updates.
If `READY` is not received within 5 seconds, the dashboard shows a connection error.

## Full System Data Flow

```
[Left encoder]  ──────────────────────┐
[Right encoder] ──────────────────────┤
[MPU6050]       ──────────────────────┼──► [ESP32 — NKTg_Engine] ──► [BTS7960] ──► [Motors]
[HX711]         ──────────────────────┘            │
                                                   │ USB Serial (20 Hz)
                                                   ▼
                                         [NKTg_System.html]
                                         Live NKTg_1 / NKTg_2 charts
```

## NKTg Values Computed in Dashboard

The HTML dashboard recomputes NKTg_1 and NKTg_2 from the raw fields for display:

```javascript
const n1 = x * (m * v);           // NKTg_1
const n2 = dm_dt * (m * v);       // NKTg_2
```

This mirrors the on-chip computation and allows visual verification that the ESP32 and dashboard are in sync.
