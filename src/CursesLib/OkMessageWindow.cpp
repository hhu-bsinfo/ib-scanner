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
#include "OkMessageWindow.h"

namespace CursesLib {

OkMessageWindow::OkMessageWindow(uint32_t posX, uint32_t posY, const char *title, const char *message,
                                 std::function<void()> onClick) :
        MessageWindow(posX, posY, title, message),
        m_buttonPosX(0),
        m_buttonPosY(0),
        m_onClick(std::move(onClick)) {
    m_buttonPosX = static_cast<uint32_t >((GetWidth() - m_button.length() > 0) ?
                                          (GetWidth() - m_button.length()) / 2 : 0);
    m_buttonPosY = GetHeight() - 1;
}

OkMessageWindow::OkMessageWindow(const char *title, const char *message, std::function<void()> onClick) :
        MessageWindow(title, message),
        m_buttonPosX(0),
        m_buttonPosY(0),
        m_onClick(std::move(onClick)) {
    m_buttonPosX = static_cast<uint32_t >((GetWidth() - m_button.length() > 0) ?
                                          (GetWidth() - m_button.length()) / 2 : 0);
    m_buttonPosY = GetHeight() - 1;
}

void OkMessageWindow::DrawContent() {
    MessageWindow::DrawContent();

    EnableAttribute(A_REVERSE);
    PrintStringAt(m_buttonPosX, m_buttonPosY, m_button.c_str());
    DisableAttribute(A_REVERSE);
}

void OkMessageWindow::HandleKey(int c) {
    Window::HandleKey(c);

    if (c == 10) {
        m_onClick();
        WindowManager::GetInstance()->DeregisterWindow(this);
    }
}

}