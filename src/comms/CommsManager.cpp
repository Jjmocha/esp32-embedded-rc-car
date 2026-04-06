#include "CommsManager.h"
#include "../types/Messages.h"

// ============================================================
// CommsManager.cpp
// ------------------------------------------------------------
// Converts incoming web/UI data into internal message structs
// and stores them into VehicleState.
//
// Right now:
// - receives grouped drive values from WebServerManager
// - builds DriveCommand
// - stores latest command in VehicleState
//
// Later this class can also handle:
// - mode changes
// - status requests
// - telemetry requests
// - Bluetooth / ESP-NOW / other inputs
// ============================================================

CommsManager::CommsManager(VehicleState& vehicleState)
    : m_vehicleState(vehicleState)
{
}

void CommsManager::begin()
{
    // Register callback from the web layer into this class.
    m_webServerManager.setDriveCommandCallback(
        [this](int throttlePercent, int steeringPercent)
        {
            onDriveCommandReceived(throttlePercent, steeringPercent);
        });

    m_webServerManager.begin();
}

void CommsManager::update()
{
    m_webServerManager.update();
}

void CommsManager::onDriveCommandReceived(int throttlePercent, int steeringPercent)
{
    // Clamp values into the expected UI command range.
    throttlePercent = constrain(throttlePercent, -100, 100);
    steeringPercent = constrain(steeringPercent, -100, 100);

    // Build an internal grouped drive message.
    DriveCommand command;
    command.throttlePercent = throttlePercent;
    command.steeringPercent = steeringPercent;
    command.speedPwm = 0; // Not used directly yet; ControlManager derives PWM from throttle.
    command.direction = DriveDirection::Stop; // ControlManager derives actual direction from throttle sign.
    command.timestampMs = millis();

    // Store latest command in the system state.
    m_vehicleState.setDriveCommand(command);
}