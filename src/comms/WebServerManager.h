#pragma once

#include <WebServer.h>
#include <functional>

// ============================================================
// WebServerManager.h
// ------------------------------------------------------------
// Owns the low-level HTTP server and route registration.
//
// This class is intentionally responsible for:
// - creating/starting the WebServer
// - registering HTTP routes
// - serving the UI page
// - parsing request arguments at route level
//
// It does NOT directly decide vehicle behavior.
// Instead, it forwards parsed values upward through callbacks.
//
// This keeps the web layer separate from the control logic.
// ============================================================

class WebServerManager
{
public:
    // Callback type for grouped drive command input.
    // Parameters:
    //   throttlePercent
    //   steeringPercent
    using DriveCommandCallback = std::function<void(int, int)>;

    WebServerManager();

    // Start the server and register routes.
    void begin();

    // Must be called repeatedly in the main loop.
    void update();

    // Register callback used when /drive is received.
    void setDriveCommandCallback(DriveCommandCallback callback);

private:
    WebServer m_server;
    DriveCommandCallback m_driveCommandCallback;

    void registerRoutes();

    // Individual route handlers
    void handleRoot();
    void handleDrive();
    void handleNotFound();
};