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

#include <cstring>
#include <sstream>
#include "WindowManager.h"
#include "MessageWindow.h"

namespace Curses {

MessageWindow::MessageWindow(uint32_t posX, uint32_t posY, const char *title, const char *message) :
        Window(posX, posY, 0, 0, title),
        m_msgWidth(0),
        m_posChanged(true) {
    std::stringstream stream(message);
    std::string line;

    while (getline(stream, line)) {
        m_messageLines.emplace_back(line);

        if (line.length() > m_msgWidth) {
            m_msgWidth = static_cast<uint32_t>(line.length());
        }
    }

    m_width = static_cast<uint32_t>(m_msgWidth > m_title.size() ? m_msgWidth : m_title.size()) + 6;
    m_height = static_cast<uint32_t>(m_messageLines.size() + 7);
}

MessageWindow::MessageWindow(const char *title, const char *message) : Window(0, 0, 0, 0, title),
                                                                       m_msgWidth(0),
                                                                       m_posChanged(true) {
    WindowManager *manager = WindowManager::GetInstance();

    std::stringstream stream(message);
    std::string line;

    while (getline(stream, line)) {
        m_messageLines.emplace_back(line);

        if (line.length() > m_msgWidth) {
            m_msgWidth = static_cast<uint32_t>(line.length());
        }
    }

    m_width = static_cast<uint32_t>(m_msgWidth > m_title.size() ? m_msgWidth : m_title.size()) + 6;
    m_height = static_cast<uint32_t>(m_messageLines.size() + 7);

    m_posX = (manager->GetTerminalWidth() - m_width > 0) ? (manager->GetTerminalWidth() - m_width) / 2 : 0;
    m_posY = (manager->GetTerminalHeight() - m_height > 0) ? (manager->GetTerminalHeight() - m_height) / 2 : 0;
}

void MessageWindow::DrawContent() {
    Window::DrawContent();

    auto msgPosX = GetWidth() - m_msgWidth > 0 ? (GetWidth() - m_msgWidth) / 2 - 1 : 0;
    auto msgPosY = static_cast<uint32_t>((GetHeight() - m_messageLines.size()) / 2);

    for (uint32_t i = 0; i < m_messageLines.size(); i++) {
        PrintStringAt(msgPosX, msgPosY + i, m_messageLines[i].c_str());
    }
}

void MessageWindow::HandleKey(int c) {
    Window::HandleKey(c);
}

}