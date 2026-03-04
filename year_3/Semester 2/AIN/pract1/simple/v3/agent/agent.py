import os
import requests
import datetime
import json
import pandas as pd
import feedparser

from typing import List, Dict

from google.adk.agents import Agent
from google.adk.models.lite_llm import LiteLlm




# ------------------------
# TOOLS (funciones Python)
# ------------------------

def fetch_news(categories: List[str]) -> Dict:
    """
    Recupera noticias desde Google News RSS para varias categorías.
    Devuelve news_by_category con title, content (summary) y url.
    """

    results = {}

    for cat in categories:
        q = cat.replace(" ", "+")
        rss_url = (
            f"https://news.google.com/rss/search?q={q}&hl=en&gl=US&ceid=US:en"
        )

        feed = feedparser.parse(rss_url)
        articles = []

        for entry in feed.entries[:10]:  # max 10 artículos
            articles.append({
                "title": entry.title,
                "content": entry.summary if "summary" in entry else "",
                "url": entry.link
            })

        results[cat] = articles

    return {"status": "success", "news_by_category": results}



def summarize_article(title: str, content: str, max_sentences: int = 3) -> Dict:
    """
    Esta tool podría delegar la parte 'inteligente' al LLM,
    pero aquí lo dejamos como stub simple.
    """
    # En un mundo real, podrías guardar el artículo en estado y dejar
    # que el LLM lo resuma; aquí devolvemos un "resumen" de juguete.
    summary = f"(Resumen corto de '{title}' limitado a {max_sentences} frases)."
    return {"status": "success", "summary": summary}


def format_bulletin(date: str, items_json: str) -> Dict:
    """
    items_json debe ser un JSON string con una lista de items:
        [{"title": "...", "summary": "...", "url": "..."}]
    """
    items = json.loads(items_json)

    lines = [f"# Boletín de noticias – {date}", ""]

    for i, item in enumerate(items, start=1):
        lines.append(f"## {i}. {item['title']}")
        lines.append("")
        lines.append(item["summary"])
        if item.get("url"):
            lines.append("")
            lines.append(f"[Leer más]({item['url']})")
        lines.append("")

    return {"status": "success", "bulletin_md": "\n".join(lines)}


# ------------------------
# ROOT AGENT (multi-tool)
# ------------------------

root_agent = Agent(
    name="news_bulletin_agent",
    #model="gemini-2.0-flash",
    model=LiteLlm(model="openai/gpt-oss-120b", 
        api_base="https://api.poligpt.upv.es/", 
        api_key="sk-LFXs1kjaSxtEDgOMlPUOpA"),
    description=(
        "Agente que genera un boletín de noticias personalizado "
        "según los intereses del usuario."
    ),
    instruction=(
        "Eres un asistente que genera un boletín diario de noticias.\n"
        "- Usa obligatoriamente fetch_news para obtener noticias (tecnología, deportes, política, etc.).\n"
        "- Usa obligatoriamente summarize_article para crear resúmenes concisos.\n"
        "- Usa obligatoriamenteformat_bulletin para construir el boletín final en Markdown.\n"
        "Pide al usuario sus intereses si no están claros."
    ),
    tools=[fetch_news, summarize_article, format_bulletin],
)
