#pragma once

#include <Arduino.h>

// ============================================================
// MotorDriver.h
// ------------------------------------------------------------
// Low-level driver for your dual H-bridge motor board.
//
// This driver owns ONLY the direct hardware behavior for
// the two DC motors.
//
// It does NOT know about:
// - UI
// - Wi-Fi
// - modes
// - command timeout logic
// - obstacle avoidance
//
// It only knows how to drive the motors.
// ============================================================

class MotorDriver
{
public:
    MotorDriver(
        uint8_t pwmPinA,
        uint8_t pwmPinB,
        uint8_t dir1PinA,
        uint8_t dir2PinA,
        uint8_t dir1PinB,
        uint8_t dir2PinB);

    // Initialize GPIO and PWM channels.
    void begin();

    // Stop both motors immediately.
    void stop();

    // Drive both motors forward with the same PWM magnitude.
    void driveForward(int pwm);

    // Drive both motors in reverse with the same PWM magnitude.
    void driveReverse(int pwm);

    // Optional helper hooks for future zero-turn support.
    void zeroTurnLeft(int pwm);
    void zeroTurnRight(int pwm);

    // Accessors for debug / state reporting.
    int getAppliedPwmA() const;
    int getAppliedPwmB() const;

private:
    // Hardware pins
    uint8_t m_pwmPinA;
    uint8_t m_pwmPinB;
    uint8_t m_dir1PinA;
    uint8_t m_dir2PinA;
    uint8_t m_dir1PinB;
    uint8_t m_dir2PinB;

    // ESP32 LEDC PWM channels
    int m_pwmChannelA;
    int m_pwmChannelB;

    // Last values written to hardware
    int m_appliedPwmA;
    int m_appliedPwmB;

    // Internal helpers
    void setMotorAForward(int pwm);
    void setMotorAReverse(int pwm);
    void setMotorBForward(int pwm);
    void setMotorBReverse(int pwm);

    void writePwmA(int pwm);
    void writePwmB(int pwm);
};