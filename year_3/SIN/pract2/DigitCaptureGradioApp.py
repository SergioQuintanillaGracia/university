import gradio as gr
import numpy as np
from PIL import Image, ImageOps

class DigitCaptureApp:
    def __init__(self):
        """Initialize the app with pre-trained weights."""
        self.demo = self.create_interface()
        self.X = np.array([])
        self.y = np.array([], dtype=np.int64)

    # -----------------------------
    # Utils
    # -----------------------------
    def status(self, msg):
        counts = np.unique(self.y, return_counts=True)
        counts = ', '.join([f"'{c}': {v}" for c,v in zip(counts[0],counts[1])])
        return f'{msg}\n\nSamples: ({counts})'

    def load_X_y(self, image_fn, label_fn, mode='loading'):
        if image_fn is None or image_fn == "":
            raise RuntimeError(f"Please provide a filename for the images before {mode} the dataset.")
        if label_fn is None or label_fn == "":
            raise RuntimeError(f"Please provide a filename for the labels before {mode} the dataset.")
        X = np.load(image_fn)
        if X.dtype!=np.float64 or len(X.shape)!=2 or X.shape[1]!=64 or X.shape[0]<=0:
            raise RuntimeError(f'{image_fn} is not a valid images file')
        y = np.load(label_fn)
        if len(y.shape)!=1 or (len(self.y)>0) and self.y.dtype != y.dtype:
            raise RuntimeError(f'{label_fn} is not a valid labels file')
        if y.shape[0]!=X.shape[0]:
            raise RuntimeError(f'mismatch between the number of labels and images')
        return X,y
    
    # -----------------------------
    # Image Preprocessing
    # -----------------------------
    def preprocess_image(self, image):
        """Preprocess the image according to NIST-style normalization."""

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

    # -----------------------------
    # Save image
    # -----------------------------
    def save_image(self, canvas, label):
        if isinstance(canvas, dict):
            # Extract the composite image from gr.Paint dict
            image = canvas.get("composite")
        else:
            image = canvas

        if image is None:
            return self.status("No image to save.")

        if label is None or label.strip() == "":
            return self.status("Please provide a class label before saving.")
        try:
            label = int(label)
        except Exception as e:
            return self.status("Please provide a numeric class label")
        if label < 0 or label > 9:
            return self.status("Please provide a numeric class number in range [0,9]")

        # Preprocess image
        x = self.preprocess_image(image)

        if self.X.size == 0:
            self.X = x
        else:
            self.X = np.vstack([self.X,x])
        self.y = np.append(self.y, label)

        return self.status(f"Saving image of class {label}")

    # -----------------------------
    # Manage dataset
    # -----------------------------
    def save_dataset(self, image_fn, label_fn):

        if image_fn is None or image_fn == "":
            return self.status("Please provide a filename for the images before saving the dataset.")

        if label_fn is None or label_fn == "":
            return self.status("Please provide a filename for the labels before saving the dataset.")

        np.save(image_fn,self.X)
        np.save(label_fn,self.y)
        return self.status(f"Saving images to {image_fn} and labels to {label_fn}")

    def load_dataset(self, image_fn, label_fn):
        try:
            X,y = self.load_X_y(image_fn, label_fn)
        except Exception as e:
            return self.status(f"Error: {e}")
        self.X,self.y = X,y
        return self.status(f"Loading images from {image_fn} and labels from {label_fn}")

    def merge_dataset(self, image_fn, label_fn):
        try:
            X,y = self.load_X_y(image_fn, label_fn, mode='merging')
        except Exception as e:
            return self.status(f"Error: {e}")
        if len(self.X) == 0:
            self.X,self.y = X,y
        else:
            self.X = np.concatenate((self.X, X), axis=0)
            self.y = np.concatenate((self.y, y), axis=0)
        return self.status(f"Merging images with {image_fn} and labels with {label_fn}")
    

    # -----------------------------
    # Gradio Interface
    # -----------------------------
    def create_interface(self):
        """Create and return the Gradio interface."""
        with gr.Blocks(theme=gr.themes.Default()) as demo:
            gr.Markdown("## Handwritten Digit Capture")

            with gr.Row():
                canvas = gr.Paint(
                    layers=False,
                    label="Draw a digit here",
                    width=200,
                    height=200,
                    brush=gr.Brush(default_color="black", default_size=8),
                    type="pil",
                    elem_id="my_canvas",
                )
                output = gr.Textbox(label="Status:",lines=3)
            
            with gr.Row():
                label_input = gr.Textbox(label="Enter class label (0-9)", placeholder="e.g., 5")
                save_image_btn = gr.Button("Save image")

            save_image_btn.click(fn=self.save_image, inputs=[canvas, label_input], outputs=output)

            with gr.Row():
                image_fn = gr.Textbox(label="Please provide a filename for saving/loading/merging the images", placeholder="e.g., images.npy")
                label_fn = gr.Textbox(label="Please provide a filename for saving/loading/merging the labels", placeholder="e.g., labels.npy")
                with gr.Column():
                    save_dataset_btn = gr.Button("Save dataset")
                    merge_dataset_btn = gr.Button("Merge dataset")
                    load_dataset_btn = gr.Button("Load dataset")

            save_dataset_btn.click(fn=self.save_dataset, inputs=[image_fn, label_fn], outputs=output)
            load_dataset_btn.click(fn=self.load_dataset, inputs=[image_fn, label_fn], outputs=output)
            merge_dataset_btn.click(fn=self.merge_dataset, inputs=[image_fn, label_fn], outputs=output)

        return demo

    def launch(self, **args):
        """Launch the Gradio app."""
        self.demo.launch(*args)