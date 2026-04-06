#pragma once

// ============================================================
// SystemConfig.h
// ------------------------------------------------------------
// Central place for non-pin configuration values.
//
// Put things here like:
// - software steering limits
// - PWM limits
// - command timeout values
// - Wi-Fi credentials
// - future safety thresholds
//
// Do NOT put hardware pin assignments here.
// Those belong in PinConfig.h
// ============================================================

namespace SystemConfig
{
    // --------------------------------------------------------
    // Wi-Fi Access Point settings
    // --------------------------------------------------------
    namespace WiFi
    {
        constexpr const char* AP_SSID = "RC2_CAR";
        constexpr const char* AP_PASSWORD = "12345678";
    }

    // --------------------------------------------------------
    // Steering configuration
    // --------------------------------------------------------
    // These are temporary software steering limits based on
    // your current rough calibration.
    //
    // You said something around:
    // left ~ 70
    // center ~ somewhere in the middle
    // right ~ 95
    // --------------------------------------------------------
    namespace Steering
    {
        // Physical / approximate range currently believed usable
        constexpr int SOFTWARE_MIN_ANGLE_DEG = 70;
        constexpr int SOFTWARE_CENTER_ANGLE_DEG = 82;
        constexpr int SOFTWARE_MAX_ANGLE_DEG = 95;

        // Input UI steering percent range
        constexpr int INPUT_PERCENT_MIN = -100;
        constexpr int INPUT_PERCENT_CENTER = 0;
        constexpr int INPUT_PERCENT_MAX = 100;
    }

    // --------------------------------------------------------
    // Drive / PWM configuration
    // --------------------------------------------------------
    namespace Drive
    {
        constexpr int PWM_MIN = 0;
        constexpr int PWM_MAX = 255;

        // Maximum command magnitude allowed from UI mapping
        constexpr int DEFAULT_MAX_COMMAND_PWM = 255;
    }

    // --------------------------------------------------------
    // Timing configuration
    // --------------------------------------------------------
    namespace Timing
    {
        // If no fresh drive command arrives within this time,
        // we hard stop for safety/debugging.
        constexpr unsigned long COMMAND_TIMEOUT_MS = 300;

        // We may use these in later managers.
        constexpr unsigned long CONTROL_UPDATE_PERIOD_MS = 10;
        constexpr unsigned long SENSOR_UPDATE_PERIOD_MS = 20;
        constexpr unsigned long STATUS_UPDATE_PERIOD_MS = 100;
    }

    // --------------------------------------------------------
    // Safety-related configuration
    // --------------------------------------------------------
    namespace Safety
    {
        // Placeholder for future logic.
        constexpr int MIN_FRONT_DISTANCE_CM = 15;
    }
} 