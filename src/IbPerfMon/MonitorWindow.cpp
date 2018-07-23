#include <algorithm>
#include <IbPerfLib/IbMadException.h>
#include "MonitorWindow.h"

namespace IbPerfMon {

char MonitorWindow::metricTable[] = {
        'k',
        'm',
        'g',
        't',
        'p',
        'e'
};

MonitorWindow::MonitorWindow(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, const char *title,
                             IbPerfLib::IbPerfCounter *perfCounter, uint32_t refreshInterval) :
        ListWindow(posX, posY, width, height, title),
        m_perfCounter(perfCounter),
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
    ListWindow::DrawContent();
}

void MonitorWindow::SetPerfCounter(IbPerfLib::IbPerfCounter *counter) {
    m_perfCounter = counter;
    m_highlight = 0;
    m_xmitThroughput = 0;
    m_rcvThroughput = 0;

    RefreshValues();
}

void MonitorWindow::RefreshValues() {
    m_items.clear();

    try {
        m_perfCounter->RefreshCounters();
    } catch(const IbPerfLib::IbMadException &exception) {
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

    m_items.emplace_back("Xmit Througput:                      " + FormatValue(m_xmitThroughput, "Bytes/s") +
            " (" + std::to_string(m_xmitThroughput) + ")");
    m_items.emplace_back("Rcv Througput:                       " + FormatValue(m_rcvThroughput, "Bytes/s") +
            " (" + std::to_string(m_rcvThroughput) + ")");
    m_items.emplace_back(
            "Xmit Data:                           " + FormatValue(m_perfCounter->GetXmitDataBytes(), "Bytes") +
            " (" + std::to_string(m_perfCounter->GetXmitDataBytes()) + ")");
    m_items.emplace_back(
            "Rcv Data:                            " + FormatValue(m_perfCounter->GetRcvDataBytes(), "Bytes") +
            " (" + std::to_string(m_perfCounter->GetRcvDataBytes()) + ")");
    m_items.emplace_back("Xmit Pkts:                           " + FormatValue(m_perfCounter->GetXmitPkts()) +
                         " (" + std::to_string(m_perfCounter->GetXmitPkts()) + ")");
    m_items.emplace_back("Rcv Pkts:                            " + FormatValue(m_perfCounter->GetRcvPkts()) +
                         " (" + std::to_string(m_perfCounter->GetRcvPkts()) + ")");

    m_items.emplace_back("Unicast Xmit Pkts:                   " + FormatValue(m_perfCounter->GetUnicastXmitPkts()) +
                         " (" + std::to_string(m_perfCounter->GetUnicastXmitPkts()) + ")");
    m_items.emplace_back("Unicast Rcv Pkts:                    " + FormatValue(m_perfCounter->GetUnicastRcvPkts()) +
                         " (" + std::to_string(m_perfCounter->GetUnicastRcvPkts()) + ")");
    m_items.emplace_back("Multicast Xmit Pkts:                 " + FormatValue(m_perfCounter->GetMulticastXmitPkts()) +
                         " (" + std::to_string(m_perfCounter->GetMulticastXmitPkts()) + ")");
    m_items.emplace_back("Multicast Rcv Pkts:                  " + FormatValue(m_perfCounter->GetMulticastRcvPkts()) +
                         " (" + std::to_string(m_perfCounter->GetMulticastRcvPkts()) + ")");

    m_items.emplace_back("Symbol Errors:                       " + FormatValue(m_perfCounter->GetSymbolErrors()) +
                         " (" + std::to_string(m_perfCounter->GetSymbolErrors()) + ")");
    m_items.emplace_back("Link Downed:                         " + FormatValue(m_perfCounter->GetLinkDownedCounter()) +
                         " (" +
                         std::to_string(m_perfCounter->GetLinkDownedCounter()) + ")");
    m_items.emplace_back(
            "Link Recoveries:                     " + FormatValue(m_perfCounter->GetLinkRecoveryCounter()) +
            " (" + std::to_string(m_perfCounter->GetLinkRecoveryCounter()) + ")");
    m_items.emplace_back("Rcv Errors:                          " + FormatValue(m_perfCounter->GetRcvErrors()) +
                         " (" + std::to_string(m_perfCounter->GetRcvErrors()) + ")");
    m_items.emplace_back(
            "Rcv Remote Physical Errors:          " + FormatValue(m_perfCounter->GetRcvRemotePhysicalErrors())
            + " (" + std::to_string(m_perfCounter->GetRcvRemotePhysicalErrors()) + ")");
    m_items.emplace_back(
            "Rcv Switch Relay Errors:             " + FormatValue(m_perfCounter->GetRcvSwitchRelayErrors()) +
            " (" + std::to_string(m_perfCounter->GetRcvSwitchRelayErrors()) + ")");
    m_items.emplace_back("Xmit Discards:                       " + FormatValue(m_perfCounter->GetXmitDiscards()) +
                         " (" + std::to_string(m_perfCounter->GetXmitDiscards()) + ")");
    m_items.emplace_back(
            "Xmit Constraint Errors:              " + FormatValue(m_perfCounter->GetXmitConstraintErrors()) +
            " (" + std::to_string(m_perfCounter->GetXmitConstraintErrors()) + ")");
    m_items.emplace_back(
            "Rcv Constraint Errors:               " + FormatValue(m_perfCounter->GetRcvConstraintErrors()) +
            " (" + std::to_string(m_perfCounter->GetRcvConstraintErrors()) + ")");
    m_items.emplace_back(
            "Local Link Integrity Errors:         " + FormatValue(m_perfCounter->GetLocalLinkIntegrityErrors())
            + " (" + std::to_string(m_perfCounter->GetLocalLinkIntegrityErrors()) + ")");
    m_items.emplace_back("Excessive Buffer Overrun Errors:     " +
                         FormatValue(m_perfCounter->GetExcessiveBufferOverrunErrors()) +
                         " (" + std::to_string(m_perfCounter->GetExcessiveBufferOverrunErrors()) + ")");
    m_items.emplace_back("VL15 Dropped:                        " + FormatValue(m_perfCounter->GetVL15Dropped()) +
                         " (" + std::to_string(m_perfCounter->GetVL15Dropped()) + ")");
    m_items.emplace_back("Xmit Wait:                           " + FormatValue(m_perfCounter->GetXmitWait()) +
                         " (" + std::to_string(m_perfCounter->GetXmitWait()) + ")");

    m_lastXmit = m_perfCounter->GetXmitDataBytes();
    m_lastRcv = m_perfCounter->GetRcvDataBytes();
}

void MonitorWindow::ResetValues() {
    m_perfCounter->ResetCounters();

    m_xmitThroughput = 0;
    m_rcvThroughput = 0;

    RefreshValues();
}

void MonitorWindow::RefreshThread() {
    while (true) {
        m_refreshThroughput = true;
        RefreshValues();
        CursesLib::WindowManager::GetInstance()->RequestRefresh();
        std::this_thread::sleep_for(std::chrono::milliseconds(m_refreshInterval));
    }
}

std::string MonitorWindow::FormatValue(uint64_t value, const char *unit) {
    char buf[100];
    long double fValue = value;

    uint32_t counter = 0;
    while (fValue > 1000 && counter < sizeof(metricTable)) {
        fValue = fValue / 1000;
        counter++;
    }

    if (counter == 0) {
        snprintf(buf, 100, "%.3Lf %s", fValue, unit);
    } else {
        snprintf(buf, 100, "%.3Lf %c%s", fValue, metricTable[counter - 1], unit);
    }

    return std::string(buf);
}

}