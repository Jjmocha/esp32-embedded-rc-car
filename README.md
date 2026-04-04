# ESP32 Embedded RC Car

A custom-built embedded robotic vehicle featuring real-time wireless control, multi-sensor integration, and custom PCB design. The system is controlled through a custom iPad UI over a self-hosted WiFi network.

---

## 🚀 Features

* Real-time wireless control via custom iPad interface
* ESP32-based embedded control system
* PWM motor speed and steering control (L298 H-bridge)
* Multi-sensor integration:

  * Time-of-Flight (ToF)
  * Ultrasonic
  * Line-following sensors
  * Magnetometer
* Interrupt-driven firmware for responsive sensor handling
* Custom PCB design for power distribution and sensor interfacing
* EMI reduction through optimized wiring and layout

---

## 🧠 System Architecture

User Interface (iPad)
↓
WiFi Commands
↓
ESP32
┌───────────────┐
│ Motor Control │ → DC Motor
│ Servo Control │ → Steering
│ Sensor Logic  │ ← Sensors
└───────────────┘

---

## 🛠️ Hardware

* ESP32 Dev Board
* L298 H-Bridge Motor Driver
* DC Motor + Servo
* ToF, Ultrasonic, Line Sensors, Magnetometer
* Custom-designed PCB (KiCad)

---

## 💻 Firmware

* Written in C/C++ (ESP32 platform)
* Interrupt-driven sensor handling
* PWM-based motor and steering control
* Wireless command parsing over WiFi

---

## ⚙️ Challenges & Debugging

* Diagnosed ESP32 overheating due to PCB wiring issue
* Resolved motor instability caused by signal noise
* Improved system stability through grounding and EMI reduction
* Debugged hardware using oscilloscope measurements

---

## 📸 Project Media

*(Add images here — build, wiring, UI, PCB)*

---

## 📌 Status

🚧 In Progress — expanding sensor integration and control features

