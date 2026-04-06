#include "WebServerManager.h"
#include "UI_html.h"

// ============================================================
// WebServerManager.cpp
// ------------------------------------------------------------
// Current routes:
//
//   GET /
//     -> serves control webpage
//
//   GET /drive?throttle=<int>&steer=<int>
//     -> forwards grouped drive input to callback
//
// Notes:
// - For now we use query-string GET requests.
// - Later we can add:
//     /mode
//     /status
//     /telemetry
//     JSON POST endpoints
// ============================================================

WebServerManager::WebServerManager()
    : m_server(80)
{
}

void WebServerManager::begin()
{
    registerRoutes();
    m_server.begin();
}

void WebServerManager::update()
{
    m_server.handleClient();
}

void WebServerManager::setDriveCommandCallback(DriveCommandCallback callback)
{
    m_driveCommandCallback = callback;
}

void WebServerManager::registerRoutes()
{
    // Main webpage
    m_server.on("/", HTTP_GET, [this]() { handleRoot(); });

    // Grouped drive command
    m_server.on("/drive", HTTP_GET, [this]() { handleDrive(); });

    // 404 handler
    m_server.onNotFound([this]() { handleNotFound(); });
}

void WebServerManager::handleRoot()
{
    m_server.send(200, "text/html", index_html);
}

void WebServerManager::handleDrive()
{
    // Validate required args exist.
    if (!m_server.hasArg("throttle") || !m_server.hasArg("steer"))
    {
        m_server.send(400, "text/plain", "Missing throttle or steer argument");
        return;
    }

    // Parse integer values from query args.
    const int throttlePercent = m_server.arg("throttle").toInt();
    const int steeringPercent = m_server.arg("steer").toInt();

    // Forward parsed values to higher-level callback if registered.
    if (m_driveCommandCallback)
    {
        m_driveCommandCallback(throttlePercent, steeringPercent);
    }

    m_server.send(200, "text/plain", "ok");
}

void WebServerManager::handleNotFound()
{
    m_server.send(404, "text/plain", "Not found");
}