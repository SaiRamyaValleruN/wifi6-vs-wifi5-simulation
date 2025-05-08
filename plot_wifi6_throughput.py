import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Load CSV
df = pd.read_csv("wifi6_results.csv")

# Clean up columns
df["ChannelWidth"] = df["ChannelWidth"].str.replace(" MHz", "")
df["GI"] = df["GI"].str.replace(" ns", "")

# Filter to 800ns only (fastest)
df = df[df["GI"] == "800"]

# Plot
plt.figure(figsize=(12, 6))
sns.barplot(data=df, x="MCS", y="Throughput_Mbps", hue="ChannelWidth")

plt.title("Wi-Fi 6 Throughput vs MCS (800ns GI)")
plt.xlabel("MCS Index")
plt.ylabel("Throughput (Mbps)")
plt.legend(title="Channel Width (MHz)")
plt.grid(True)
plt.tight_layout()
plt.savefig("wifi6_throughput_plot.png")
plt.show()
