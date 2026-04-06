#include "ControlManager.h"
#include "../types/SystemConfig.h"

// ============================================================
// ControlManager.cpp
// ------------------------------------------------------------
// Current behavior:
//
// - If drive command is stale beyond timeout:
//     -> motor hard stop
//     -> steering center
//     -> update software state flags
//
// - Otherwise:
//     -> derive direction from throttle sign
//     -> map throttle magnitude to PWM
//     -> map steering percent to servo angle
//     -> apply to hardware
//     -> store what was actually applied
//
// Notes:
// - For now we use direct mapping, no ramping.
// - For now both motors receive same PWM.
// - For now steering is centered on timeout.
// ============================================================

ControlManager::ControlManager(
    VehicleState& vehicleState,
    MotorDriver& motorDriver,
    SteeringDriver& steeringDriver)
    : m_vehicleState(vehicleState),
      m_motorDriver(motorDriver),
      m_steeringDriver(steeringDriver)
{
}

void ControlManager::begin()
{
    // Start in a known safe state.
    applyHardStop();
}

void ControlManager::update()
{
    const unsigned long nowMs = millis();

    // Check whether latest drive command is still fresh enough.
    if (isDriveCommandTimedOut(nowMs))
    {
        applyHardStop();
        return;
    }

    // If command is fresh, clear timeout-related flags and apply it.
    SoftwareStateData& softwareState = m_vehicleState.getSoftwareState();
    softwareState.commandTimedOut = false;
    softwareState.hardStopActive = false;

    applyDriveCommand();
}

bool ControlManager::isDriveCommandTimedOut(unsigned long nowMs) const
{
    const unsigned long lastCommandTimeMs = m_vehicleState.getLastDriveCommandTimeMs();

    // If command time is zero, we treat that as "no valid drive command yet".
    if (lastCommandTimeMs == 0)
    {
        return true;
    }

    return (nowMs - lastCommandTimeMs) > SystemConfig::Timing::COMMAND_TIMEOUT_MS;
}

void ControlManager::applyHardStop()
{
    // Apply immediate safe output.
    m_motorDriver.stop();
    m_steeringDriver.center();

    // Update internal live state to reflect what actually happened.
    ActuatorData& actuatorData = m_vehicleState.getActuatorData();
    actuatorData.appliedDirection = DriveDirection::Stop;
    actuatorData.appliedPwmLeft = 0;
    actuatorData.appliedPwmRight = 0;
    actuatorData.appliedSteeringAngleDeg = m_steeringDriver.getAppliedAngleDeg();

    // Update software flags.
    SoftwareStateData& softwareState = m_vehicleState.getSoftwareState();
    softwareState.commandTimedOut = true;
    softwareState.hardStopActive = true;
}

void ControlManager::applyDriveCommand()
{
    const DriveCommand& driveCommand = m_vehicleState.getCommandMailbox().latestDriveCommand;

    // Convert user intent into low-level outputs.
    const DriveDirection direction = deriveDirectionFromThrottle(driveCommand.throttlePercent);
    const int pwm = mapThrottlePercentToPwm(driveCommand.throttlePercent);
    const int steeringAngleDeg = mapSteeringPercentToAngleDeg(driveCommand.steeringPercent);

    // Always apply steering, even if stopped, so the user can
    // reposition steering while not moving.
    m_steeringDriver.setAngleDeg(steeringAngleDeg);

    // Apply motor behavior from direction + pwm.
    switch (direction)
    {
        case DriveDirection::Forward:
            m_motorDriver.driveForward(pwm);
            break;

        case DriveDirection::Reverse:
            m_motorDriver.driveReverse(pwm);
            break;

        case DriveDirection::Stop:
        default:
            m_motorDriver.stop();
            break;
    }

    // Record what was actually applied to the hardware.
    ActuatorData& actuatorData = m_vehicleState.getActuatorData();
    actuatorData.appliedDirection = direction;

    if (direction == DriveDirection::Stop)
    {
        actuatorData.appliedPwmLeft = 0;
        actuatorData.appliedPwmRight = 0;
    }
    else
    {
        actuatorData.appliedPwmLeft = m_motorDriver.getAppliedPwmA();
        actuatorData.appliedPwmRight = m_motorDriver.getAppliedPwmB();
    }

    actuatorData.appliedSteeringAngleDeg = m_steeringDriver.getAppliedAngleDeg();
}

int ControlManager::mapThrottlePercentToPwm(int throttlePercent) const
{
    // User/UI range should be approximately -100..+100.
    // We only need magnitude for PWM.
    const int magnitudePercent = abs(constrain(throttlePercent, -100, 100));

    // Map 0..100 -> PWM_MIN..DEFAULT_MAX_COMMAND_PWM
    return map(
        magnitudePercent,
        0,
        100,
        SystemConfig::Drive::PWM_MIN,
        SystemConfig::Drive::DEFAULT_MAX_COMMAND_PWM);
}

int ControlManager::mapSteeringPercentToAngleDeg(int steeringPercent) const
{
    // Clamp user steering request to expected range.
    const int clampedPercent = constrain(steeringPercent, -100, 100);

    // Map:
    //   -100 -> min angle
    //      0 -> center
    //   +100 -> max angle
    //
    // Because Arduino's map() is linear between endpoints, and your
    // center is not necessarily exactly halfway, we split the mapping
    // into two halves for better behavior.
    if (clampedPercent < 0)
    {
        return map(
            clampedPercent,
            -100,
            0,
            SystemConfig::Steering::SOFTWARE_MIN_ANGLE_DEG,
            SystemConfig::Steering::SOFTWARE_CENTER_ANGLE_DEG);
    }

    return map(
        clampedPercent,
        0,
        100,
        SystemConfig::Steering::SOFTWARE_CENTER_ANGLE_DEG,
        SystemConfig::Steering::SOFTWARE_MAX_ANGLE_DEG);
}

DriveDirection ControlManager::deriveDirectionFromThrottle(int throttlePercent) const
{
    if (throttlePercent > 0)
    {
        return DriveDirection::Forward;
    }

    if (throttlePercent < 0)
    {
        return DriveDirection::Reverse;
    }

    return DriveDirection::Stop;
}