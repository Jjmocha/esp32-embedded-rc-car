#pragma once

#include "Messages.h"

// ============================================================
// VehicleTypes.h
// ------------------------------------------------------------
// This file defines internal data "shapes" used throughout
// the car software.
//
// Important distinction:
// - Messages.h  -> what outside layers ask for / send
// - VehicleTypes.h -> what the system internally tracks
//
// These are just type definitions.
// They do NOT store the live state themselves.
// ============================================================

// ------------------------------------------------------------
// SensorData
// ------------------------------------------------------------
// Snapshot of sensor-related information currently known
// by the system.
//
// Even if some sensors are not implemented yet, defining
// the fields now helps shape the architecture cleanly.
// ------------------------------------------------------------
struct SensorData
{
    float batteryVoltage = 0.0f;
    float estimatedSpeedMps = 0.0f;
    float headingDeg = 0.0f;

    int frontDistanceCm = -1;

    long leftMotorEncoderTicks = 0;
    long rightMotorEncoderTicks = 0;
};

// ------------------------------------------------------------
// ActuatorData
// ------------------------------------------------------------
// Records what outputs the software is currently applying
// to hardware.
//
// This is useful for debugging because "requested command"
// and "applied output" are not always the same thing.
//
// Example:
// - user requests forward
// - safety logic sees obstacle
// - applied output becomes stop
// ------------------------------------------------------------
struct ActuatorData
{
    DriveDirection appliedDirection = DriveDirection::Stop;

    int appliedPwmLeft = 0;
    int appliedPwmRight = 0;

    int appliedSteeringAngleDeg = 0;
};

// ------------------------------------------------------------
// SoftwareStateData
// ------------------------------------------------------------
// Tracks high-level software state and flags.
//
// These are not raw sensor values and not raw user requests.
// They describe what the software believes is going on.
// ------------------------------------------------------------
struct SoftwareStateData
{
    ControlMode activeMode = ControlMode::Manual;

    bool commandTimedOut = false;
    bool hardStopActive = false;
    bool obstacleOverrideActive = false;
};

// ------------------------------------------------------------
// CommandMailbox
// ------------------------------------------------------------
// Stores the latest command-style messages received.
//
// The word "mailbox" is useful here because for RC control
// we generally care about the latest message, not an entire
// queue of old inputs.
// ------------------------------------------------------------
struct CommandMailbox
{
    DriveCommand latestDriveCommand;
    ModeCommand latestModeCommand;
    StatusRequest latestStatusRequest;
};