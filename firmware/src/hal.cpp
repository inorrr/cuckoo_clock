#include "hal.h"
#include "config.h"

HAL hal;

void HAL::begin() {
  // Servos
  servoPast.attach(PIN_SERVO_PAST);
  servoPresent.attach(PIN_SERVO_PRESENT);
  servoFuture.attach(PIN_SERVO_FUTURE);

  closePastBird();
  closePresentBird();
  closeFutureBird();

  // Stepper
  stepper = AccelStepper(AccelStepper::HALF4WIRE, PIN_STEPPER_IN1,
                         PIN_STEPPER_IN3, PIN_STEPPER_IN2, PIN_STEPPER_IN4);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);

  // Audio
  audio.setPinout(PIN_I2S_BCLK, PIN_I2S_LRC, PIN_I2S_DOUT);
  audio.setVolume(21); // 0-21
  SPIFFS.begin(true);

  // Encoder
  pinMode(PIN_ENCODER_CLK, INPUT);
  pinMode(PIN_ENCODER_DT, INPUT);
  pinMode(PIN_ENCODER_SW, INPUT_PULLUP);
}

void HAL::update() {
  stepper.run();
  audio.loop();
  updateEncoder();
  // Encoder logic would go here (interrupt based usually better but polling for
  // simplicity)
}

void HAL::openPastBird() { servoPast.write(90); }
void HAL::closePastBird() { servoPast.write(0); }
void HAL::openPresentBird() { servoPresent.write(90); }
void HAL::closePresentBird() { servoPresent.write(0); }
void HAL::openFutureBird() { servoFuture.write(90); }
void HAL::closeFutureBird() { servoFuture.write(0); }

void HAL::moveHands(long steps) { stepper.move(steps); }

void HAL::setHandsSpeed(float speed) { stepper.setMaxSpeed(speed); }

void HAL::stopHands() { stepper.stop(); }

bool HAL::areHandsMoving() { return stepper.isRunning(); }

void HAL::playAudio(const char *filename) {
  audio.connecttoFS(SPIFFS, filename);
}

bool HAL::saveAudioFile(const char *filename, const char *base64Data) {
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    return false;
  }

  size_t len = strlen(base64Data);
  size_t dlen = 0;
  size_t olen = 0;

  // Calculate output length
  mbedtls_base64_decode(NULL, 0, &dlen, (const unsigned char *)base64Data, len);

  unsigned char *out = (unsigned char *)malloc(dlen);
  if (!out) {
    file.close();
    return false;
  }

  int ret = mbedtls_base64_decode(out, dlen, &olen,
                                  (const unsigned char *)base64Data, len);
  if (ret == 0) {
    file.write(out, olen);
  }

  free(out);
  file.close();
  return ret == 0;
}

void HAL::stopAudio() { audio.stopSong(); }

bool HAL::isAudioPlaying() { return audio.isRunning(); }

int HAL::getEncoderRotation() { return encoderPosition; }

bool HAL::isWinding() {
  // Check if encoder is rotating fast enough
  // This is a simple implementation.
  // In reality, we'd check the rate of change of encoderPosition.
  // For this prototype, let's assume we track "winding" state based on recent
  // activity.
  return (millis() - lastWindingTime < WINDING_TIMEOUT_MS) &&
         (abs(encoderPosition) >
          WINDING_THRESHOLD * 20); // 20 clicks per turn approx
}

// Call this in update()
void HAL::updateEncoder() {
  static int lastClk = HIGH;
  int newClk = digitalRead(PIN_ENCODER_CLK);
  if (newClk != lastClk) {
    if (digitalRead(PIN_ENCODER_DT) != newClk) {
      encoderPosition++;
    } else {
      encoderPosition--;
    }
    lastWindingTime = millis();
  }
  lastClk = newClk;
}
