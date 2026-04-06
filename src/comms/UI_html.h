#pragma once

// ============================================================
// UI_html.h
// ------------------------------------------------------------
// Stores the HTML/JS for the RC2 web control page.
//
// Current UI choice:
// - left slider   -> throttle (-100 .. +100)
// - right slider  -> steering (-100 .. +100)
//
// Both sliders centered means:
// - throttle = 0  -> stop
// - steering = 0  -> center
//
// This UI sends grouped drive commands to:
//
//   /drive?throttle=<value>&steer=<value>
//
// Notes:
// - We keep this simple for now using GET requests.
// - Later we can switch to JSON POST if desired.
// ============================================================

static const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>RC2 Control</title>
  <style>
    body {
      margin: 0;
      padding: 20px;
      background: #111;
      color: white;
      font-family: Arial, sans-serif;
      text-align: center;
    }

    h1 {
      margin-bottom: 10px;
    }

    p {
      margin-top: 4px;
      margin-bottom: 4px;
    }

    .panel {
      max-width: 900px;
      margin: 0 auto;
    }

    .status-box {
      margin-top: 12px;
      margin-bottom: 20px;
      padding: 12px;
      border-radius: 10px;
      background: #1e1e1e;
    }

    .slider-row {
      display: flex;
      justify-content: center;
      align-items: flex-end;
      gap: 80px;
      margin-top: 20px;
      flex-wrap: wrap;
    }

    .slider-group {
      display: flex;
      flex-direction: column;
      align-items: center;
      width: 180px;
    }

    .slider-label {
      margin-bottom: 12px;
      font-size: 18px;
      font-weight: bold;
    }

    /* Vertical slider styling */
    input[type=range].vertical {
      -webkit-appearance: slider-vertical;
      width: 40px;
      height: 280px;
      background: transparent;
    }

    /* Horizontal slider styling */
    input[type=range].horizontal {
      width: 280px;
      margin-top: 20px;
    }

    .value-box {
      margin-top: 12px;
      font-size: 16px;
      min-height: 24px;
    }

    .button-row {
      margin-top: 30px;
    }

    button {
      font-size: 18px;
      padding: 10px 18px;
      margin: 6px;
      border: none;
      border-radius: 10px;
      cursor: pointer;
      background: #2b2b2b;
      color: white;
    }

    button:active {
      background: #4a4a4a;
    }

    .hint {
      opacity: 0.8;
      font-size: 14px;
      margin-top: 16px;
    }
  </style>
</head>
<body>
  <div class="panel">
    <h1>RC2 Controller</h1>
    <p>Left slider = throttle, right slider = steering</p>

    <div class="status-box">
      <p id="statusText">Status: Waiting</p>
      <p id="throttleText">Throttle: 0</p>
      <p id="steerText">Steering: 0</p>
    </div>

    <div class="slider-row">
      <div class="slider-group">
        <div class="slider-label">Throttle</div>
        <!--
          Throttle:
            +100 = forward
               0 = stop
            -100 = reverse

          We orient the slider visually so top feels like forward.
          Because browser vertical sliders often increase downward,
          we invert in JavaScript.
        -->
        <input
          id="throttleSlider"
          class="vertical"
          type="range"
          min="-100"
          max="100"
          value="0"
          step="1">
        <div class="value-box" id="throttleValue">0</div>
      </div>

      <div class="slider-group">
        <div class="slider-label">Steering</div>
        <!--
          Steering:
            -100 = left
               0 = center
            +100 = right
        -->
        <input
          id="steerSlider"
          class="horizontal"
          type="range"
          min="-100"
          max="100"
          value="0"
          step="1">
        <div class="value-box" id="steerValue">0</div>
      </div>
    </div>

    <div class="button-row">
      <button id="centerSteeringBtn">Center Steering</button>
      <button id="stopBtn">Stop</button>
    </div>

    <div class="hint">
      Timeout safety is active. If commands stop arriving, the car will hard stop.
    </div>
  </div>

  <script>
    // ----------------------------------------------------------
    // Base URL
    // ----------------------------------------------------------
    // Empty string means same host that served the page.
    const baseURL = "";

    // ----------------------------------------------------------
    // UI elements
    // ----------------------------------------------------------
    const throttleSlider = document.getElementById("throttleSlider");
    const steerSlider = document.getElementById("steerSlider");

    const statusText = document.getElementById("statusText");
    const throttleText = document.getElementById("throttleText");
    const steerText = document.getElementById("steerText");

    const throttleValue = document.getElementById("throttleValue");
    const steerValue = document.getElementById("steerValue");

    const centerSteeringBtn = document.getElementById("centerSteeringBtn");
    const stopBtn = document.getElementById("stopBtn");

    // ----------------------------------------------------------
    // Send grouped drive command
    // ----------------------------------------------------------
    // Sends both throttle and steering together.
    // This matches the architecture we discussed:
    // driving intent belongs together.
    // ----------------------------------------------------------
    function sendDriveCommand(throttle, steer) {
      const url = baseURL + "/drive?throttle=" + throttle + "&steer=" + steer;

      fetch(url)
        .then(response => {
          if (!response.ok) {
            throw new Error("HTTP " + response.status);
          }

          statusText.textContent = "Status: Sent drive command";
        })
        .catch(err => {
          console.log("Error sending drive command:", err);
          statusText.textContent = "Status: Error sending command";
        });
    }

    // ----------------------------------------------------------
    // Update on-screen text and send command
    // ----------------------------------------------------------
    function updateAndSend() {
      // Throttle slider is visually vertical.
      // To make "up" feel like forward, we invert the sign.
      const rawThrottle = parseInt(throttleSlider.value, 10);
      const throttle = -rawThrottle;

      const steer = parseInt(steerSlider.value, 10);

      throttleValue.textContent = throttle;
      steerValue.textContent = steer;

      throttleText.textContent = "Throttle: " + throttle;
      steerText.textContent = "Steering: " + steer;

      sendDriveCommand(throttle, steer);
    }

    // ----------------------------------------------------------
    // Events
    // ----------------------------------------------------------
    throttleSlider.addEventListener("input", updateAndSend);
    steerSlider.addEventListener("input", updateAndSend);

    centerSteeringBtn.addEventListener("click", () => {
      steerSlider.value = 0;
      updateAndSend();
    });

    stopBtn.addEventListener("click", () => {
      throttleSlider.value = 0;
      updateAndSend();
    });

    // Initialize visible values.
    updateAndSend();
  </script>
</body>
</html>
)rawliteral";