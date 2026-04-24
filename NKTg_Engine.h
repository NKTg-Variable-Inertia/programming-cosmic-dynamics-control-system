#pragma once
#include "Config.h"

// ================================================================
// NKTg_Engine.h — Core NKTg mathematics
//
// Runs at 100 Hz directly on ESP32. No host computer required.
//
// NKTg_1 = x * (m * v)           position energy — detects trajectory instability
// NKTg_2 = (dm/dt) * (m * v)     mass-change energy — detects sudden load loss
// ================================================================

struct NKTgResult {
    float n1;   // NKTg_1 value
    float n2;   // NKTg_2 value
};

// Compute both NKTg values from live sensor readings
inline NKTgResult computeNKTg(float x, float v, float m, float dm_dt) {
    const float momentum = m * v;
    return { x * momentum, dm_dt * momentum };
}

// Control states derived from NKTg output
enum class VehicleState {
    NORMAL,
    REDUCE_SPEED,
    EMERGENCY_STOP
};

// Classify vehicle state using the configured danger thresholds
inline VehicleState classifyState(const NKTgResult& nktg) {
    if (nktg.n1 > DANGER_N1) return VehicleState::EMERGENCY_STOP;
    if (nktg.n2 < DANGER_N2) return VehicleState::REDUCE_SPEED;
    return VehicleState::NORMAL;
}
