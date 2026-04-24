#pragma once

// ================================================================
// Config.h — System-wide constants for NKTg Ground Vehicle
// Edit this file to tune vehicle behavior without touching
// any other source file.
// ================================================================

// ── Sensor calibration ──────────────────────────────────────────
#define WHEEL_CIRCUMFERENCE   0.2f      // Wheel circumference in meters — measure physically
#define PPR                   600       // Encoder pulses per revolution
#define LOADCELL_SCALE        2280.0f   // Load cell calibration factor — calibrate with known mass

// ── Timing ──────────────────────────────────────────────────────
#define CONTROL_HZ            100       // Control loop frequency (Hz)
#define SEND_HZ               20        // Web telemetry frequency (Hz)
#define DT_MS                 (1000 / CONTROL_HZ)
#define SEND_INTERVAL_MS      (1000 / SEND_HZ)

// ── NKTg thresholds ─────────────────────────────────────────────
#define DANGER_N1             500.0f    // NKTg_1 above this -> emergency brake
#define DANGER_N2            -50.0f     // NKTg_2 below this -> speed reduction

// ── Motor PWM ───────────────────────────────────────────────────
#define NORMAL_SPEED          180       // Normal PWM duty (0-255)
#define REDUCED_SPEED         90        // Reduced PWM duty (50%)
#define EMERGENCY_STOP_PWM    0
#define PWM_FREQ              5000
#define PWM_RESOLUTION        8

// ── Pin assignments ─────────────────────────────────────────────
// Encoders (input-only pins on ESP32)
#define ENC_L_A   34
#define ENC_L_B   35
#define ENC_R_A   36
#define ENC_R_B   39

// BTS7960 motor driver
#define MOTOR_RPWM  25
#define MOTOR_LPWM  26
#define MOTOR_EN    27

// HX711 load cell ADC
#define HX711_DT    4
#define HX711_SCK   5

// ── Safety ──────────────────────────────────────────────────────
#define MIN_MASS    0.1f    // Minimum mass in kg — prevents divide-by-zero
