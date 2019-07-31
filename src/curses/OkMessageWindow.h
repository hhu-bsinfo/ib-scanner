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

#ifndef IBSCANNER_OKMSGBOX_H
#define IBSCANNER_OKMSGBOX_H

#include <functional>
#include "MessageWindow.h"

namespace Curses {

/**
 * MessageWindow with an OK-Button.
 *
 * When the user presses enter, the button is triggered, a callback-function is called and the window is closed.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class OkMessageWindow : public MessageWindow {

public:

    /**
     * Constructor.
     *
     * @param posX X-coordinate of upper left corner
     * @param posY Y-coordinate of upper left corner
     * @param title The window's title
     * @param message The message to be shown
     * @param onClick The callback-function, which is called, after the button has been pressed.
     */
    OkMessageWindow(uint32_t posX, uint32_t posY, const char *title, const char *message,
                    std::function<void()> onClick);

    /**
     * Constructor.
     *
     * @param title The window's title
     * @param message The message to be shown
     * @param onClick The callback-function, which is called, after the button has been pressed.
     */
    OkMessageWindow(const char *title, const char *message, std::function<void()> onClick);

    /**
     * Destructor.
     */
    ~OkMessageWindow() override = default;

protected:

    /**
     * Overriding function from Window.
     */
    void DrawContent() override;

    /**
     * Overriding function from Window.
     */
    void HandleKey(int c) override;

private:

    uint32_t m_buttonPosX, m_buttonPosY;

    std::string m_button = " OK ";

    std::function<void()> m_onClick;
};

}

#endif
