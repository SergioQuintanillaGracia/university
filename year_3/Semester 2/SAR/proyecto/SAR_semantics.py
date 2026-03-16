# Versión 1.1

from typing import List, Tuple
import numpy as np
import spacy
import torch
from transformers import AutoTokenizer, AutoModel
from sentence_transformers import SentenceTransformer
from sklearn.neighbors import KDTree

BATCH_SIZE = 16

class EmbeddingModel:
    def __init__(self) -> None:
        self.embeddings = None
        self.kdtree = None

    def fit(self, texts: List[str]) -> None:
        """Genera embeddings y construye el KDTree."""
        self.embeddings = self.get_embeddings(texts)
        self.kdtree = KDTree(self.embeddings, metric="euclidean")

    def get_embeddings(self, texts: List[str]) -> np.ndarray:
        """Método abstracto para obtener embeddings."""
        raise NotImplementedError("Este método debe ser implementado por las subclases.")

    def query(self, query: str, top_k: int = 5) -> List[Tuple[float, int]]:
        """Consulta y recupera las frases más cercanas."""
        query_embedding = self.get_embeddings([query])#.reshape(1, -1)
        distances, indices = self.kdtree.query(query_embedding, k=top_k)
        distances, indices = distances[0].tolist(), indices[0].tolist()
        return [(dist, ind) for dist, ind in zip(distances, indices)]

    def set_kdtree(self, kdtree: KDTree) -> None:
        """Cambia el valor del atributo kdtree."""
        self.kdtree = kdtree

    def set_embeddings(self, embeddings: np.array) -> None:
        """Cambia el valor del atributo embeddings."""
        self.embeddings = embeddings


class SpacyStaticModel(EmbeddingModel):
    def __init__(self, remove_stopwords: bool=True, remove_noalpha: bool=True) -> None:
        super().__init__()
        self.nlp = spacy.load("es_core_news_lg", disable=["parser", "tagger", "ner", "lemmatizer", "attribute_ruler"])
        self.vector_length = self.nlp.vocab.vectors.shape[1]
        self.remove_SW = remove_stopwords
        self.remove_noalpha = remove_noalpha

    def get_embeddings(self, texts: List[str]) -> np.ndarray:
        vectors = []
        for text in texts:
            tokens = [tk for tk in self.nlp(text)]
            if self.remove_SW:
                tokens = [tk for tk in tokens if not tk.is_stop]
            if self.remove_noalpha:
                tokens = [tk for tk in tokens if tk.is_alpha]
            if len(tokens) == 0:
                v = np.zeros(self.vector_length)
            else:
                v = np.mean([tk.vector for tk in tokens], axis=0)
            vectors.append(v)
        return np.array(vectors)


class BetoEmbeddingModel(EmbeddingModel):
    def __init__(self) -> None:
        super().__init__()
        if torch.cuda.is_available():
            self.device = torch.device("cuda")
        elif torch.backends.mps.is_available():
            self.device = torch.device("mps")
        else:
            self.device = "cpu"        
        self.tokenizer = AutoTokenizer.from_pretrained("dccuchile/bert-base-spanish-wwm-uncased")
        self.model = AutoModel.from_pretrained("dccuchile/bert-base-spanish-wwm-uncased", ignore_mismatched_sizes=True).to(self.device)
        self.model.eval()  # Desactiva dropout

    def get_embeddings(self, texts: List[str], batch_size: int=BATCH_SIZE) -> np.ndarray:
        embeddings = []
        #print(len(texts))
        #print(self.model.config.max_position_embeddings)
        for i in range(0, len(texts), batch_size):
            batch_texts = texts[i:i + batch_size]
            inputs = self.tokenizer(
                batch_texts,
                return_tensors="pt",
                truncation=True,
                padding=True,
                max_length=self.model.config.max_position_embeddings
            ).to(self.device)
            with torch.no_grad():
                outputs = self.model.forward(**inputs)
            embeddings.append(outputs.last_hidden_state.mean(dim=1).squeeze().cpu().numpy())
            del inputs, outputs
            torch.cuda.empty_cache()
        return np.vstack(embeddings)




class BetoEmbeddingCLSModel(EmbeddingModel):
    def __init__(self) -> None:
        super().__init__()
        if torch.cuda.is_available():
            self.device = torch.device("cuda")
        elif torch.backends.mps.is_available():
            self.device = torch.device("mps")
        else:
            self.device = "cpu"        
        self.tokenizer = AutoTokenizer.from_pretrained("dccuchile/bert-base-spanish-wwm-uncased")
        self.model = AutoModel.from_pretrained("dccuchile/bert-base-spanish-wwm-uncased", ignore_mismatched_sizes=True).to(self.device)
        self.model.eval()  # Desactiva dropout


    def get_embeddings(self, texts: List[str], batch_size: int=BATCH_SIZE) -> np.ndarray:
        embeddings = []
        for i in range(0, len(texts), batch_size):
            batch_texts = texts[i:i + batch_size]
            inputs = self.tokenizer(
                batch_texts,
                return_tensors="pt",
                truncation=True,
                padding=True,
                max_length=self.model.config.max_position_embeddings
            ).to(self.device)
            with torch.no_grad():
                outputs = self.model.forward(**inputs)
            #embeddings.extend(outputs.last_hidden_state[:,0,:].squeeze().cpu().numpy())
            embeddings.append(outputs.last_hidden_state[:,0,:].squeeze().cpu().numpy())
            del inputs, outputs
            torch.cuda.empty_cache()
        return np.vstack(embeddings)
                
class SentenceBertEmbeddingModel(EmbeddingModel):
    def __init__(self) -> None:
        super().__init__()
        self.model = SentenceTransformer("hiiamsid/sentence_similarity_spanish_es")

    def get_embeddings(self, texts: List[str]) -> np.ndarray:
        return self.model.encode(texts)
