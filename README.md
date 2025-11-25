# cuckoo_clock

## Architecture:
1. Presentation/Control Layer (Hardware Interface): input handler(clock winding), actuator control, hardware interface module.
2. Business Logic Layer: story orchestrator(main loop), temporal narrator(logic for time-shifting)
3. Data/Service Layer: current newws fetcher, GenAI news engine, Text-to-Speech

```
carol_clock_app/
├── main.py
├── config.py
|
├── business_logic/
│   ├── __init__.py
│   ├── orchestrator.py
│   └── temporal_narrator.py
|
├── data_services/
│   ├── __init__.py
│   ├── news_fetcher.py
│   ├── genai_engine.py
│   └── tts_service.py
|
└── hardware_control/
    ├── __init__.py
    ├── input_handler.py
    └── actuator_control.py
```

## Flow
1. Winding Triggered: The Input Handler sends a "Start Story" signal to the Story Orchestrator.
2. Gathering Content: The Story Orchestrator calls the Current News Fetcher to get a recent headline, then passes it to the GenAI News Engine to receive the three complete scripts (Past, Present, Future). It also sends the scripts to the TTS Service to get the audio files.
3. Past Narration: The Temporal Narrator tells the Actuator Control to: Move the hands backward, Deploy Bird 1. Play the Past audio file.
4. Present Narration: The Temporal Narrator tells the Actuator Control to: Pause the hands. Deploy Bird 2 (or switch focus to Bird 2). Play the Present audio file.
5. Future Narration: The Temporal Narrator tells the Actuator Control to: Move the hands forward (spin). Deploy Bird 3. Play the Future audio file.
6. Reset: The Temporal Narrator tells the Actuator Control to: Move the hands back to the current time. Retract all birds.


## Technology
1. Language: Python?
2. GenAI: google-genai?
3. News API: ?
