import os
import numpy as np
import matplotlib.pyplot as plt

# Make sure the output directory exists
os.makedirs("figures", exist_ok=True)

# ----- Easy Function Plot -----
x = np.linspace(0, 100, 200)
y_easy = x  # linear gradient

plt.figure(figsize=(8, 5))
plt.plot(x, y_easy, color="blue", linewidth=2)
plt.xlabel("Solution quality (bit matches)")
plt.ylabel("Fitness")
plt.title("Easy Evaluation Function Landscape")
plt.ylim(0, 105)
plt.xlim(0, 100)
plt.grid(True, linestyle="--", alpha=0.6)
plt.savefig("figures/easy_landscape.png", dpi=300)
plt.close()

# ----- Hard Function Plot -----
x = np.linspace(0, 100, 200)
y_hard = np.where(x < 80, x, 80)  # plateau at 80 after first 80 bits
# Add spike at 100
y_hard = np.where(np.isclose(x, 100, atol=0.25), 100, y_hard)

plt.figure(figsize=(8, 5))
plt.plot(x, y_hard, color="red", linewidth=2)
plt.xlabel("Solution quality (bit matches)")
plt.ylabel("Fitness")
plt.title("Hard Evaluation Function Landscape")
plt.ylim(0, 105)
plt.xlim(0, 100)
plt.grid(True, linestyle="--", alpha=0.6)
plt.savefig("figures/hard_landscape.png", dpi=300)
plt.close()

"Figures saved in ./figures as easy_landscape.png and hard_landscape.png"
