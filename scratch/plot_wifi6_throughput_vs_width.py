import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("wifi6_results.csv")

# Filter for MCS 4 and GI 800 ns
df_filtered = df[(df["MCS"] == 4) & (df["GuardInterval"] == 800)]

# Sort by channel width
df_filtered = df_filtered.sort_values("ChannelWidth")

plt.figure()
plt.plot(df_filtered["ChannelWidth"], df_filtered["ThroughputMbps"], marker="o", linestyle="-")
plt.xlabel("Channel Width (MHz)")
plt.ylabel("Throughput (Mbps)")
plt.title("Wi-Fi 6: Throughput vs Channel Width\n(MCS 4, GI 800 ns)")
plt.grid(True)
plt.tight_layout()
plt.savefig("plot_wifi6_throughput_vs_width.png", dpi=300)
plt.show()
