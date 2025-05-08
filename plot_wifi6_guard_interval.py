import pandas as pd
import matplotlib.pyplot as plt

# Load Wi-Fi 6 results
df = pd.read_csv("wifi6_results.csv")

# Convert guard interval to integer for sorting
df["GuardInterval"] = df["GuardInterval"].astype(int)

# Group by GuardInterval and plot
plt.figure(figsize=(10, 6))
for width in sorted(df["ChannelWidth"].unique()):
    subset = df[df["ChannelWidth"] == width]
    avg_throughput = subset.groupby("GuardInterval")["ThroughputMbps"].mean()
    plt.plot(avg_throughput.index, avg_throughput.values, marker="o", label=f"{width} MHz")

plt.title("Wi-Fi 6: Throughput vs Guard Interval")
plt.xlabel("Guard Interval (ns)")
plt.ylabel("Avg Throughput (Mbps)")
plt.grid(True)
plt.legend()
plt.tight_layout()

plt.savefig("plot_wifi6_guard_interval.png")
plt.show()
