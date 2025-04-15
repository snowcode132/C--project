import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file produced by your C++ code
df = pd.read_csv("output.txt")

centroids = df.groupby('cluster', as_index=False)[['studyTime', 'grade']].mean()

plt.figure(figsize=(8, 6))
plt.scatter(df['studyTime'], df['grade'], c=df['cluster'], cmap='viridis', alpha=0.6, label='Data Points')

# Plot centroids as red X markers
plt.scatter(centroids['studyTime'], centroids['grade'], color='red', marker='X', s=150, label='Centroids')


centroids_sorted = centroids.sort_values(by='cluster')
plt.plot(centroids_sorted['studyTime'], centroids_sorted['grade'], linestyle='--', color='black', label='Centroid Path')

#Adding the labels and title
plt.xlabel('Study Time')
plt.ylabel('Grade')
plt.title('K-Means Clustering: Data Points and Centroids')
plt.legend()

# Show the plot
plt.show()