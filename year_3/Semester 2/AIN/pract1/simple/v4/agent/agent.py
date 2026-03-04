from google.adk.agents import LlmAgent
from google.adk.agents.callback_context import CallbackContext
from google.adk.models.lite_llm import LiteLlm
from google.adk.models import LlmRequest, LlmResponse
import re
from typing import Optional


PALABRAS_PROHIBIDAS = ["tonto", "idiota", "imbécil", "estúpido"]


def filtrar_texto(texto: str) -> str:
    for palabra in PALABRAS_PROHIBIDAS:
        patron = r"\b" + re.escape(palabra) + r"\b"
        texto = re.sub(patron, "estupendo", texto, flags=re.IGNORECASE)
    return texto


def before_callback(
    callback_context: CallbackContext,
    llm_request: LlmRequest,
) -> Optional[LlmResponse]:
    # Filtra SOLO los contenidos del usuario (entrada)
    if not llm_request.contents:
        return None

    for content in llm_request.contents:
        if getattr(content, "role", None) != "user":
            continue

        parts = getattr(content, "parts", None) or []
        for part in parts:
            # Solo toca partes de texto (si hay imágenes/otros, se dejan intactos)
            if hasattr(part, "text") and part.text:
                part.text = filtrar_texto(part.text)

    return None


root_agent = LlmAgent(
    name="simple",  
    model=LiteLlm(
        model="openai/gpt-oss-120b",
        api_base="https://api.poligpt.upv.es/",
        api_key="sk-LFXs1kjaSxtEDgOMlPUOpA"
    ),
    description="Agente que responde normal, pero filtra malsonantes en la pregunta del usuario.",
    instruction="Responde en español de forma breve y útil.",
    before_model_callback=before_callback
)