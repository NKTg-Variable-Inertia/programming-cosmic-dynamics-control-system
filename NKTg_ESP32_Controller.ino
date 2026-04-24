// ================================================================
// NKTg_ESP32_Controller.ino
// NKTg Real-time Controller — ESP32 + Physical Sensors
// Production firmware: MPU6050 + Encoder + Load Cell
//
// Sends "x,v,m,dm_dt" to NKTg_System.html via USB Serial at 20 Hz.
// Motor control runs autonomously at 100 Hz without a host computer.
// ================================================================

#include <Wire.h>
#include <MPU6050.h>
#include <HX711.h>
#include "Config.h"
#include "NKTg_Engine.h"
#include "Sensors.h"
#include "MotorControl.h"

// ── Global objects ───────────────────────────────────────────────
MPU6050 imu;
HX711   loadcell;
volatile long encoderCount = 0;

// ── State variables ──────────────────────────────────────────────
float x      = 0.0f;   // Position (m)
float v      = 0.0f;   // Velocity (m/s)
float m_curr = 0.0f;   // Current mass (kg)
float m_prev = 0.0f;   // Previous mass reading (kg)
float dm_dt  = 0.0f;   // Mass rate of change (kg/s)

unsigned long lastTime = 0;
unsigned long lastSend = 0;

// ── Encoder ISR ──────────────────────────────────────────────────
void IRAM_ATTR encoderISR() {
    if (digitalRead(ENC_L_B) > 0) encoderCount++;
    else                           encoderCount--;
}

// ================================================================
// SETUP
// ================================================================
void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);

    if (!initSensors()) {
        Serial.println("ERROR: sensor init failed — check MPU6050 wiring");
        while (true) delay(1000);
    }

    attachInterrupt(digitalPinToInterrupt(ENC_L_A), encoderISR, RISING);
    initMotors();

    m_curr = readMass();
    m_prev = m_curr;
    lastTime = millis();

    Serial.println("READY");   // Handshake with NKTg_System.html
}

// ================================================================
// MAIN LOOP
// ================================================================
void loop() {
    const unsigned long now = millis();

    // ── Layer 1-3: 100 Hz control loop ───────────────────────────
    if (now - lastTime >= DT_MS) {
        const float dt_sec = (now - lastTime) / 1000.0f;
        lastTime = now;

        // Velocity and position from encoder
        v  = computeVelocity(dt_sec);
        x += v * dt_sec;

        // Mass and dm/dt from load cell
        const float m_new = readMass();
        if (m_new > 0.0f) {
            m_prev = m_curr;
            m_curr = m_new;
            dm_dt  = (m_curr - m_prev) / dt_sec;
        }

        // Compute NKTg and drive motors
        const NKTgResult nktg = computeNKTg(x, v, m_curr, dm_dt);
        driveMotor(classifyState(nktg));
    }

    // ── Layer 4: 20 Hz telemetry to web dashboard ─────────────────
    if (now - lastSend >= SEND_INTERVAL_MS) {
        lastSend = now;
        // Format: "x,v,m,dm_dt\n"
        Serial.print(x,      3); Serial.print(',');
        Serial.print(v,      3); Serial.print(',');
        Serial.print(m_curr, 3); Serial.print(',');
        Serial.println(dm_dt, 4);
    }
}
