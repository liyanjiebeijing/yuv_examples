import numpy as np

def calculate_mae(a, b):
    return np.mean(np.abs(a - b))

def calculate_max_error(a, b):
    return np.max(np.abs(a - b))

def calculate_mse(a, b):
    return np.mean((a - b) ** 2)

def calculate_cosine_difference(a, b):
    norm_a = np.linalg.norm(a)
    norm_b = np.linalg.norm(b)
    cosine_diff = (1 + (np.dot(a, b) / (norm_a * norm_b))) / 2
    return cosine_diff