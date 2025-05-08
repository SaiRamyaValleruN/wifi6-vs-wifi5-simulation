import pandas as pd
import matplotlib.pyplot as plt

# Load both Wi-Fi 5 and Wi-Fi 6 results
df5 = pd.read_csv("wifi5_results.csv")
df6 = pd.read_csv("wifi6_results.csv")

# Group by MCS and calculate average packet loss
packet_loss_avg_5 = df5.groupby('MCS')['PacketLoss'].mean()
packet_loss_avg_6 = df6.groupby('MCS')['PacketLoss'].mean()

# Plot
plt.figure(figsize=(10, 6))
plt.plot(packet_loss_avg_5.index, packet_loss_avg_5.values, marker='o', label='Wi-Fi 5')
plt.plot(packet_loss_avg_6.index, packet_loss_avg_6.values, marker='s', label='Wi-Fi 6')
plt.title("Packet Loss vs. MCS: Wi-Fi 5 vs. Wi-Fi 6")
plt.xlabel("MCS Index")
plt.ylabel("Average Packet Loss (packets)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("wifi5_vs_wifi6_packet_loss_vs_mcs.png")
plt.savefig("wifi6_vs_wifi5_packet_loss.png", dpi=300, bbox_inches='tight')
plt.show()

