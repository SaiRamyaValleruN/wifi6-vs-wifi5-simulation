#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/applications-module.h"
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("WifiHeNetworkCustom");

int main(int argc, char *argv[]) {
  NS_LOG_UNCOND("Running updated WiFi 6 custom simulation...");
  double simulationTime = 10.0;

  std::ofstream outFile("wifi5_results.csv");
  outFile << "MCS,ChannelWidth,GuardInterval,ThroughputMbps,PacketLoss\n";

  std::vector<std::string> giValues = {"800"};  // Wi-Fi 5 only supports 800 ns
  std::vector<uint32_t> channelWidths = {20, 40, 80, 160};

  for (uint8_t mcs = 0; mcs <= 9; ++mcs) {
    for (std::string gi : giValues) {
      for (uint32_t width : channelWidths) {

        NodeContainer nodes;
        nodes.Create(2);

        YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
        YansWifiPhyHelper phy;
        phy.SetChannel(channel.Create());

        WifiHelper wifi;
        wifi.SetStandard(WIFI_STANDARD_80211ac);
        wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue("VhtMcs" + std::to_string(mcs)),
                                     "ControlMode", StringValue("VhtMcs0"));

        WifiMacHelper mac;
        Ssid ssid = Ssid("wifi5-bss");

        mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid));
        NetDeviceContainer devices;
        devices = wifi.Install(phy, mac, nodes.Get(0));

        mac.SetType("ns3::ApWifiMac",
                    "Ssid", SsidValue(ssid),
                    "BeaconGeneration", BooleanValue(true),
                    "BeaconInterval", TimeValue(MicroSeconds(10240)));
        devices.Add(wifi.Install(phy, mac, nodes.Get(1)));

        MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
        mobility.Install(nodes);

        InternetStackHelper stack;
        stack.Install(nodes);

        Ipv4AddressHelper address;
        address.SetBase("10.1.1.0", "255.255.255.0");
        Ipv4InterfaceContainer interfaces = address.Assign(devices);

        uint16_t port = 9;
        UdpServerHelper server(port);
        ApplicationContainer apps = server.Install(nodes.Get(1));
        apps.Start(Seconds(0.0));
        apps.Stop(Seconds(simulationTime));

        UdpClientHelper client(interfaces.GetAddress(1), port);
        client.SetAttribute("MaxPackets", UintegerValue(1000000));
        client.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
        client.SetAttribute("PacketSize", UintegerValue(1472));

        apps = client.Install(nodes.Get(0));
        apps.Start(Seconds(1.0));
        apps.Stop(Seconds(simulationTime));

        Simulator::Stop(Seconds(simulationTime));
        Simulator::Run();

        Ptr<UdpServer> udpServer = DynamicCast<UdpServer>(nodes.Get(1)->GetApplication(0));
        uint32_t rxPackets = udpServer->GetReceived();
        uint32_t txPackets = 1000000;
        uint32_t lostPackets = txPackets - rxPackets;
        uint32_t totalBytes = rxPackets * 1472;
        double throughput = (totalBytes * 8) / (1e6 * (simulationTime - 1));

        outFile << unsigned(mcs) << ","
                << width << ","
                << gi << ","
                << throughput << ","
                << lostPackets << "\n";

        Simulator::Destroy();
      }
    }
  }

  outFile.close();
  return 0;
}
