# Next Applications

This ground vehicle is the entry point. NKTg scales to any system where mass varies and real-time response matters.

---

## From ground vehicle to larger systems

### Industrial robot arms
Apply NKTg_2 to detect unexpected contact forces — when a robot arm picks up an object heavier than expected, dm/dt spikes instantly, triggering a controlled hold instead of a damaging overcurrent.

### Drones and UAVs
NKTg_2 tracks mass change when payload is released mid-flight. The flight controller adjusts thrust immediately — no PID re-tuning required, physics handles it.

### Autonomous ships
NKTg_1 with `x` as lateral deviation from the planned route detects heading drift early, before GPS lag makes it visible.

### Industrial conveyor lines
A load cell array along the belt computes dm/dt continuously — missing or misplaced products are detected within one sensor cycle rather than at end-of-line inspection.

### Exoskeletons and assistive devices
NKTg_2 senses load shift on the wearer in real time, adjusting support torque before the user feels instability.

---

## Technical extensions

| Extension | Description |
|-----------|-------------|
| 3D NKTg | Extend to vector fields: `x⃗ · (m · v⃗)` across three axes |
| Predictive NKTg | Use NKTg trajectory to forecast future state rather than only react |
| Adaptive thresholds | Auto-tune DANGER_N1 / DANGER_N2 based on operating history |
| Sensor fusion | Combine GPS + IMU + encoder for higher-accuracy `x` and `v` estimates |
| Multi-agent | Share NKTg state between vehicles in a swarm for coordinated braking |

---

## Related repositories

| Repo | Status | Description |
|------|--------|-------------|
| `nktg-core` | planned | Shared C++ NKTg library (Arduino + ROS2) |
| `nktg-drone` | planned | NKTg flight controller for quadrotors |
| `nktg-arm` | planned | NKTg joint control for 6-DOF robot arms |
| `nktg-sim` | planned | Physics simulation for threshold tuning without hardware |

---

## The broader principle

From the source document:

> Wherever mass varies and real-time reflexes are required, NKTg can become the indispensable physics foundation layer. With NKTg, robots and AI no longer act according to rigid programming alone — they sense, predict, and respond to the world according to the precise laws of natural, durable stability.
