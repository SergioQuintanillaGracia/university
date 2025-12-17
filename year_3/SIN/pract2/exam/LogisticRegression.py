#!/usr/bin/env python
# coding: utf-8

# # Logistic regression

# In[9]:


import numpy as np


# **OneHotEncoding:** $\;$ function to convert class labels into one-hot encoding representation

# In[10]:


def OneHotEncoding(y):
  # Unique class labels  
  c = np.unique(y); C = c.size;

  # Mapping class labels from 0 to C-1
  # Create a mapping from values in c to their corresponding indices
  mapping = {value: idx for idx, value in enumerate(c)}
  # Convert y using the mapping
  y = np.vectorize(mapping.get)(y)

  # Generate one-hot encoding using np.eye
  return np.eye(C)[y] 


# **ComputeGradient:** $\;$ weight update for logistic regression

# In[ ]:


def ComputeGradient(X, Y, W):
  N, D = X.shape
  X = np.concatenate((np.ones((N,1)), X),axis=1)
  N, C = Y.shape
  Z = np.zeros((N, C)).astype(float)

  # Compute logits
  a = X @ W
  # Normalizing logits for robust computation to avoid overflow
  a -= a.max(axis=1, keepdims=True) 
  # Compute softmax (probabilistic class label)
  mu = np.exp(a)
  mu /= np.sum(mu, axis=1, keepdims=True)
  # Substract actual label from probabilistic class label
  Z = mu - Y
  # Gradient is the sum over all samples of the outer dot product
  return (X.T @ Z)/N


# <p style="page-break-after:always;"></p>

# **LogisticRegressionClassification:** $\;$ classification of samples provided a weight matrix

# In[ ]:


def LogisticRegressionClassification(X, W):
  N, D = X.shape
  X = np.concatenate((np.ones((N,1)), X),axis=1)
  # Compute logits
  a = X @ W
  # Normalizing logits for robust computation to avoid overflow
  a -= a.max(axis=1, keepdims=True) 
  # Compute softmax (probabilistic class label)
  mu = np.exp(a)
  mu /= np.sum(mu, axis=1, keepdims=True)
  return np.argmax(mu,axis=1)


# **LogisticRegressionTraining:** $\;$ implementation of batch training based on stochastic gradient descent

# In[ ]:


def LogisticRegressionTraining(X, y, W=None, bs=1, maxEpochs=10, eta=1e-2, tol=1e-3):
  rng = np.random.default_rng(seed=23)
  N, D = X.shape;
  Y = OneHotEncoding(y)
  N, C = Y.shape
  if W is None:
    W = np.zeros((1+D, C))
  grad = np.inf; epoch = 0
  while np.max(np.abs(grad)) > tol and epoch < maxEpochs:
    perm = rng.permutation(N); Xperm = X[perm]; Yperm = Y[perm]
    j = 0
    for j in range(N//bs):
      X_batch = Xperm[j*bs:(j+1)*bs]
      Y_batch = Yperm[j*bs:(j+1)*bs]
      grad = ComputeGradient(X_batch, Y_batch, W)
      W = W - eta*grad
    # If there are remaining samples after splitting into batches
    if((j+1)*bs < N):
      X_batch = Xperm[(j+1)*bs:]
      Y_batch = Yperm[(j+1)*bs:]
      grad = ComputeGradient(X_batch, Y_batch, W)
      W = W - eta*grad 
    epoch += 1
  return W


# <p style="page-break-after:always;"></p>
