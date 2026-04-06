#include "VehicleState.h"

// ============================================================
// VehicleState.cpp
// ------------------------------------------------------------
// Straightforward storage class implementation.
//
// Right now this class is intentionally simple.
// Later, if needed, we can add:
// - validation
// - timestamp checks
// - thread safety / mutexing
// - helper update methods
// ============================================================

VehicleState::VehicleState()
{
    // Keep software mode aligned with the latest mode command
    // at startup.
    m_softwareState.activeMode = m_commandMailbox.latestModeCommand.mode;
}

void VehicleState::setDriveCommand(const DriveCommand& command)
{
    m_commandMailbox.latestDriveCommand = command;
}

void VehicleState::setModeCommand(const ModeCommand& command)
{
    m_commandMailbox.latestModeCommand = command;

    // For now, whenever a mode command is accepted,
    // we immediately reflect it in active software mode.
    // Later, this could be validated by ControlManager.
    m_softwareState.activeMode = command.mode;
}

void VehicleState::setStatusRequest(const StatusRequest& request)
{
    m_commandMailbox.latestStatusRequest = request;
}

const CommandMailbox& VehicleState::getCommandMailbox() const
{
    return m_commandMailbox;
}

CommandMailbox& VehicleState::getCommandMailbox()
{
    return m_commandMailbox;
}

const SensorData& VehicleState::getSensorData() const
{
    return m_sensorData;
}

SensorData& VehicleState::getSensorData()
{
    return m_sensorData;
}

const ActuatorData& VehicleState::getActuatorData() const
{
    return m_actuatorData;
}

ActuatorData& VehicleState::getActuatorData()
{
    return m_actuatorData;
}

const SoftwareStateData& VehicleState::getSoftwareState() const
{
    return m_softwareState;
}

SoftwareStateData& VehicleState::getSoftwareState()
{
    return m_softwareState;
}

unsigned long VehicleState::getLastDriveCommandTimeMs() const
{
    return m_commandMailbox.latestDriveCommand.timestampMs;
}