from typing import Dict, Any, List

from google.adk.agents import LlmAgent
from google.adk.models.lite_llm import LiteLlm
from google.adk.tools import FunctionTool
from google.adk.tools.tool_context import ToolContext

import random

QUESTION_BANK: List[Dict[str, Any]] = [
    {
        "question": "¿Qué es un agente racional?",
        "answer": "un agente que maximiza su utilidad esperada"
    },
    {
        "question": "¿Qué es un entorno parcialmente observable?",
        "answer": "cuando el agente no puede ver el estado completo"
    },
    {
        "question": "¿Qué es la arquitectura BDI?",
        "answer": "beliefs desires intentions"
    },
    {
        "question": "¿Qué representa la utilidad en agentes?",
        "answer": "la preferencia del agente por los estados"
    },
    {
        "question": "¿Qué es una política en agentes?",
        "answer": "una función que asigna acciones a estados"
    },
    {
        "question": "¿Qué caracteriza un sistema multiagente?",
        "answer": "la interacción entre múltiples agentes"
    },
    {
        "question": "¿Qué es cooperación en agentes?",
        "answer": "agentes trabajando juntos para un objetivo común"
    },
    {
        "question": "¿Qué es negociación entre agentes?",
        "answer": "proceso de acuerdos mutuamente aceptables"
    },
    {
        "question": "¿Qué es un entorno determinista?",
        "answer": "cuando las acciones producen resultados predecibles"
    },
    {
        "question": "¿Qué es ACL en agentes?",
        "answer": "agent communication language"
    },
]


QUIZ: List[Dict[str, Any]] = []

def start_quiz(tool_context: ToolContext) -> Dict[str, Any]:
    state = tool_context.state

    # Seleccionar 5 preguntas aleatorias del banco
    global QUIZ
    QUIZ = random.sample(QUESTION_BANK, 5)

    state["quiz_started"] = True
    state["current_question_index"] = 0
    state["correct_answers"] = 0
    state["total_answered"] = 0
    state["finished"] = False

    if not QUIZ:
        return {"status": "error", "message": "No hay preguntas disponibles."}

    q = QUIZ[0]["question"]

    return {
        "status": "started",
        "question": q,
        "question_number": 1,
        "total_questions": len(QUIZ)
    }


def submit_answer(tool_context: ToolContext, answer: str) -> Dict[str, Any]:
    state = tool_context.state

    if not state.get("quiz_started"):
        return {"status": "error", "message": "El cuestionario aún no ha empezado. Usa start_quiz primero."}

    i = state.get("current_question_index", 0)
    if i >= len(QUIZ):
        state["finished"] = True
        return {"status": "finished", "message": "El cuestionario ya ha terminado."}

    correct_answer = QUIZ[i]["answer"]
    user = (answer or "").strip().lower()
    correct = (correct_answer or "").strip().lower()
    is_correct = user == correct

    state["total_answered"] = state.get("total_answered", 0) + 1
    if is_correct:
        state["correct_answers"] = state.get("correct_answers", 0) + 1

    i += 1
    state["current_question_index"] = i

    if i >= len(QUIZ):
        state["finished"] = True
        total = state["total_answered"]
        corrects = state["correct_answers"]
        score = round(100 * corrects / total, 2) if total > 0 else 0.0
        state["score_percentage"] = score
        return {
            "status": "finished",
            "is_correct": is_correct,
            "correct_answer": correct_answer,
            "total_answered": total,
            "correct_answers": corrects,
            "score_percentage": score,
            "message": "Cuestionario completado."
        }
    else:
        next_q = QUIZ[i]["question"]
        return {
            "status": "next",
            "is_correct": is_correct,
            "correct_answer": correct_answer,
            "next_question": next_q,
            "question_number": i + 1,
            "total_questions": len(QUIZ)
        }


def get_quiz_status(tool_context: ToolContext) -> Dict[str, Any]:
    state = tool_context.state
    return {
        "quiz_started": bool(state.get("quiz_started", False)),
        "current_question_index": state.get("current_question_index", 0),
        "correct_answers": state.get("correct_answers", 0),
        "total_answered": state.get("total_answered", 0),
        "finished": bool(state.get("finished", False)),
        "score_percentage": state.get("score_percentage", 0.0)
    }


def reset_quiz(tool_context: ToolContext) -> Dict[str, Any]:
    state = tool_context.state
    state["quiz_started"] = False
    state["current_question_index"] = 0
    state["correct_answers"] = 0
    state["total_answered"] = 0
    state["finished"] = False
    state["score_percentage"] = 0.0
    return {"status": "reset", "message": "El cuestionario se ha reiniciado."}


start_quiz_tool = FunctionTool(start_quiz)
submit_answer_tool = FunctionTool(submit_answer)
get_quiz_status_tool = FunctionTool(get_quiz_status)
reset_quiz_tool = FunctionTool(reset_quiz)


QUIZ_INSTRUCTIONS = """
ERES UN AGENTE QUE GESTIONA UN CUESTIONARIO DE PREGUNTAS.
TU FUNCIONAMIENTO DEPENDE DE HERRAMIENTAS. SIGUE ESTAS REGLAS:

REGLAS CRÍTICAS:
1. NUNCA formules preguntas por tu cuenta.
2. SOLO puedes presentar una pregunta al usuario si antes llamaste a start_quiz() o submit_answer().
3. CUANDO el usuario diga algo relacionado con:
   - "quiero hacer un test"
   - "quiero empezar"
   - "empecemos el test"
   → DEBES LLAMAR INMEDIATAMENTE a start_quiz().

4. CUANDO el usuario conteste una pregunta:
   → DEBES llamar a submit_answer(answer="[texto del usuario]").

5. CUANDO submit_answer devuelva "status": "finished":
   → EXPLICA el resultado final usando los datos devueltos.
   → NO sigas preguntando.

6. SI el usuario pregunta por el progreso:
   → llama a get_quiz_status().

7. SI el usuario quiere reiniciar:
   → llama a reset_quiz() y luego start_quiz().

FORMATO DE LLAMADA A HERRAMIENTAS:
- Siempre llama a herramientas usando el formato JSON que ADK espera.
- NO respondas con texto normal si debes llamar a una herramienta.

NO IMPROVISES EL FLUJO. El cuestionario SIEMPRE sigue:
start_quiz() → submit_answer() → submit_answer() ... → resultado final.
"""


root_agent = LlmAgent(
    name="quiz_agent",
    model=LiteLlm(
        model="openai/gpt-oss-120b",
        api_base="https://api.poligpt.upv.es/",
        api_key="sk-LFXs1kjaSxtEDgOMlPUOpA"
    ),
    instruction=QUIZ_INSTRUCTIONS,
    tools=[start_quiz_tool, submit_answer_tool, get_quiz_status_tool, reset_quiz_tool]
)