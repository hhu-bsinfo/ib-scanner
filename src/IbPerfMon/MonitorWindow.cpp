#include <algorithm>
#include <IbPerfLib/Exception/IbPerfException.h>
#include <cstring>
#include "MonitorWindow.h"

namespace IbPerfMon {

char MonitorWindow::metricTable[] = {
        ' ',
        'k',
        'm',
        'g',
        't',
        'p',
        'e'
};

MonitorWindow::MonitorWindow(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, const char *title,
                             IbPerfLib::IbPerfCounter *perfCounter, IbPerfLib::IbDiagPerfCounter *diagPerfCounter,
                             uint32_t refreshInterval) :
        ListWindow(posX, posY, width, height, title),
        m_perfCounter(perfCounter),
        m_diagPerfCounter(diagPerfCounter),
        m_lastXmit(0),
        m_lastRcv(0),
        m_xmitThroughput(0),
        m_rcvThroughput(0),
        m_refreshThroughput(false),
        m_refreshInterval(refreshInterval) {
    m_refreshThread = std::thread(&MonitorWindow::RefreshThread, this);
}

MonitorWindow::~MonitorWindow() {
    m_refreshThread.detach();
}

void MonitorWindow::DrawContent() {
    m_refreshLock.lock();

    ListWindow::DrawContent();

    m_refreshLock.unlock();
}

void MonitorWindow::SetPerfCounter(IbPerfLib::IbPerfCounter *perfCounter,
        IbPerfLib::IbDiagPerfCounter *diagPerfCounter) {
    m_refreshLock.lock();

    m_perfCounter = perfCounter;
    m_diagPerfCounter = diagPerfCounter;

    m_highlight = 0;
    m_scrollOffset = 0;

    m_xmitThroughput = 0;
    m_rcvThroughput = 0;

    RefreshValues();

    m_refreshLock.unlock();
}

void MonitorWindow::RefreshValues() {
    m_items.clear();

    try {
        m_perfCounter->RefreshCounters();

        if(m_diagPerfCounter != nullptr) {
            m_diagPerfCounter->RefreshCounters();
        }
    } catch(const IbPerfLib::IbPerfException &exception) {
        m_items.emplace_back("An error occurred while refreshing the performance counters:");
        m_items.emplace_back(exception.what());
        m_items.emplace_back("Retrying...");

        return;
    }

    if(m_refreshThroughput) {
        if (m_lastXmit != 0 && m_lastRcv != 0) {
            m_xmitThroughput = (m_perfCounter->GetXmitDataBytes() - m_lastXmit) / (m_refreshInterval / 1000);
            m_rcvThroughput = (m_perfCounter->GetRcvDataBytes() - m_lastRcv) / (m_refreshInterval / 1000);

        } else {
            m_xmitThroughput = 0;
            m_rcvThroughput = 0;
        }

        m_lastXmit = m_perfCounter->GetXmitDataBytes();
        m_lastRcv = m_perfCounter->GetRcvDataBytes();

        m_refreshThroughput = false;
    }

    m_items.emplace_back(FormatValue("Xmit Throughput", m_xmitThroughput, "Bytes/s"));
    m_items.emplace_back(FormatValue("Rcv Throughput", m_xmitThroughput, "Bytes/s"));

    m_items.emplace_back(FormatValue("Xmit Data", m_perfCounter->GetXmitDataBytes(), "Bytes"));
    m_items.emplace_back(FormatValue("Rcv Data", m_perfCounter->GetRcvDataBytes(), "Bytes"));
    m_items.emplace_back(FormatValue("Xmit Pkts", m_perfCounter->GetXmitPkts()));
    m_items.emplace_back(FormatValue("Rcv Pkts", m_perfCounter->GetRcvPkts()));

    m_items.emplace_back(FormatValue("Unicast Xmit Pkts", m_perfCounter->GetUnicastXmitPkts()));
    m_items.emplace_back(FormatValue("Unicast Rcv Pkts", m_perfCounter->GetUnicastRcvPkts()));
    m_items.emplace_back(FormatValue("Multicast Xmit Pkts", m_perfCounter->GetMulticastXmitPkts()));
    m_items.emplace_back(FormatValue("Multicast Rcv Pkts", m_perfCounter->GetMulticastRcvPkts()));

    m_items.emplace_back(FormatValue("Symbol Errors", m_perfCounter->GetSymbolErrors()));
    m_items.emplace_back(FormatValue("Link Downed", m_perfCounter->GetLinkDownedCounter()));
    m_items.emplace_back(FormatValue("Link Recoveries", m_perfCounter->GetLinkRecoveryCounter()));
    m_items.emplace_back(FormatValue("Rcv Errors", m_perfCounter->GetRcvErrors()));
    m_items.emplace_back(FormatValue("Rcv Remote Physical Errors", m_perfCounter->GetRcvRemotePhysicalErrors()));
    m_items.emplace_back(FormatValue("Rcv Switch Relay Errors", m_perfCounter->GetRcvSwitchRelayErrors()));
    m_items.emplace_back(FormatValue("Xmit Discards", m_perfCounter->GetXmitDiscards()));
    m_items.emplace_back(FormatValue("Xmit Constraint Errors", m_perfCounter->GetXmitConstraintErrors()));
    m_items.emplace_back(FormatValue("Rcv Constraint Errors", m_perfCounter->GetRcvConstraintErrors()));
    m_items.emplace_back(FormatValue("Local Link Integrity Errors", m_perfCounter->GetLocalLinkIntegrityErrors()));
    m_items.emplace_back(FormatValue("Excessive Buffer Overrun Errors",
            m_perfCounter->GetExcessiveBufferOverrunErrors()));
    m_items.emplace_back(FormatValue("VL15 Dropped", m_perfCounter->GetVL15Dropped()));
    m_items.emplace_back(FormatValue("Xmit Wait", m_perfCounter->GetXmitWait()));

    m_lastXmit = m_perfCounter->GetXmitDataBytes();
    m_lastRcv = m_perfCounter->GetRcvDataBytes();

    if(m_diagPerfCounter != nullptr) {
        m_items.emplace_back(FormatValue("Lifespan", m_diagPerfCounter->GetLifespan()));

        m_items.emplace_back(FormatValue("Rq Local Length Errors", m_diagPerfCounter->GetRqLocalLengthErrors()));
        m_items.emplace_back(FormatValue("Rq Local Qp Protection Errors",
                m_diagPerfCounter->GetRqLocalQpProtectionErrors()));
        m_items.emplace_back(FormatValue("Rq Out Of Sequence Errors", m_diagPerfCounter->GetRqOutOfSequenceErrors()));
        m_items.emplace_back(FormatValue("Rq Remote Access Errors", m_diagPerfCounter->GetRqRemoteAccessErrors()));
        m_items.emplace_back(FormatValue("Rq Remote Invalid Request Errors",
                m_diagPerfCounter->GetRqRemoteInvalidRequestErrors()));
        m_items.emplace_back(FormatValue("Rq Rnr Nak Num", m_diagPerfCounter->GetRqRnrNakNum()));
        m_items.emplace_back(FormatValue("Rq Completion Queue Entry Errors",
                m_diagPerfCounter->GetRqCompletionQueueEntryErrors()));

        m_items.emplace_back(FormatValue("SqBad Response Errors", m_diagPerfCounter->GetSqBadResponseErrors()));
        m_items.emplace_back(FormatValue("Sq Local Length Errors", m_diagPerfCounter->GetSqLocalLengthErrors()));
        m_items.emplace_back(FormatValue("Sq Local Protection Errors",
                m_diagPerfCounter->GetSqLocalProtectionErrors()));
        m_items.emplace_back(FormatValue("Sq Local Qp Protection Errors",
                m_diagPerfCounter->GetSqLocalQpProtectionErrors()));
        m_items.emplace_back(FormatValue("Sq Memory Window Bind Errors",
                m_diagPerfCounter->GetSqMemoryWindowBindErrors()));
        m_items.emplace_back(FormatValue("Sq Out Of Sequence Errors",
                m_diagPerfCounter->GetSqOutOfSequenceErrors()));
        m_items.emplace_back(FormatValue("Sq Remote Access Errors", m_diagPerfCounter->GetSqRemoteAccessErrors()));
        m_items.emplace_back(FormatValue("Sq Remote Invalid Request Errors",
                m_diagPerfCounter->GetSqRemoteInvalidRequestErrors()));
        m_items.emplace_back(FormatValue("Sq Rnr Nak Num", m_diagPerfCounter->GetSqRnrNakNum()));
        m_items.emplace_back(FormatValue("Sq Remote Operation Errors",
                m_diagPerfCounter->GetSqRemoteOperationErrors()));
        m_items.emplace_back(FormatValue("Sq Rnr Nak Retries Exceeded Errors",
                m_diagPerfCounter->GetSqRnrNakRetriesExceededErrors()));
        m_items.emplace_back(FormatValue("Sq Transport Retries Exceeded Errors",
                m_diagPerfCounter->GetSqTransportRetriesExceededErrors()));
        m_items.emplace_back(FormatValue("Sq Completion Queue Entry Errors",
                m_diagPerfCounter->GetSqCompletionQueueEntryErrors()));
    }
}

void MonitorWindow::ResetValues() {
    m_refreshLock.lock();

    m_perfCounter->ResetCounters();

    m_xmitThroughput = 0;
    m_rcvThroughput = 0;

    RefreshValues();

    m_refreshLock.unlock();
}

void MonitorWindow::RefreshThread() {
    while (true) {
        m_refreshLock.lock();
        
        m_refreshThroughput = true;
        RefreshValues();
        CursesLib::WindowManager::GetInstance()->RequestRefresh();

        m_refreshLock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(m_refreshInterval));
    }
}

std::string MonitorWindow::FormatValue(const std::string &name, uint64_t value, const std::string &unit) {
    long double fValue = value;
    char buf[GetWidth()];

    // Calculate formatted value
    uint32_t counter = 0;
    while (fValue > 1000 && counter < sizeof(metricTable)) {
        fValue = fValue / 1000;
        counter++;
    }

    // Print name and value
    snprintf(buf, GetWidth(), "%-40s %.3Lf %c%s (%lu)", (name + ":").c_str(),
            fValue, metricTable[counter], unit.c_str(), value);

    return std::string(buf);
}

}