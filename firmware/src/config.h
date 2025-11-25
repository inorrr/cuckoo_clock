#ifndef CONFIG_H
#define CONFIG_H

// Network
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASS "YOUR_PASSWORD"
#define API_URL "http://YOUR_SERVER_IP:8000/api/generate-temporal-narrative"

// Pins
#define PIN_SERVO_PAST 13
#define PIN_SERVO_PRESENT 12
#define PIN_SERVO_FUTURE 14

#define PIN_STEPPER_IN1 19
#define PIN_STEPPER_IN2 18
#define PIN_STEPPER_IN3 5
#define PIN_STEPPER_IN4 17

#define PIN_I2S_BCLK 26
#define PIN_I2S_LRC 25
#define PIN_I2S_DOUT 22

#define PIN_ENCODER_CLK 32
#define PIN_ENCODER_DT 33
#define PIN_ENCODER_SW 27

// Constants
#define STEPS_PER_REVOLUTION 2048 // For 28BYJ-48
#define WINDING_THRESHOLD 3 // Full turns
#define WINDING_TIMEOUT_MS 5000
#define IDLE_TIMEOUT_MS 2000

#endif
