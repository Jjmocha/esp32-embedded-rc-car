#include "App.h"

#include <Arduino.h>
#include <WiFi.h>

#include "../config/PinConfig.h"
#include "../types/SystemConfig.h"

// ============================================================
// App.cpp
// ------------------------------------------------------------
// Wires together:
// - shared state
// - hardware drivers
// - communications
// - control logic
//
// Startup order matters:
// 1. Serial
// 2. Hardware drivers
// 3. Wi-Fi AP
// 4. Comms layer
// 5. Control layer
//
// In the update loop:
// - comms first, so newest command can arrive
// - control second, so that newest command is applied quickly
// ============================================================

App::App()
    : m_vehicleState(),
      m_motorDriver(
          PinConfig::GPIO_MOTOR_PWM_1,
          PinConfig::GPIO_MOTOR_PWM_2,
          PinConfig::GPIO_MOTOR_DIR1_A,
          PinConfig::GPIO_MOTOR_DIR2_A,
          PinConfig::GPIO_MOTOR_DIR1_B,
          PinConfig::GPIO_MOTOR_DIR2_B),
      m_steeringDriver(PinConfig::GPIO_SERVO_PWM),
      m_commsManager(m_vehicleState),
      m_controlManager(m_vehicleState, m_motorDriver, m_steeringDriver)
{
}

void App::begin()
{
    Serial.begin(115200);
    delay(500);

    Serial.println();
    Serial.println("========================================");
    Serial.println("RC2 starting...");
    Serial.println("========================================");

    // --------------------------------------------------------
    // Initialize hardware outputs first.
    // This helps us reach a known-safe state early.
    // --------------------------------------------------------
    m_motorDriver.begin();
    m_steeringDriver.begin();

    // --------------------------------------------------------
    // Start ESP32 access point.
    // --------------------------------------------------------
    Serial.println("Starting Wi-Fi access point...");

    WiFi.mode(WIFI_AP);
    WiFi.softAP(
        SystemConfig::WiFi::AP_SSID,
        SystemConfig::WiFi::AP_PASSWORD);

    Serial.print("AP SSID: ");
    Serial.println(SystemConfig::WiFi::AP_SSID);

    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());

    // --------------------------------------------------------
    // Start web/comms layer after Wi-Fi is ready.
    // --------------------------------------------------------
    m_commsManager.begin();
    Serial.println("CommsManager started.");

    // --------------------------------------------------------
    // Start control layer last.
    // It will immediately enforce safe state until commands arrive.
    // --------------------------------------------------------
    m_controlManager.begin();
    Serial.println("ControlManager started.");

    Serial.println("RC2 ready.");
}

void App::update()
{
    // Read incoming web commands first.
    m_commsManager.update();

    // Apply safety and outputs second.
    m_controlManager.update();
}