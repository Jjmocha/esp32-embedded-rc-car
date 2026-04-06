#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>

// ============================================================
// SteeringDriver.h
// ------------------------------------------------------------
// Low-level servo steering driver.
//
// This driver owns ONLY:
// - attaching the servo
// - clamping requested angle into safe software bounds
// - writing the angle to the servo
//
// It does NOT know about steering percent, UI, or modes.
// Higher layers should convert user intent into an angle.
// ============================================================

class SteeringDriver
{
public:
    explicit SteeringDriver(uint8_t servoPin);

    void begin();

    // Set the servo to a requested angle in degrees.
    // This function applies software safety limits.
    void setAngleDeg(int angleDeg);

    // Helper to center steering immediately.
    void center();

    int getAppliedAngleDeg() const;

private:
    uint8_t m_servoPin;
    Servo m_servo;

    int m_appliedAngleDeg;
};