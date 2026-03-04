from google.adk.agents import LlmAgent
from google.adk.models.lite_llm import LiteLlm  

from typing import List, Dict



# ------------------------
# ROOT AGENT (multi-tool)
# ------------------------

root_agent = LlmAgent(
    name="simple",
    #model="gemini-2.0-flash",
    model=LiteLlm(model="openai/gpt-oss-120b", api_base="https://api.poligpt.upv.es/", api_key="sk-LFXs1kjaSxtEDgOMlPUOpA"),
    description="Agente que contesta lo que pueda",
    instruction=(
        "Eres el Agente Simple.\n"
        "Tu tarea es contestar lo que puedas a lo que te llegue."
    ),
)
