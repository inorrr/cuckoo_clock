#include "config.h"
#include "hal.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

enum State { IDLE, WINDING, CONNECTING, PERFORMANCE, RESET };

State currentState = IDLE;
unsigned long lastTickTime = 0;

void setup() {
  Serial.begin(115200);
  hal.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  // Non-blocking wifi connect for now
}

void loop() {
  hal.update();

  switch (currentState) {
  case IDLE:
    // Timekeeping logic
    if (millis() - lastTickTime > 60000) {
      hal.moveHands(STEPS_PER_REVOLUTION / 60); // Move 1 minute
      lastTickTime = millis();
    }

    if (hal.isWinding()) {
      currentState = WINDING;
    }
    break;

  case WINDING:
    // Check for winding completion
    if (!hal.isWinding()) { // Simplified check
      currentState = CONNECTING;
    }
    break;

  case CONNECTING:
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(API_URL);
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST("{}");

      if (httpResponseCode == 200) {
        String response = http.getString();

        // Allocate a large document.
        // Adjust size based on expected audio length.
        DynamicJsonDocument doc(60000);
        DeserializationError error = deserializeJson(doc, response);

        if (!error) {
          const char *presentAudio = doc["present_audio"];
          const char *pastAudio = doc["past_audio"];
          const char *futureAudio = doc["future_audio"];

          hal.saveAudioFile("/present.mp3", presentAudio);
          hal.saveAudioFile("/past.mp3", pastAudio);
          hal.saveAudioFile("/future.mp3", futureAudio);

          currentState = PERFORMANCE;
        } else {
          Serial.println("JSON Parse Error");
          currentState = IDLE;
        }
      } else {
        Serial.println("HTTP Error");
        currentState = IDLE;
      }
      http.end();
    }
    break;

  case PERFORMANCE:
    // Segment A: Present
    hal.openPresentBird();
    hal.playAudio("/present.mp3");
    while (hal.isAudioPlaying()) {
      hal.update();
    }
    hal.closePresentBird();
    delay(500);

    // Segment B: Past
    hal.moveHands(-1000); // Should be non-blocking or we wait?
    // Spec says "Hands rotate... Audio plays". Usually simultaneous?
    // "End: Left Bird closes." implies sequence.
    // Let's make hands move while audio plays?
    // But stepper.run() needs to be called.
    // hal.update() calls stepper.run().

    hal.openPastBird();
    hal.playAudio("/past.mp3");
    while (hal.isAudioPlaying() || hal.areHandsMoving()) {
      hal.update();
    }
    hal.closePastBird();
    delay(500);

    // Segment C: Future
    hal.moveHands(2000);
    hal.openFutureBird();
    hal.playAudio("/future.mp3");
    while (hal.isAudioPlaying() || hal.areHandsMoving()) {
      hal.update();
    }
    hal.closeFutureBird();

    currentState = RESET;
    break;

  case RESET:
    hal.stopHands();
    // Sync time logic would go here
    currentState = IDLE;
    break;
  }
}
