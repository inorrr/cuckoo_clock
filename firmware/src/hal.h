#ifndef HAL_H
#define HAL_H

#include "Audio.h"
#include <AccelStepper.h>
#include <Arduino.h>
#include <ESP32Servo.h>
#include <FS.h>
#include <SPIFFS.h>
#include <mbedtls/base64.h>

class HAL {
public:
  void begin();
  void update();

  // Servos
  void openPastBird();
  void closePastBird();
  void openPresentBird();
  void closePresentBird();
  void openFutureBird();
  void closeFutureBird();

  // Stepper
  void moveHands(long steps);
  void setHandsSpeed(float speed);
  void stopHands();
  bool areHandsMoving();

  // Audio
  void playAudio(const char *base64Audio);
  void stopAudio();
  bool isAudioPlaying();
  bool saveAudioFile(const char *filename, const char *base64Data);

  // Encoder
  int getEncoderRotation();
  bool isWinding();

private:
  Servo servoPast;
  Servo servoPresent;
  Servo servoFuture;
  AccelStepper stepper;
  Audio audio;

  long encoderPosition;
  unsigned long lastWindingTime;
  void updateEncoder();
};

extern HAL hal;

#endif
