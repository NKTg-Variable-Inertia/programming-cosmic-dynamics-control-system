#pragma once
#include "Config.h"
#include "NKTg_Engine.h"

// ================================================================
// MotorControl.h — BTS7960 motor driver interface
// Maps VehicleState to LEDC PWM output on channels 0 and 1
// ================================================================

inline void initMotors() {
    ledcSetup(0, PWM_FREQ, PWM_RESOLUTION);
    ledcSetup(1, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(MOTOR_RPWM, 0);
    ledcAttachPin(MOTOR_LPWM, 1);
    pinMode(MOTOR_EN, OUTPUT);
    digitalWrite(MOTOR_EN, HIGH);
}

inline void driveMotor(VehicleState state) {
    switch (state) {
        case VehicleState::EMERGENCY_STOP:
            ledcWrite(0, EMERGENCY_STOP_PWM);
            ledcWrite(1, EMERGENCY_STOP_PWM);
            break;

        case VehicleState::REDUCE_SPEED:
            ledcWrite(0, REDUCED_SPEED);
            ledcWrite(1, 0);
            break;

        case VehicleState::NORMAL:
        default:
            ledcWrite(0, NORMAL_SPEED);
            ledcWrite(1, 0);
            break;
    }
}
