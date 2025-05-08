import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV
df = pd.read_csv('wifi6_results.csv')

# Plot throughput for each Channel Width
for width in sorted(df['ChannelWidth'].unique()):
    subset = df[df['ChannelWidth'] == width]
    plt.plot(subset['MCS'], subset['ThroughputMbps'], label=f'{width} MHz')

plt.title("Wi-Fi 6 Throughput vs MCS")
plt.xlabel("MCS Index")
plt.ylabel("Throughput (Mbps)")
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("wifi6_throughput_plot.png", dpi=300)
plt.show()
