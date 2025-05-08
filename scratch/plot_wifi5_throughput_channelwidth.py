import pandas as pd
import matplotlib.pyplot as plt

# Load Wi-Fi 5 results
df = pd.read_csv("wifi5_results.csv")

# Group by Channel Width and calculate average throughput
throughput_avg = df.groupby('ChannelWidth')['ThroughputMbps'].mean()

# Plot
plt.figure(figsize=(10, 6))
throughput_avg.plot(kind='bar', color='skyblue')
plt.title("Wi-Fi 5: Throughput vs. Channel Width")
plt.xlabel("Channel Width (MHz)")
plt.ylabel("Average Throughput (Mbps)")
plt.grid(axis='y')
plt.tight_layout()
plt.savefig("wifi5_throughput_vs_channelwidth.png")
plt.show()
