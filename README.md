# WiFi6 vs WiFi5 Simulation

This project compares the performance of **Wi-Fi 6 (802.11ax)** and **Wi-Fi 5 (802.11ac)** using NS-3 simulations.  
The evaluation focuses on metrics like **throughput**, **packet loss**, **latency**, and the advantages of features such as **OFDMA**, **MU-MIMO**, and **BSS Coloring**.

---

## About NS-3

[![codecov](https://codecov.io/gh/nsnam/ns-3-dev-git/branch/master/graph/badge.svg)](https://codecov.io/gh/nsnam/ns-3-dev-git/branch)  
[![Gitlab CI](https://gitlab.com/nsnam/ns-3-dev/badges/master/pipeline.svg)](https://gitlab.com/nsnam/ns-3-dev/-/pipelines)  
[![Github CI](https://github.com/nsnam/ns-3-dev-git/actions/workflows/per_commit.yml/badge.svg)](https://github.com/nsnam/ns-3-dev-git/actions/workflows/per_commit.yml)  
[![Latest Release](https://gitlab.com/nsnam/ns-3-dev/-/badges/release.svg)](https://gitlab.com/nsnam/ns-3-dev/-/releases)

[ns-3](https://www.nsnam.org) is a discrete-event network simulator designed for Internet systems. It provides accurate modeling of network protocols and technologies and is widely used for simulation research and education.

---

## Project Goals

- Simulate both Wi-Fi 6 and Wi-Fi 5 using NS-3 modules
- Measure and compare:
  - Throughput
  - Packet loss
  - Delay
  - Effect of channel width and guard intervals
- Visualize results with Python-based plotting scripts

---

## How to Build and Run

### 1. Configure and Build ns-3:
```bash
./ns3 configure --enable-examples --enable-tests
./ns3 build
```

### 2. Run Simulations
Example:
```bash
./ns3 run scratch/wifi6_custom
./ns3 run scratch/wifi5_custom
```

### 3. Plot Results with Python:
Activate your Python virtual environment:
```bash
source myenv/bin/activate
```

Run plotting scripts:
```bash
python3 plot_wifi6_throughput.py
python3 compare_wifi5_wifi6.py
```

---

## Documentation

- 📖 Tutorial: https://www.nsnam.org/documentation/latest/
- 📘 API (Doxygen): https://www.nsnam.org/doxygen/
- 🧠 Wiki: https://www.nsnam.org/wiki/
- 💬 User group: https://groups.google.com/g/ns-3-users
- 🔧 GitLab Project: https://gitlab.com/nsnam/ns-3-dev

---

## Contributors

- **Sai Ramya Valleru**
- **Dheeraja Mandava**
- **Tejaswini Sodagudi**

---

## License

This project uses the GNU General Public License v2.0 only (GPL-2.0-only).  
See the `LICENSE` file for more details.
