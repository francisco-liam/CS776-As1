import os
import glob
import pandas as pd
import matplotlib.pyplot as plt

# Create the plots directory if it doesn't exist
os.makedirs("plots", exist_ok=True)

# Find all CSV files in the current directory that start with 'hillclimber'
csv_files = glob.glob("hillclimber*.csv")

for csv_file in csv_files:
    # Read the CSV (assume two columns: iteration, fitness)
    df = pd.read_csv(csv_file, header=None, names=["iteration", "fitness"])
    plt.figure()
    plt.plot(df["iteration"], df["fitness"], label=csv_file)
    plt.axhline(100, color='red', linestyle='--', label='Optimal (100)')
    plt.xlabel("Iteration")
    plt.ylabel("Average Fitness")
    plt.title(f"Average Fitness vs Iteration\n{csv_file}")
    plt.legend()
    plt.tight_layout()
    # Save the plot
    plot_path = os.path.join("plots", os.path.splitext(os.path.basename(csv_file))[0] + ".png")
    plt.savefig(plot_path)
    plt.close()
    print(f"Saved plot to {plot_path}")