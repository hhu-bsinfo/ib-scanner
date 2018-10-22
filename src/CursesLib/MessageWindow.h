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

#ifndef IBPERFMON_MSGBOX_H
#define IBPERFMON_MSGBOX_H

#include <string>
#include <ncurses.h>
#include <vector>
#include "Window.h"

namespace CursesLib {

/**
 * Window, which shows a multi-line text message.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class MessageWindow : public Window {

public:

    /**
     * Constructor.
     *
     * @param posX X-coordinate of upper left corner
     * @param posY Y-coordinate of upper left corner
     * @param title The window's title
     * @param message The message to be shown
     */
    MessageWindow(uint32_t posX, uint32_t posY, const char *title, const char *message);

    /**
     * Constructor.
     *
     * The window will be positioned in the middle of the screen.
     *
     * @param title The window's title
     * @param message The message to be shown
     */
    MessageWindow(const char *title, const char *message);

    /**
     * Desctructor.
     */
    ~MessageWindow() override = default;

    /**
     * Overriding function from Window.
     */
    void HandleKey(int c) override;

protected:

    /**
     * Overriding function from Window.
     */
    void DrawContent() override;

protected:

    uint32_t m_msgWidth;
    bool m_posChanged;

    std::vector<std::string> m_messageLines;
};

}

#endif
