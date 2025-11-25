import os
import requests
import google.generativeai as genai
from openai import OpenAI
import base64
import asyncio

# Initialize clients
genai.configure(api_key=os.getenv("GEMINI_API_KEY"))
openai_client = OpenAI(api_key=os.getenv("OPENAI_API_KEY"))

async def get_news_headline():
    api_key = os.getenv("NEWS_API_KEY")
    url = f"https://newsapi.org/v2/top-headlines?country=us&category=science&apiKey={api_key}"
    
    # Run synchronous request in thread pool
    response = await asyncio.to_thread(requests.get, url)
    data = response.json()
    
    if data.get("status") == "ok" and data.get("articles"):
        return data["articles"][0]["title"]
    return "Time passes, but some things remain constant."

async def generate_narrative():
    headline = await get_news_headline()
    
    model = genai.GenerativeModel('gemini-pro')
    
    prompt = f"""
    Role: "Spirit of the Clock" / Historian.
    Input: "{headline}"
    
    Directives:
    1. Present: Summarize the headline factually (1 sentence).
    2. Past: "Hallucinate" a historically accurate analogy from >20 years ago. Must have a resolved, positive outcome. (1 sentence).
    3. Future: Generate a forecast for the current headline that parallels the positive resolution of the past event. Tone: Optimistic, cyclical. (1 sentence).
    
    Output JSON format:
    {{
        "present": "...",
        "past": "...",
        "future": "..."
    }}
    """
    
    response = await asyncio.to_thread(model.generate_content, prompt)
    # Basic cleanup to ensure valid JSON if the model includes markdown blocks
    text = response.text.replace('```json', '').replace('```', '').strip()
    import json
    return json.loads(text)

async def synthesize_single_audio(text, voice):
    response = await asyncio.to_thread(
        openai_client.audio.speech.create,
        model="tts-1",
        voice=voice,
        input=text
    )
    return base64.b64encode(response.content).decode('utf-8')

async def synthesize_audio(narrative):
    # Parallel execution
    results = await asyncio.gather(
        synthesize_single_audio(narrative["present"], "alloy"),
        synthesize_single_audio(narrative["past"], "onyx"),
        synthesize_single_audio(narrative["future"], "nova")
    )
    
    return {
        "present": results[0],
        "past": results[1],
        "future": results[2]
    }
