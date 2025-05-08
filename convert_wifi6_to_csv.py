# convert_wifi6_to_csv.py

with open("wifi6_results.txt", "r") as infile, open("wifi6_results.csv", "w") as outfile:
    outfile.write("MCS,ChannelWidth,GI,Throughput_Mbps\n")
    for line in infile:
        parts = line.split()
        if len(parts) >= 7 and "Mbit/s" in line:
            mcs = parts[0]
            width = parts[1] + " " + parts[2]  # e.g. '20 MHz'
            gi = parts[3] + " " + parts[4]     # e.g. '800 ns'
            throughput = parts[-2]             # e.g. '103.548'
            outfile.write(f"{mcs},{width},{gi},{throughput}\n")
