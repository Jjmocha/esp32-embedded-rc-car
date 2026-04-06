#include "SteeringDriver.h"
#include "../types/SystemConfig.h"

// ============================================================
// SteeringDriver.cpp
// ------------------------------------------------------------
// This file applies the software steering bounds defined in
// SystemConfig so we do not accidentally command the servo
// beyond the safe current range.
// ============================================================

SteeringDriver::SteeringDriver(uint8_t servoPin)
    : m_servoPin(servoPin),
      m_appliedAngleDeg(SystemConfig::Steering::SOFTWARE_CENTER_ANGLE_DEG)
{
}

void SteeringDriver::begin()
{
    // Attach the servo output pin.
    //
    // If later the MG996R needs explicit pulse width tuning,
    // we can change this to:
    // m_servo.attach(pin, minUs, maxUs);
    m_servo.attach(m_servoPin);

    // Start centered.
    center();
}

void SteeringDriver::setAngleDeg(int angleDeg)
{
    // Clamp to software-limited range.
    int clampedAngle = constrain(
        angleDeg,
        SystemConfig::Steering::SOFTWARE_MIN_ANGLE_DEG,
        SystemConfig::Steering::SOFTWARE_MAX_ANGLE_DEG);

    m_appliedAngleDeg = clampedAngle;
    m_servo.write(m_appliedAngleDeg);
}

void SteeringDriver::center()
{
    setAngleDeg(SystemConfig::Steering::SOFTWARE_CENTER_ANGLE_DEG);
}

int SteeringDriver::getAppliedAngleDeg() const
{
    return m_appliedAngleDeg;
}