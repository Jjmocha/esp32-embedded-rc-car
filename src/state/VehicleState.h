#pragma once

#include "../types/Messages.h"
#include "../types/VehicleTypes.h"

// ============================================================
// VehicleState.h
// ------------------------------------------------------------
// VehicleState owns the car's current live software state.
//
// This is the "single source of truth" for:
// - latest received commands
// - latest sensor readings
// - latest applied actuator outputs
// - current software mode / flags
//
// Other layers should read from and write to this object
// rather than each layer inventing its own disconnected state.
// ============================================================

class VehicleState
{
public:
    VehicleState();

    // --------------------------------------------------------
    // Command update functions
    // --------------------------------------------------------
    void setDriveCommand(const DriveCommand& command);
    void setModeCommand(const ModeCommand& command);
    void setStatusRequest(const StatusRequest& request);

    // --------------------------------------------------------
    // Access to command mailbox
    // --------------------------------------------------------
    const CommandMailbox& getCommandMailbox() const;
    CommandMailbox& getCommandMailbox();

    // --------------------------------------------------------
    // Access to sensor data
    // --------------------------------------------------------
    const SensorData& getSensorData() const;
    SensorData& getSensorData();

    // --------------------------------------------------------
    // Access to actuator data
    // --------------------------------------------------------
    const ActuatorData& getActuatorData() const;
    ActuatorData& getActuatorData();

    // --------------------------------------------------------
    // Access to software state
    // --------------------------------------------------------
    const SoftwareStateData& getSoftwareState() const;
    SoftwareStateData& getSoftwareState();

    // --------------------------------------------------------
    // Convenience helpers
    // --------------------------------------------------------
    unsigned long getLastDriveCommandTimeMs() const;

private:
    // Latest received command-like messages
    CommandMailbox m_commandMailbox;

    // Latest sensor snapshot
    SensorData m_sensorData;

    // Latest applied hardware output snapshot
    ActuatorData m_actuatorData;

    // High-level software state flags / mode
    SoftwareStateData m_softwareState;
};