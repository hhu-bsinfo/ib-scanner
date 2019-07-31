/*
 * Copyright (C) 2018 Heinrich-Heine-Universitaet Duesseldorf,
 * Institute of Computer Science, Department Operating Systems
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef IBSCANNER_IBSCANNER_H
#define IBSCANNER_IBSCANNER_H

#include <detector/IbDiagPerfCounter.h>
#include <detector/IbFabric.h>
#include <curses/OkMessageWindow.h>
#include <curses/MenuWindow.h>
#include "MonitorWindow.h"

namespace Scanner {

/**
 * The main class.
 *
 * Uses a MenuWindow to show all Infiniband devices and a MonitorWindow to display the counters of a single device.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class Scanner {

public:
    /**
     * Constructor.
     *
     * @param compatibility Set to true, to activate compatibility mode.
     */
    explicit Scanner(bool network, bool compatibility);

    /**
     * Destructor.
     */
    ~Scanner();

    /**
     * Start the performance monitor.
     */
    void Run();

private:
    /**
     * Scan the entire Infiniband fabric for devices.
     */
    void ScanFabric();

    /**
     * Show the MenuWindow and the MonitorWindow.
     */
    void StartMonitoring();

    /**
     * Set the amount of monitor windows to either 1, 2, or 4.
     *
     * @param windowCount The amount of monitor windows to show (1, 2, or 4)
     */
    void SetWindowCount(uint8_t windowCount);

private:

    std::unordered_map<uint64_t, Detector::IbDiagPerfCounter*> m_diagPerfCounterMap;

    Detector::IbFabric *m_fabric;

    Curses::WindowManager *m_manager;

    char m_helpMessage[512];
    Curses::OkMessageWindow *m_helpWindow;
    Curses::MenuWindow *m_menuWindow;
    MonitorWindow *m_monitorWindow[4];

    int m_oldStderr;

    bool m_network;
    bool m_compatibility;

    bool m_isRunning;
};

}

#endif
