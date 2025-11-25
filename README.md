# Temporal Cuckoo Clock

A physical clock that tells the time and performs a temporal narrative based on current news headlines.

## Architecture

- **Server (Cloud Brain)**: Python FastAPI application.
  - Fetches news from NewsAPI.
  - Generates narrative using Google Gemini.
  - Synthesizes audio using OpenAI TTS.
  - Returns Base64 encoded audio for Present, Past, and Future.

- **Client (Physical Clock)**: ESP32 Firmware.
  - Controls Servos (Birds) and Stepper Motor (Hands).
  - Plays audio via I2S.
  - Connects to Wi-Fi and queries the server.

## Setup

### Server

1. Navigate to `server/`:
   ```bash
   cd server
   ```
2. Install dependencies:
   ```bash
   pip install -r requirements.txt
   ```
3. Set environment variables in `.env` (create this file):
   ```
   NEWS_API_KEY=your_key
   GEMINI_API_KEY=your_key
   OPENAI_API_KEY=your_key
   ```
4. Run the server:
   ```bash
   python main.py
   ```

### Firmware

1. Navigate to `firmware/`:
   ```bash
   cd firmware
   ```
2. Update `src/config.h` with your Wi-Fi credentials and Server IP.
3. Build and Upload using PlatformIO:
   ```bash
   pio run -t upload
   ```

## Usage

1. Power on the clock.
2. Wind the clock using the rotary encoder (3 full turns).
3. The clock will connect to the server and perform the sequence.