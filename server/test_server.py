import pytest
from fastapi.testclient import TestClient
from unittest.mock import patch, AsyncMock
import os
os.environ["OPENAI_API_KEY"] = "dummy"
os.environ["GEMINI_API_KEY"] = "dummy"
os.environ["NEWS_API_KEY"] = "dummy"

from main import app

client = TestClient(app)

@pytest.mark.asyncio
async def test_generate_temporal_narrative():
    # Mock the services
    with patch('main.generate_narrative', new_callable=AsyncMock) as mock_gen_narrative, \
         patch('main.synthesize_audio', new_callable=AsyncMock) as mock_synth_audio:
        
        # Setup mock returns
        mock_gen_narrative.return_value = {
            "present": "Present text",
            "past": "Past text",
            "future": "Future text"
        }
        
        mock_synth_audio.return_value = {
            "present": "base64_present",
            "past": "base64_past",
            "future": "base64_future"
        }
        
        response = client.post("/api/generate-temporal-narrative")
        
        assert response.status_code == 200
        data = response.json()
        
        assert data["present_audio"] == "base64_present"
        assert data["past_audio"] == "base64_past"
        assert data["future_audio"] == "base64_future"
        
        mock_gen_narrative.assert_called_once()
        mock_synth_audio.assert_called_once()
