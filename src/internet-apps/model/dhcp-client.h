/*
 * Copyright (c) 2011 UPB
 * Copyright (c) 2017 NITK Surathkal
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Author: Radu Lupu <rlupu@elcom.pub.ro>
 *         Ankit Deepak <adadeepak8@gmail.com>
 *         Deepti Rajagopal <deeptir96@gmail.com>
 *
 */

#ifndef DHCP_CLIENT_H
#define DHCP_CLIENT_H

#include "dhcp-header.h"

#include "ns3/application.h"
#include "ns3/traced-value.h"

#include <list>

namespace ns3
{

class Socket;
class Packet;
class RandomVariableStream;

/**
 * @ingroup dhcp
 *
 * @class DhcpClient
 * @brief Implements the functionality of a DHCP client
 */
class DhcpClient : public Application
{
  public:
    /**
     * @brief Get the type ID.
     * @return the object TypeId
     */
    static TypeId GetTypeId();

    DhcpClient();
    ~DhcpClient() override;

    /**
     * @brief Constructor
     * @param netDevice the NetDevice DHCP should work on
     */
    DhcpClient(Ptr<NetDevice> netDevice);

    /**
     * @brief Get the the NetDevice DHCP should work on
     * @return the NetDevice DHCP should work on
     */
    Ptr<NetDevice> GetDhcpClientNetDevice();

    /**
     * @brief Set the NetDevice DHCP should work on
     * @param netDevice the NetDevice DHCP should work on
     */
    void SetDhcpClientNetDevice(Ptr<NetDevice> netDevice);

    /**
     * @brief Get the IPv4Address of current DHCP server
     * @return Ipv4Address of current DHCP server
     */
    Ipv4Address GetDhcpServer();

    int64_t AssignStreams(int64_t stream) override;

  protected:
    void DoDispose() override;

  private:
    void StartApplication() override;
    void StopApplication() override;

    /// client states
    enum States
    {
        WAIT_OFFER = 1,    //!< State of a client that waits for the offer
        REFRESH_LEASE = 2, //!< State of a client that needs to refresh the lease
        WAIT_ACK = 9       //!< State of a client that waits for acknowledgment
    };

    static const int DHCP_PEER_PORT = 67; //!< DHCP server port

    /**
     * @brief Handles changes in LinkState
     */
    void LinkStateHandler();

    /**
     * @brief Handles incoming packets from the network
     * @param socket Socket bound to port 68 of the DHCP client
     */
    void NetHandler(Ptr<Socket> socket);

    /**
     * @brief Sends DHCP DISCOVER and changes the client state to WAIT_OFFER
     */
    void Boot();

    /**
     * @brief Stores DHCP offers in m_offerList
     * @param header DhcpHeader of the DHCP OFFER message
     */
    void OfferHandler(DhcpHeader header);

    /**
     * @brief Selects an OFFER from m_offerList
     */
    void Select();

    /**
     * @brief Sends the DHCP REQUEST message and changes the client state to WAIT_ACK
     */
    void Request();

    /**
     * @brief Receives the DHCP ACK and configures IP address of the client.
     *        It also triggers the timeout, renew and rebind events.
     * @param header DhcpHeader of the DHCP ACK message
     * @param from   Address of DHCP server that sent the DHCP ACK
     */
    void AcceptAck(DhcpHeader header, Address from);

    /**
     * @brief Remove the current DHCP information and restart the process
     */
    void RemoveAndStart();

    uint8_t m_state;              //!< State of the DHCP client
    bool m_firstBoot;             //!< First boot (used to add the link state change callback)
    Ptr<NetDevice> m_device;      //!< NetDevice pointer
    Ptr<Socket> m_socket;         //!< Socket for remote communication
    Ipv4Address m_remoteAddress;  //!< Initially set to 255.255.255.255 to start DHCP
    Ipv4Address m_offeredAddress; //!< Address offered to the client
    Ipv4Address m_myAddress;      //!< Address assigned to the client
    Address m_chaddr;         //!< chaddr of the interface (stored as an Address for convenience).
    Ipv4Mask m_myMask;        //!< Mask of the address assigned
    Ipv4Address m_server;     //!< Address of the DHCP server
    Ipv4Address m_gateway;    //!< Address of the gateway
    EventId m_requestEvent;   //!< Address refresh event
    EventId m_discoverEvent;  //!< Message retransmission event
    EventId m_refreshEvent;   //!< Message refresh event
    EventId m_rebindEvent;    //!< Message rebind event
    EventId m_nextOfferEvent; //!< Message next offer event
    EventId m_timeout;        //!< The timeout period
    EventId m_collectEvent;   //!< Offer collection event
    Time m_lease;             //!< Store the lease time of address
    Time m_renew;             //!< Store the renew time of address
    Time m_rebind;            //!< Store the rebind time of address
    Time m_nextoffer;         //!< Time to try the next offer (if request gets no reply)
    Ptr<RandomVariableStream> m_ran;   //!< Uniform random variable for transaction ID
    Time m_rtrs;                       //!< Defining the time for retransmission
    Time m_collect;                    //!< Time for which client should collect offers
    bool m_offered;                    //!< Specify if the client has got any offer
    std::list<DhcpHeader> m_offerList; //!< Stores all the offers given to the client
    uint32_t m_tran;                   //!< Stores the current transaction number to be used
    TracedCallback<const Ipv4Address&> m_newLease; //!< Trace of new lease
    TracedCallback<const Ipv4Address&> m_expiry;   //!< Trace of lease expire
};

} // namespace ns3

#endif /* DHCP_CLIENT_H */
