
from __future__ import annotations
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Tuple, Optional
import csv

try:
    # Newer docs often show this path.
    from google.adk.agents.llm_agent import Agent  # type: ignore
except Exception:  # pragma: no cover
    from google.adk.agents import Agent  # type: ignore


from google.adk.models.lite_llm import LiteLlm 


@dataclass(frozen=True)
class WeatherRow:
    temp_c: int
    rain_prob: int
    summary: str

class CsvDataStore:
    """
    Carga y cachea weather.csv y places.csv.
    Weather ahora se indexa SOLO por ciudad (sin fecha).
    """
    def __init__(self, base_dir: Path):
        self.base_dir = base_dir
        self._weather: Optional[Dict[str, WeatherRow]] = None
        self._places: Optional[Dict[Tuple[str, str], List[Dict[str, Any]]]] = None

    def _load_weather(self) -> Dict[str, WeatherRow]:
        path = self.base_dir / "weather.csv"
        db: Dict[str, WeatherRow] = {}
        with path.open(newline="", encoding="utf-8") as f:
            reader = csv.DictReader(f)
            for r in reader:
                city = (r["city"] or "").strip()
                db[city] = WeatherRow(
                    temp_c=int(r["temp_c"]),
                    rain_prob=int(r["rain_prob"]),
                    summary=(r["summary"] or "").strip(),
                )
        return db

    def _load_places(self) -> Dict[Tuple[str, str], List[Dict[str, Any]]]:
        path = self.base_dir / "places.csv"
        db: Dict[Tuple[str, str], List[Dict[str, Any]]] = {}
        with path.open(newline="", encoding="utf-8") as f:
            reader = csv.DictReader(f)
            for r in reader:
                city = (r["city"] or "").strip()
                category = (r["category"] or "").strip()
                item = {
                    "name": (r["name"] or "").strip(),
                    "type": category,
                    "price_eur": int(r["price_eur"]),
                }
                db.setdefault((city, category), []).append(item)
        return db

    def get_weather(self, city: str) -> WeatherRow:
        if self._weather is None:
            self._weather = self._load_weather()
        return self._weather.get(
            city,
            WeatherRow(temp_c=15, rain_prob=30, summary="Variable"),  # default
        )

    def search_places(self, city: str, category: str) -> List[Dict[str, Any]]:
        if self._places is None:
            self._places = self._load_places()
        return self._places.get((city, category), [])

# --- instancia global (base_dir = carpeta del paquete del agente) ---
DATA = CsvDataStore(base_dir=Path(__file__).resolve().parent)

# --- Tools ---
def get_weather(city: str) -> Dict[str, Any]:
    w = DATA.get_weather(city)
    return {"temp_c": w.temp_c, "rain_prob": w.rain_prob, "summary": w.summary}

def search_places(city: str, category: str) -> Dict[str, Any]:
    return {"result": DATA.search_places(city, category)}

def estimate_cost(place_names: List[str]) -> Dict[str, Any]:
    total = 0
    # buscamos precios en cache de places
    if DATA._places is None:
        DATA._places = DATA._load_places()

    price_index: Dict[str, int] = {}
    for (_city, _cat), items in DATA._places.items():
        for it in items:
            price_index[it["name"]] = int(it["price_eur"])

    for name in place_names:
        total += int(price_index.get(name, 0))

    return {"total_eur": total}





# ---------------------------
# Root agent
# ---------------------------

root_agent = Agent(
    # Pick a model you have configured (Gemini via GOOGLE_API_KEY, or via Vertex).
    # You can also route via LiteLLM if your environment is set up that way.
    #model="gemini-3-flash-preview",
    model=LiteLlm(model="openai/gpt-oss-120b", api_base="https://api.poligpt.upv.es/", api_key="sk-LFXs1kjaSxtEDgOMlPUOpA"),

    name="root_agent",
    description=(
        "Planifica una salida sencilla (2 opciones) en función del tiempo, preferencia y presupuesto, "
        "usando tools para fundamentar la respuesta."
    ),
    instruction=(
        "Eres un planificador de salidas. SIEMPRE debes fundamentarte en herramientas.\n"
        "Pasos: (1) llama a get_weather(city).\n"
        "(2) decide la categoría (SOLO entre estas: museo, outdoor, ocio): en funcion del tiempo que haga, por ejemplo si llueve mejor ir a un museo\n"
        "(3) llama a search_places(city, category) y elige hasta 2 lugares.\n"
        "(4) llama a estimate_cost con los nombres seleccionados.\n"
        "(5) responde en español con: categoría elegida, tiempo (resumen), 2 planes y coste total, "
        "sin inventar lugares fuera de la tool."
    ),
    tools=[get_weather, search_places, estimate_cost],
)
