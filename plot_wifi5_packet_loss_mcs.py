import pandas as pd
import matplotlib.pyplot as plt

# Load Wi-Fi 5 results
df = pd.read_csv("wifi5_results.csv")

# Group by MCS and calculate average packet loss
packet_loss_avg = df.groupby('MCS')['PacketLoss'].mean()

# Plot
plt.figure(figsize=(10, 6))
packet_loss_avg.plot(marker='o', linestyle='-')
plt.title("Wi-Fi 5: Packet Loss vs. MCS")
plt.xlabel("MCS Index")
plt.ylabel("Average Packet Loss (packets)")
plt.grid(True)
plt.tight_layout()
plt.savefig("wifi5_packet_loss_vs_mcs.png")
plt.show()
