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

#ifndef PROJECT_IBPERFMON_H
#define PROJECT_IBPERFMON_H

#include <IbPerfLib/IbFabric.h>
#include <CursesLib/OkMessageWindow.h>
#include <CursesLib/MenuWindow.h>
#include "MonitorWindow.h"

namespace IbPerfMon {

/**
 * The main class.
 *
 * Uses a MenuWindow to show all Infiniband devices and a MonitorWindow to display the counters of a single device.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class IbPerfMon {

public:
    /**
     * Constructor.
     *
     * @param compatibility Set to true, to activate compatibility mode.
     */
    explicit IbPerfMon(bool compatibility = false);

    /**
     * Destructor.
     */
    ~IbPerfMon();

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

    IbPerfLib::IbFabric *m_fabric;

    CursesLib::WindowManager *m_manager;

    char m_helpMessage[512];
    CursesLib::OkMessageWindow *m_helpWindow;
    CursesLib::MenuWindow *m_menuWindow;
    MonitorWindow *m_monitorWindow[4];

    bool m_compatibility;
    bool m_isRunning;
};

}

#endif
