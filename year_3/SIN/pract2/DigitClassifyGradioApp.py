import gradio as gr
import numpy as np
from PIL import ImageOps

# Simulating preprocessing (except for slant normalization) according to
# NIST Form-Based Handprint Recognition System
# https://tsapps.nist.gov/publication/get_pdf.cfm?pub_id=900721
def preprocess_image(image):

    # Convert to grayscale
    image = image.convert("L")

    # Invert grayscale (white background -> black digit)
    inverted_image = ImageOps.invert(image)
    bbox = inverted_image.getbbox()
    if bbox is None:
        return None

    cropped = inverted_image.crop(bbox)
    resized = cropped.resize((32, 32))
    x = np.array(resized, dtype=np.uint8)
    x = np.where(x > 128, 1, 0)

    xb = np.zeros((8, 8))
    for i in range(8):
        for j in range(8):
            xb[i, j] = np.sum(x[i*4:(i+1)*4, j*4:(j+1)*4])

    return xb.reshape(1, -1) / 16.0

def classify(canvas, W, classifier):

    if isinstance(canvas, dict):
        image = canvas.get("composite")
    else:
        image = canvas

    if image is None:
        return None

    """Classify the drawn digit using your trained weights."""
    x = preprocess_image(image)
    if x is None:
        return "No content to classify."

    haty = classifier(x, W)
    return f"Predicted digit: {haty}"


def create_interface(W, classifier):
    """Creates and launches the Gradio interface (Gradio 5.x compatible)."""
    def classify_wrapper(image):
        return classify(image, W, classifier)

    with gr.Blocks() as demo:
        gr.Markdown("## Handwritten Digit Classification")

        with gr.Row():
            # Use gr.Paint for drawing in Gradio 5.x+
            canvas = gr.Paint(
                label="Draw a digit here",
                width=200,
                height=200,
                brush=gr.Brush(default_color="black", default_size=8),
                type="pil",
            )

            output = gr.Textbox(label="Classification result")

        classify_btn = gr.Button("Classify image")
        classify_btn.click(fn=classify_wrapper, inputs=canvas, outputs=output)

    return demo
