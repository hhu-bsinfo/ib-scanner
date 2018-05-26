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

#include <ncurses.h>
#include "CursesLib/WindowManager.h"
#include "CursesLib/OkMessageWindow.h"
#include "Version.h"
#include "MonitorWindow.h"
#include "IbPerfMon.h"

namespace IbPerfMon {

IbPerfMon::IbPerfMon() :
        m_fabric(nullptr),
        m_manager(CursesLib::WindowManager::GetInstance()),
        m_helpWindow(nullptr),
        m_menuWindow(nullptr),
        m_monitorWindow(nullptr),
        m_isRunning(true)
{
    snprintf(m_helpMessage, 512, "IbPerfMon %s - git %s\n"
                                 "Build date: %s\n"
                                 "Copyright (C) 2018 Heinrich-Heine-Universitaet Duesseldorf,\n"
                                 "Institute of Computer Science, Department Operating Systems\n"
                                 "Licensed under GPL v3\n\n"
                                 "Up/Down: Navigate menu\n"
                                 "Right/Left: Open/Close menu entry\n"
                                 "Enter: Select\n"
                                 "Tab: Switch window", VERSION, GIT_REV, BUILD_DATE);
}

IbPerfMon::~IbPerfMon() {
    delete m_helpWindow;
    delete m_monitorWindow;
    delete m_menuWindow;
    delete m_fabric;
}

void IbPerfMon::Run() {
    m_manager->Initialize();

    m_helpWindow = new CursesLib::OkMessageWindow("Help", m_helpMessage, [] {});

    m_manager->Start();

    ScanFabric();

    m_manager->AddMenuFunction("Help", [&] { m_manager->RegisterWindow(m_helpWindow); });
    m_manager->AddMenuFunction("Reset Current", [&] {
        m_monitorWindow->GetPerfCounter()->ResetCounters();
        m_monitorWindow->RefreshValues();
        m_manager->RequestRefresh();
    });
    m_manager->AddMenuFunction("Reset All", [&] {
        m_fabric->ResetCounters();
        m_monitorWindow->RefreshValues();
        m_manager->RequestRefresh();
    });
    m_manager->AddMenuFunction("Exit", [&] { m_isRunning = false; });

    StartMonitoring();

    m_manager->DeregisterWindow(m_helpWindow);

    m_manager->Stop();
}

void IbPerfMon::ScanFabric() {
    char doneMsgBuf[100];

    CursesLib::MessageWindow scanMsg("IbPerfMon", "Scanning fabric! Please wait...");
    m_manager->RegisterWindow(&scanMsg);

    try {
        m_fabric = new IbPerfLib::IbFabric();
    } catch (std::runtime_error &error) {
        m_manager->Stop();

        endwin();
        printf("%s\nDo you have root-privileges?\n", error.what());

        exit(EXIT_FAILURE);
    }

    m_manager->DeregisterWindow(&scanMsg);

    bool wait = true;

    snprintf(doneMsgBuf, 100, "Finished scanning fabric! %d nodes found.", m_fabric->GetNumNodes());
    CursesLib::OkMessageWindow doneMsg("IbPerfMon", doneMsgBuf, [&] { wait = false; });
    CursesLib::WindowManager::GetInstance()->RegisterWindow(&doneMsg);

    while (wait);
}

void IbPerfMon::StartMonitoring() {
    uint32_t termWidth = m_manager->GetTerminalWidth();
    uint32_t termHeight = m_manager->GetTerminalHeight();

    m_menuWindow = new CursesLib::MenuWindow(0, 0, 70, termHeight - 1, "Menu");
    m_monitorWindow = new MonitorWindow(70, 0, termWidth - 70, termHeight - 1,
                                        m_fabric->GetNodes()[0]->GetDescription().c_str(), m_fabric->GetNodes()[0]);

    for (IbPerfLib::IbNode *node : m_fabric->GetNodes()) {
        CursesLib::MenuItem item(node->GetDescription().c_str(), [&, node]() {
            m_monitorWindow->SetPerfCounter(node);
            m_monitorWindow->SetTitle(node->GetDescription().c_str());
        });

        for (IbPerfLib::IbPort *port : node->GetPorts()) {
            char portName[10];

            snprintf(portName, 10, "Port %d", unsigned(port->GetNum()));
            item.AddSubitem(CursesLib::MenuItem(portName, [&, port, portName]() {
                m_monitorWindow->SetPerfCounter(port);
                m_monitorWindow->SetTitle(portName);
            }));
        }

        m_menuWindow->AddItem(item);
    }

    m_manager->RegisterWindow(m_monitorWindow);
    m_manager->RegisterWindow(m_menuWindow);

    while (m_isRunning);

    m_manager->DeregisterWindow(m_menuWindow);
    m_manager->DeregisterWindow(m_monitorWindow);
}

}

int main(int argc, char *argv[]) {
    IbPerfMon::IbPerfMon perfMon;

    perfMon.Run();

    exit(EXIT_SUCCESS);
}