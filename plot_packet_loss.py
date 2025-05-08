import pandas as pd
import matplotlib.pyplot as plt

wifi6_df = pd.read_csv("../wifi6_results.csv")
wifi5_df = pd.read_csv("../wifi5_results.csv")

plt.figure(figsize=(10, 6))
for width in sorted(wifi6_df['ChannelWidth'].unique()):
    w6 = wifi6_df[wifi6_df['ChannelWidth'] == width]
    w5 = wifi5_df[wifi5_df['ChannelWidth'] == width]
    plt.plot(w6['MCS'], w6['PacketLoss'], label=f'Wi-Fi 6 - {width} MHz')
    plt.plot(w5['MCS'], w5['PacketLoss'], '--', label=f'Wi-Fi 5 - {width} MHz')

plt.xlabel("MCS Index")
plt.ylabel("Packet Loss (packets)")
plt.title("Wi-Fi 6 vs Wi-Fi 5: Packet Loss Comparison")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("nano plot_packet_loss.png")
plt.show()

