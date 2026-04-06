#include "MotorDriver.h"
#include "../types/SystemConfig.h"

// ============================================================
// MotorDriver.cpp
// ------------------------------------------------------------
// Notes about the control style used here:
//
// Each H-bridge channel gets:
// - two direction pins
// - one PWM pin
//
// For now:
// - both motors always use the same magnitude in normal driving
// - future zero-turn support is provided as helper functions
//
// If later your motor orientation causes one side to need the
// opposite direction for "forward", we can fix that here in
// one place without touching higher-level code.
// ============================================================

namespace
{
    // Chosen LEDC channels for ESP32 PWM output.
    // Keeping these private to this translation unit.
    constexpr int kMotorPwmChannelA = 0;
    constexpr int kMotorPwmChannelB = 1;

    constexpr int kPwmFrequencyHz = 20000;   // 20 kHz, typically quiet for motors
    constexpr int kPwmResolutionBits = 8;    // 0..255
}

MotorDriver::MotorDriver(
    uint8_t pwmPinA,
    uint8_t pwmPinB,
    uint8_t dir1PinA,
    uint8_t dir2PinA,
    uint8_t dir1PinB,
    uint8_t dir2PinB)
    : m_pwmPinA(pwmPinA),
      m_pwmPinB(pwmPinB),
      m_dir1PinA(dir1PinA),
      m_dir2PinA(dir2PinA),
      m_dir1PinB(dir1PinB),
      m_dir2PinB(dir2PinB),
      m_pwmChannelA(kMotorPwmChannelA),
      m_pwmChannelB(kMotorPwmChannelB),
      m_appliedPwmA(0),
      m_appliedPwmB(0)
{
}

void MotorDriver::begin()
{
    // Set direction pins as outputs.
    pinMode(m_dir1PinA, OUTPUT);
    pinMode(m_dir2PinA, OUTPUT);
    pinMode(m_dir1PinB, OUTPUT);
    pinMode(m_dir2PinB, OUTPUT);

    // Configure PWM for each motor channel.
    ledcSetup(m_pwmChannelA, kPwmFrequencyHz, kPwmResolutionBits);
    ledcSetup(m_pwmChannelB, kPwmFrequencyHz, kPwmResolutionBits);

    ledcAttachPin(m_pwmPinA, m_pwmChannelA);
    ledcAttachPin(m_pwmPinB, m_pwmChannelB);

    // Start safe.
    stop();
}

void MotorDriver::stop()
{
    // Coast-style stop at the H-bridge logic side:
    // both direction pins low, PWM zero.
    //
    // If later you want active braking, we can add a separate
    // brake() function depending on how your driver behaves.
    digitalWrite(m_dir1PinA, LOW);
    digitalWrite(m_dir2PinA, LOW);

    digitalWrite(m_dir1PinB, LOW);
    digitalWrite(m_dir2PinB, LOW);

    writePwmA(0);
    writePwmB(0);
}

void MotorDriver::driveForward(int pwm)
{
    // Both motors driven same direction for straight driving.
    setMotorAForward(pwm);
    setMotorBForward(pwm);
}

void MotorDriver::driveReverse(int pwm)
{
    // Both motors driven same direction for reverse driving.
    setMotorAReverse(pwm);
    setMotorBReverse(pwm);
}

void MotorDriver::zeroTurnLeft(int pwm)
{
    // One side reverse, other side forward.
    // Exact orientation may need flipping depending on your
    // motor wiring and physical mounting.
    setMotorAReverse(pwm);
    setMotorBForward(pwm);
}

void MotorDriver::zeroTurnRight(int pwm)
{
    // Opposite of zeroTurnLeft().
    setMotorAForward(pwm);
    setMotorBReverse(pwm);
}

int MotorDriver::getAppliedPwmA() const
{
    return m_appliedPwmA;
}

int MotorDriver::getAppliedPwmB() const
{
    return m_appliedPwmB;
}

void MotorDriver::setMotorAForward(int pwm)
{
    pwm = constrain(pwm, SystemConfig::Drive::PWM_MIN, SystemConfig::Drive::PWM_MAX);

    digitalWrite(m_dir1PinA, HIGH);
    digitalWrite(m_dir2PinA, LOW);
    writePwmA(pwm);
}

void MotorDriver::setMotorAReverse(int pwm)
{
    pwm = constrain(pwm, SystemConfig::Drive::PWM_MIN, SystemConfig::Drive::PWM_MAX);

    digitalWrite(m_dir1PinA, LOW);
    digitalWrite(m_dir2PinA, HIGH);
    writePwmA(pwm);
}

void MotorDriver::setMotorBForward(int pwm)
{
    pwm = constrain(pwm, SystemConfig::Drive::PWM_MIN, SystemConfig::Drive::PWM_MAX);

    digitalWrite(m_dir1PinB, HIGH);
    digitalWrite(m_dir2PinB, LOW);
    writePwmB(pwm);
}

void MotorDriver::setMotorBReverse(int pwm)
{
    pwm = constrain(pwm, SystemConfig::Drive::PWM_MIN, SystemConfig::Drive::PWM_MAX);

    digitalWrite(m_dir1PinB, LOW);
    digitalWrite(m_dir2PinB, HIGH);
    writePwmB(pwm);
}

void MotorDriver::writePwmA(int pwm)
{
    m_appliedPwmA = constrain(pwm, SystemConfig::Drive::PWM_MIN, SystemConfig::Drive::PWM_MAX);
    ledcWrite(m_pwmChannelA, m_appliedPwmA);
}

void MotorDriver::writePwmB(int pwm)
{
    m_appliedPwmB = constrain(pwm, SystemConfig::Drive::PWM_MIN, SystemConfig::Drive::PWM_MAX);
    ledcWrite(m_pwmChannelB, m_appliedPwmB);
}