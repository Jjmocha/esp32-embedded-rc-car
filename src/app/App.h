#pragma once

#include "../state/VehicleState.h"
#include "../comms/CommsManager.h"
#include "../control/ControlManager.h"
#include "../drivers/MotorDriver.h"
#include "../drivers/SteeringDriver.h"

// ============================================================
// App.h
// ------------------------------------------------------------
// High-level application object.
//
// This class wires the major layers together and provides
// simple begin()/update() entry points for main.cpp.
//
// It helps keep main.cpp tiny and clean.
// ============================================================

class App
{
public:
    App();

    void begin();
    void update();

private:
    // Central shared state
    VehicleState m_vehicleState;

    // Low-level drivers
    MotorDriver m_motorDriver;
    SteeringDriver m_steeringDriver;

    // Middle / upper layers
    CommsManager m_commsManager;
    ControlManager m_controlManager;
};