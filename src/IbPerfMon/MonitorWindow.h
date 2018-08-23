#ifndef PROJECT_MONITORWINDOW_H
#define PROJECT_MONITORWINDOW_H

#include <thread>
#include <mutex>
#include <unistd.h>
#include <ncurses.h>
#include <IbPerfLib/IbPerfCounter.h>
#include <IbPerfLib/IbDiagPerfCounter.h>
#include <CursesLib/Window.h>
#include <CursesLib/WindowManager.h>
#include <CursesLib/ListWindow.h>

namespace IbPerfMon {

/**
 * ListWindow, which shows the performance counters of an Infiniband device.
 *
 * The counters are automatically refreshed in a given interval.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class MonitorWindow : public CursesLib::ListWindow {

public:
    /**
     * Constructor.
     *
     * @param posX X-coordinate of upper left corner
     * @param posY Y-coordinate of upper left corner
     * @param width The width
     * @param height The height
     * @param title The title (shown at the window's top)
     * @param perfCounter The performance counter to be displayed
     * @param refreshInterval The interval, in which the counters shall be refreshed
     */
    MonitorWindow(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, const char *title,
                  IbPerfLib::IbPerfCounter *perfCounter, IbPerfLib::IbDiagPerfCounter *diagPerfCounter = nullptr,
                  uint32_t refreshInterval = 2000);

    /**
     * Destructor.
     */
    ~MonitorWindow() override;

    /**
     * Set the performance counters.
     */
    void SetPerfCounter(IbPerfLib::IbPerfCounter *perfCounter, IbPerfLib::IbDiagPerfCounter *diagPerfCounter = nullptr);

    /**
     * Reset the counters.
     */
    void ResetValues();

private:
    /**
     * Overriding function from Window.
     */
    void DrawContent() override;

    /**
     * Refresh the counters.
     */
    void RefreshValues();

    /**
     * Refreshes the values in the given interval.
     */
    void RefreshThread();

    /**
     * Format a value.
     *
     * @param value The value
     * @param unit The values's unit (Default = "Units")
     *
     * @return The formatted string
     */
    std::string FormatValue(const std::string &name, uint64_t value, const std::string &unit = "Units");

private:

    IbPerfLib::IbPerfCounter *m_perfCounter;
    IbPerfLib::IbDiagPerfCounter *m_diagPerfCounter;

    uint64_t m_lastXmit, m_lastRcv;
    uint64_t m_xmitThroughput, m_rcvThroughput;
    bool m_refreshThroughput;

    std::mutex m_refreshLock;
    std::thread m_refreshThread;
    uint32_t m_refreshInterval;

    static char metricTable[7];
};

}

#endif
