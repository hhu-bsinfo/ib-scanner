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

#include <utility>
#include "WindowManager.h"
#include "YesNoMessageWindow.h"

namespace CursesLib {

YesNoMessageWindow::YesNoMessageWindow(uint32_t posX, uint32_t posY, const char *title, const char *message,
                                       std::function<void(bool)> onClick) :
        MessageWindow(posX, posY, title, message),
        m_yesButtonPosX(0),
        m_noButtonPosX(0),
        m_buttonPosY(0),
        m_choice(true),
        m_onClick(std::move(onClick)) {
    if (GetWidth() < m_yesButton.length() + m_noButton.length() + 10) {
        WindowManager *manager = WindowManager::GetInstance();

        m_width = static_cast<uint32_t>(m_yesButton.length() + m_noButton.length() + 10);
        m_posY = (manager->GetTerminalHeight() - m_height > 0) ? (manager->GetTerminalHeight() - m_height) / 2 : 0;

        wresize(m_window, m_height, m_width);
    }

    m_yesButtonPosX = static_cast<uint32_t >(GetWidth() - m_yesButton.length()) / 4;
    m_noButtonPosX = m_yesButtonPosX + static_cast<uint32_t >(GetWidth() - m_noButton.length()) / 2;

    m_buttonPosY = GetHeight() - 1;
}

YesNoMessageWindow::YesNoMessageWindow(const char *message, const char *title, std::function<void(bool)> onClick) :
        MessageWindow(message, title),
        m_yesButtonPosX(0),
        m_noButtonPosX(0),
        m_buttonPosY(0),
        m_choice(true),
        m_onClick(std::move(onClick)) {
    if (GetWidth() < m_yesButton.length() + m_noButton.length() + 10) {
        WindowManager *manager = WindowManager::GetInstance();

        m_width = static_cast<uint32_t>(m_yesButton.length() + m_noButton.length() + 10);
        m_posY = (manager->GetTerminalHeight() - m_height > 0) ? (manager->GetTerminalHeight() - m_height) / 2 : 0;

        wresize(m_window, m_height, m_width);
    }

    m_yesButtonPosX = static_cast<uint32_t >(GetWidth() - m_yesButton.length()) / 4;
    m_noButtonPosX = m_yesButtonPosX + static_cast<uint32_t >(GetWidth() - m_noButton.length()) / 2;

    m_buttonPosY = GetHeight() - 1;
}

void YesNoMessageWindow::DrawContent() {
    MessageWindow::DrawContent();

    if (m_choice) {
        EnableAttribute(A_REVERSE);
        PrintStringAt(m_yesButtonPosX, m_buttonPosY, m_yesButton.c_str());
        DisableAttribute(A_REVERSE);
        PrintStringAt(m_noButtonPosX, m_buttonPosY, m_noButton.c_str());
    } else {
        PrintStringAt(m_yesButtonPosX, m_buttonPosY, m_yesButton.c_str());
        EnableAttribute(A_REVERSE);
        PrintStringAt(m_noButtonPosX, m_buttonPosY, m_noButton.c_str());
        DisableAttribute(A_REVERSE);
    }
}

void YesNoMessageWindow::HandleKey(int c) {
    switch (c) {
        case KEY_LEFT:
            m_choice = true;
            WindowManager::GetInstance()->RequestRefresh();
            break;
        case KEY_RIGHT:
            m_choice = false;
            WindowManager::GetInstance()->RequestRefresh();
            break;
        case 10:
            m_onClick(m_choice);
            WindowManager::GetInstance()->DeregisterWindow(this);
            break;
        default:
            break;
    }
}

}