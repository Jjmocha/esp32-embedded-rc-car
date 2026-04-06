#pragma once

#include "../state/VehicleState.h"
#include "WebServerManager.h"

// ============================================================
// CommsManager.h
// ------------------------------------------------------------
// Top-level communications manager.
//
// Responsibilities right now:
// - own the WebServerManager
// - translate incoming UI/web requests into internal messages
// - write those messages into VehicleState
//
// This class acts like a bridge:
//
//   Web/UI  --->  CommsManager  --->  VehicleState
//
// That means control logic never needs to know anything about
// HTTP parsing details.
// ============================================================

class CommsManager
{
public:
    explicit CommsManager(VehicleState& vehicleState);

    void begin();
    void update();

private:
    VehicleState& m_vehicleState;
    WebServerManager m_webServerManager;

    // Callback target for grouped drive commands.
    void onDriveCommandReceived(int throttlePercent, int steeringPercent);
};