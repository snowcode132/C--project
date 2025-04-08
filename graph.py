import pandas as pd
import matplotlib.pyplot as plt

# Load the output from the C++ program
df = pd.read_csv("output.txt")

# Optional: customize cluster colors
clusters = df['cluster'].unique()
colors = plt.cm.get_cmap('tab10', len(clusters))

# Plot
plt.figure(figsize=(10, 6))
for cluster_id in clusters:
    subset = df[df['cluster'] == cluster_id]
    plt.scatter(subset['studyTime'], subset['grade'], label=f'Cluster {cluster_id}', s=100)

plt.title("K-Means Clustering of Study Time vs Grade")
plt.xlabel("Study Time (hrs/week)")
plt.ylabel("Grade Class")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()