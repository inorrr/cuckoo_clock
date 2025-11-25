from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import os
from dotenv import load_dotenv
from services import generate_narrative, synthesize_audio

load_dotenv()

app = FastAPI()

class NarrativeResponse(BaseModel):
    present_audio: str
    past_audio: str
    future_audio: str

@app.post("/api/generate-temporal-narrative", response_model=NarrativeResponse)
async def generate_temporal_narrative():
    try:
        # Step 1 & 2: Generate Narrative
        narrative = await generate_narrative()
        
        # Step 3: Synthesize Audio
        audio_files = await synthesize_audio(narrative)
        
        return NarrativeResponse(
            present_audio=audio_files["present"],
            past_audio=audio_files["past"],
            future_audio=audio_files["future"]
        )
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
