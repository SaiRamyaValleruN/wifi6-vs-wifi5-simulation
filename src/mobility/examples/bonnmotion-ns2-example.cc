/*
 * Copyright 2012 Eric Gamess
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 */

/*
 * Example program using a ns-2-formatted mobility trace generated
 * by the BonnMotion mobility framework.
 *
 * With the provided tracefile (bonnmotion.ns_movements), the movements of
 * one node are simulated for 1000 seconds.  There are a few other files
 * that provide metadata about the mobility:
 * - src/mobility/examples/bonnmotion.ns_params
 * - src/mobility/examples/bonnmotion.params
 *
 * These files are documented in the BonnMotion documentation.
 *
 * It is important to remember that the trace file dictates how many nodes
 * (in this case, one) and how long (in this case, 1000 seconds) that the
 * ns-3 program should use.  If you want to change the mobility pattern,
 * number of nodes, or duration, you need to use BonnMotion or another
 * tool to generate a new trace.
 *
 * Finally, note that you can visualize this program using the pyviz
 * visualizer:
 *  ./ns3 run bonnmotion-ns2-example --vis
 */

#include "ns3/core-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;

void
showPosition(Ptr<Node> node, double deltaTime)
{
    uint32_t nodeId = node->GetId();
    Ptr<MobilityModel> mobModel = node->GetObject<MobilityModel>();
    Vector3D pos = mobModel->GetPosition();
    Vector3D speed = mobModel->GetVelocity();
    std::cout << "At " << Simulator::Now().GetSeconds() << " node " << nodeId << ": Position("
              << pos.x << ", " << pos.y << ", " << pos.z << ");   Speed(" << speed.x << ", "
              << speed.y << ", " << speed.z << ")" << std::endl;

    Simulator::Schedule(Seconds(deltaTime), &showPosition, node, deltaTime);
}

int
main(int argc, char* argv[])
{
    std::cout.precision(2);
    std::cout.setf(std::ios::fixed);

    double deltaTime = 100;
    std::string traceFile = "src/mobility/examples/bonnmotion.ns_movements";

    CommandLine cmd(__FILE__);
    cmd.AddValue("traceFile", "Ns2 movement trace file", traceFile);
    cmd.AddValue("deltaTime", "time interval (s) between updates (default 100)", deltaTime);
    cmd.Parse(argc, argv);

    Ptr<Node> n0 = CreateObject<Node>();

    Ns2MobilityHelper ns2 = Ns2MobilityHelper(traceFile);
    ns2.Install();

    Simulator::Schedule(Seconds(0), &showPosition, n0, deltaTime);

    Simulator::Stop(Seconds(1000));
    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
