# ESP32 Embedded RC Car

This project is a custom-built embedded robotic vehicle designed to explore real-time control, wireless communication, and multi-sensor integration. The system is controlled through a custom iPad interface over a self-hosted WiFi network.

## Overview

The goal of this project is to develop a complete embedded system that integrates hardware, firmware, and user interaction. The vehicle supports real-time control and incorporates multiple sensors for future autonomous features.

## Features

* Wireless control via custom iPad UI over WiFi
* ESP32-based control system
* PWM motor and steering control using L298 H-bridge
* Integration of ToF, ultrasonic, line-following, and magnetometer sensors
* Interrupt-driven firmware for responsive sensor handling
* Custom PCB design for power distribution and sensor interfacing

## System Description

The iPad interface sends control commands over a WiFi network hosted by the ESP32.
The ESP32 processes these commands and updates motor speed and steering accordingly.
Sensor data is handled using interrupt-driven routines to maintain responsiveness.

## Hardware

* ESP32 development board
* L298 H-bridge motor driver
* DC motor and steering servo
* ToF, ultrasonic, line sensors, and magnetometer
* Custom PCB (KiCad)

## Software

* Written in C/C++ for the ESP32 platform
* PWM-based motor and steering control
* Interrupt-based sensor handling
* Wireless communication for command input

## Development Notes

* Resolved power integrity and signal noise issues using oscilloscope analysis
* Improved system stability through wiring and layout changes to reduce EMI
* Debugged hardware/software interaction issues during integration

## Media

### Images

![RC Car](images/car.jpg)
![UI]<img width="1210" height="834" alt="IMG_0550" src="https://github.com/user-attachments/assets/23f33ee9-5bc5-4465-afe6-6f4bcec9bea6" />


### Video

[Demo Video][(https://youtube.com/your-link)](https://drive.google.com/file/d/1IzowMYk3Depq9Ds3jsoLurYnOoPi3Gak/view?usp=drivesdk)

## Status

In progress

