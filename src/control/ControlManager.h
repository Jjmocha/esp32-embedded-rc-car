#pragma once

#include "../state/VehicleState.h"
#include "../drivers/MotorDriver.h"
#include "../drivers/SteeringDriver.h"

// ============================================================
// ControlManager.h
// ------------------------------------------------------------
// The control layer is the "brain" between:
//
// - requested commands (from comms / UI)
// - current software safety rules
// - hardware drivers
//
// For now it does three important jobs:
// 1. Read latest drive command from VehicleState
// 2. Apply command timeout hard-stop safety
// 3. Convert throttle/steering percentages into hardware outputs
//
// Later this is where things like obstacle override, heading
// hold, speed ramping, and autonomous logic will live.
// ============================================================

class ControlManager
{
public:
    ControlManager(
        VehicleState& vehicleState,
        MotorDriver& motorDriver,
        SteeringDriver& steeringDriver);

    void begin();

    // Run one control update step.
    // Call this repeatedly from the main application loop.
    void update();

private:
    VehicleState& m_vehicleState;
    MotorDriver& m_motorDriver;
    SteeringDriver& m_steeringDriver;

    // Internal helper steps
    bool isDriveCommandTimedOut(unsigned long nowMs) const;
    void applyHardStop();
    void applyDriveCommand();

    // Mapping helpers
    int mapThrottlePercentToPwm(int throttlePercent) const;
    int mapSteeringPercentToAngleDeg(int steeringPercent) const;
    DriveDirection deriveDirectionFromThrottle(int throttlePercent) const;
};