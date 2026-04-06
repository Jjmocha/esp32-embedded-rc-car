#pragma once

#include <stdint.h>

// ============================================================
// PinConfig.h
// ------------------------------------------------------------
// Central place for all ESP32 pin assignments.
// These are hardware wiring definitions only.
// Do NOT put software limits, timing, or behavior values here.
// ============================================================

namespace PinConfig
{
    // ---------------- I2C ----------------
    constexpr uint8_t GPIO_SDA = 21;
    constexpr uint8_t GPIO_SCL = 22;

    // ---------------- ToF Sensors ----------------
    constexpr uint8_t GPIO_ToF_XSHUT_1 = 23;
    constexpr uint8_t GPIO_ToF_XSHUT_2 = 27;
    constexpr uint8_t GPIO_ToF_XSHUT_3 = 33;
    constexpr uint8_t GPIO_ToF_XSHUT_4 = 2;   // ESP32 boot strap pin; must remain HIGH at boot

    // ---------------- Motor Driver ----------------
    constexpr uint8_t GPIO_MOTOR_PWM_1  = 4;
    constexpr uint8_t GPIO_MOTOR_PWM_2  = 5;
    constexpr uint8_t GPIO_MOTOR_DIR1_A = 12; // ESP32 boot strap pin; must remain LOW at boot
    constexpr uint8_t GPIO_MOTOR_DIR2_A = 13;
    constexpr uint8_t GPIO_MOTOR_DIR1_B = 14;
    constexpr uint8_t GPIO_MOTOR_DIR2_B = 15; // ESP32 boot strap pin; must remain HIGH at boot

    // ---------------- Servo ----------------
    constexpr uint8_t GPIO_SERVO_PWM = 16;

    // ---------------- Encoders ----------------
    constexpr uint8_t GPIO_MOTOR_ENCODER_1      = 17;
    constexpr uint8_t GPIO_MOTOR_ENCODER_2      = 18;
    constexpr uint8_t GPIO_MAGNETOMETER_ENCODER = 19;

    // ---------------- Ultrasonic ----------------
    constexpr uint8_t GPIO_ULTRASONIC_TRIG = 25;
    constexpr uint8_t GPIO_ULTRASONIC_ECHO = 26;

    // ---------------- Line / Reflectance Sensors ----------------
    constexpr uint8_t GPIO_TRCT5000L_L  = 32;
    constexpr uint8_t GPIO_TRCT5000L_ML = 34;
    constexpr uint8_t GPIO_TRCT5000L_M  = 35;
    constexpr uint8_t GPIO_TRCT5000L_MR = 36;
    constexpr uint8_t GPIO_TRCT5000L_R  = 39;
}