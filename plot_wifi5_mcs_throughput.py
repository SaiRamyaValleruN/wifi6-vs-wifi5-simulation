import pandas as pd
import matplotlib.pyplot as plt

# Load Wi-Fi 5 data
df = pd.read_csv("wifi5_results.csv")

# Filter only 800 ns GI (Wi-Fi 5 supports only this)
df = df[df["GuardInterval"] == 800]

# Plot for each channel width
for width in sorted(df["ChannelWidth"].unique()):
    subset = df[df["ChannelWidth"] == width]
    plt.plot(subset["MCS"], subset["ThroughputMbps"], label=f"{width} MHz")

plt.title("Wi-Fi 5: Throughput vs MCS")
plt.xlabel("MCS Index")
plt.ylabel("Throughput (Mbps)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
