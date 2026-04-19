# PRIMERA PARTE:
Test, 10 o 15 minutos, 3 preguntas, en papel, sobre lo que hemos hecho en prácticas
<br><br><br>

# SEGUNDA PARTE: EXPERIMENTO 💀:
## Práctica 1
- En sklearn, para qué sirve el parámetro `k` (`n_components`) en PCA?
- De qué va mnist (números), fashion_mnist (prendas de ropa), cifar-10 (imágenes de varias clases)? (No hay que saber la resolución de las imágenes)
- Saber cómo funciona PCA
- Diferencia entre los métodos `fit(X)`, `transform(X)`, `fit_transform(X)`, `inverse_transform(X)`
    - Si llamamos a `fit(X)`, luego tenemos que llamar a transform(X)
    - `fit_transform(X)` hace las 2 cosas
    - `inverse_transform(X)` pasa los datos pequeñitos de nuevo a la dimensión original
- Qué es el error de reconstrucción?
<br>

## Práctica 2
- Qué es:
    - `reg_param` (interpola la matriz de varianzas con la matriz identidad) (es un parámetro de regularización)
    - `tol` (tolerancia) en LDA suaviza varianzas y covarianzas
- Saber utilizar LDA y QDA
- Búsqueda de hiperparámetros (puede que haya que usarlo en el problema):
```
clf = LinearDiscriminantAnalysis(); G = {'tol': loguniform(1e-9, 1e-1)}
splitter = ShuffleSplit(n_splits=2, test_size=0.1, random_state=23)
RS = RandomizedSearchCV(clf, G, n_iter=10, scoring='accuracy', n_jobs=-1, cv=splitter, pre_dispatch=8)
```
<br>

## Práctica 3: Regresión logística
- `tol` (tolerancia) (umbral de error de error a partir del cual si bajamos de ahí, paramos el entrenamiento, porque consideramos que el modelo ya es suficientemente bueno)
- Ingeniería de características:
    - `degree` es el grado máximo de las características generadas
    - `interaction_only` (boolean): Si nos cargamos lo original para quedarnos con las polinomial features
<br>


## Posibles ejercicios:
### Para el clasificador QDA, haz una búsqueda de hiperparámetros para el parámetro reg_param (práctica 2):
- Forma 1:
![Imagen hiperparámetros QDA forma 1](https://i.imgur.com/rCtAv1b.png)

- Forma 2:
![Imagen hiperparámetros QDA forma 2](https://i.imgur.com/2Ov9fgK.png)

### En regresión logística, haz una búsqueda de hiperparámetros para el parámetro tol (práctica 3):
![Imagen hiperparámetros regresión logística](https://i.imgur.com/a7cQDqT.png)
