#pragma once

#include <Arduino.h>

// ============================================================
// Messages.h
// ------------------------------------------------------------
// These structs represent "messages" or "requests" moving
// between layers of the software.
//
// Think of them as grouped pieces of information the UI or
// another system sends to the car.
//
// These are NOT raw HTTP packets.
// These are internal C++ representations of meaningful commands.
// ============================================================

// ------------------------------------------------------------
// Direction requested by the user for driving.
// ------------------------------------------------------------
enum class DriveDirection
{
    Stop = 0,
    Forward,
    Reverse
};

// ------------------------------------------------------------
// High-level mode the car is operating in.
// For now we will only use Manual, but defining the others
// now keeps the architecture ready for later.
// ------------------------------------------------------------
enum class ControlMode
{
    Manual = 0,
    Assisted,
    Autonomous
};

// ------------------------------------------------------------
// DriveCommand
// ------------------------------------------------------------
// Represents the current user driving intent.
//
// throttlePercent:
//   -100 = full reverse
//      0 = stop
//   +100 = full forward
//
// steeringPercent:
//   -100 = full left
//      0 = center
//   +100 = full right
//
// speedPwm:
//   optional direct PWM-like magnitude command
//   For now, we may derive actual PWM from throttlePercent.
//   Keeping this here gives us flexibility later.
//
// timestampMs:
//   When the command was received/stored.
// ------------------------------------------------------------
struct DriveCommand
{
    int throttlePercent = 0;
    int steeringPercent = 0;
    int speedPwm = 0;
    DriveDirection direction = DriveDirection::Stop;
    unsigned long timestampMs = 0;
};

// ------------------------------------------------------------
// ModeCommand
// ------------------------------------------------------------
// Used when the UI or another input source changes the car's
// control mode.
// ------------------------------------------------------------
struct ModeCommand
{
    ControlMode mode = ControlMode::Manual;
    unsigned long timestampMs = 0;
};

// ------------------------------------------------------------
// StatusRequest
// ------------------------------------------------------------
// Used when the UI asks for data that is NOT part of the
// immediate driving decision.
//
// Example: battery voltage, speed estimate, heading, etc.
// This is separate from DriveCommand on purpose.
// ------------------------------------------------------------
struct StatusRequest
{
    bool requestBatteryVoltage = false;
    bool requestEstimatedSpeed = false;
    bool requestHeadingDeg = false;
    bool requestFrontDistanceCm = false;
    unsigned long timestampMs = 0;
};