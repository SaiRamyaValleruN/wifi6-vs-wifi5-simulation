/*
 * Copyright (c) 2007,2008,2009 INRIA, UDcast
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 * Authors: Jahanzeb Farooq <jahanzeb.farooq@sophia.inria.fr>
 *          Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 *                               <amine.ismail@UDcast.com>
 */

#ifndef BANDWIDTH_MANAGER_H
#define BANDWIDTH_MANAGER_H

#include "bs-uplink-scheduler.h"
#include "cid.h"
#include "ul-job.h"
#include "wimax-net-device.h"

#include <stdint.h>

/*
 The same bandwidth manager class serves both for BS and SS though some functions are exclusive to
 only one of them.
 */

namespace ns3
{

class SSRecord;
class ServiceFlow;
class UlJob;
class UplinkScheduler;

/**
 * @ingroup wimax
 * @brief This class manage the bandwidth request and grant mechanism.
 * The bandwidth request and grant mechanism is supported by the Bandwidth
 * Manager. Both BS and SS maintain a bandwidth manager. Furthermore BS's
 * bandwidth manager works together with the uplink scheduler to determine
 * total bandwidth available and allocation size for each service flow.
 * Bandwidth request mechanism is a key feature of the WiMAX scheduler
 * since all three non-UGS services explicitly request for bandwidth by
 * sending a bandwidth request to BS.
 */
class BandwidthManager : public Object
{
  public:
    /**
     * @brief Get the type ID.
     * @return the object TypeId
     */
    static TypeId GetTypeId();
    /**
     * Constructor
     *
     * @param device WIMAX device
     */
    BandwidthManager(Ptr<WimaxNetDevice> device);
    ~BandwidthManager() override;

    // Delete copy constructor and assignment operator to avoid misuse
    BandwidthManager(const BandwidthManager&) = delete;
    BandwidthManager& operator=(const BandwidthManager&) = delete;

    void DoDispose() override;

    /**
     * Calculate allocation size function
     * @param ssRecord the SS record
     * @param serviceFlow the service flow
     * @returns the allocation size
     */
    uint32_t CalculateAllocationSize(const SSRecord* ssRecord, const ServiceFlow* serviceFlow);
    /**
     * Select flow for request function
     * @param bytesToRequest bytes to request
     * @returns a service flow
     */
    ServiceFlow* SelectFlowForRequest(uint32_t& bytesToRequest);
    /**
     * Send bandwidth request
     * @param uiuc the UIUC
     * @param allocationSize the allocation size
     */
    void SendBandwidthRequest(uint8_t uiuc, uint16_t allocationSize);
    /**
     * Process bandwidth request
     * @param bwRequestHdr the bandwidth request
     */
    void ProcessBandwidthRequest(const BandwidthRequestHeader& bwRequestHdr);
    /// Set subframe ratio
    void SetSubframeRatio();
    /**
     * Get symbols per frame allocated
     * @returns the symbols per the frame allocated
     */
    uint32_t GetSymbolsPerFrameAllocated();

  private:
    Ptr<WimaxNetDevice> m_device; ///< the device
    uint16_t m_nrBwReqsSent;      ///< bandwidth requests sent
};

} // namespace ns3

#endif /* BANDWIDTH_MANAGER_H */
