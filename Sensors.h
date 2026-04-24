#pragma once
#include <Wire.h>
#include <MPU6050.h>
#include <HX711.h>
#include "Config.h"

// ================================================================
// Sensors.h — Unified sensor management
// Covers: MPU6050 IMU, HX711 load cell, quadrature encoders
// ================================================================

extern MPU6050 imu;
extern HX711   loadcell;
extern volatile long encoderCount;

// Initialize all sensors. Returns false if MPU6050 is unreachable.
inline bool initSensors() {
    Wire.begin();

    imu.initialize();
    if (!imu.testConnection()) return false;

    loadcell.begin(HX711_DT, HX711_SCK);
    loadcell.set_scale(LOADCELL_SCALE);
    loadcell.tare();   // Zero with no load on scale

    pinMode(ENC_L_A, INPUT_PULLUP);
    pinMode(ENC_L_B, INPUT_PULLUP);

    return true;
}

// Read current mass from load cell (kg). Returns -1 if ADC not ready.
inline float readMass() {
    if (!loadcell.is_ready()) return -1.0f;
    return max(MIN_MASS, loadcell.get_units());
}

// Drain encoder count and return instantaneous velocity (m/s).
inline float computeVelocity(float dt_sec) {
    const long count = encoderCount;
    encoderCount = 0;
    const float dist = static_cast<float>(count) / PPR * WHEEL_CIRCUMFERENCE;
    return dist / dt_sec;
}
