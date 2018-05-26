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

#ifndef PROJECT_MENULIST_H
#define PROJECT_MENULIST_H

#include <vector>
#include <ncurses.h>
#include "Window.h"
#include "MenuItem.h"
#include "ListWindow.h"

namespace CursesLib {

/**
 * A ListWindow, which contains a scrollable list of item, that can have subitems.
 *
 * The list can be navigated through with the up and down arrow keys.
 * Items can be expanded/collapsed by using the left/right keys.
 * An item can be chosen by pressing the Enter key. This will trigger a callback function.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class MenuWindow : public ListWindow {

public:

    /**
     * Constructor.
     *
     * @param posX X-coordinate of upper left corner
     * @param posY Y-coordinate of upper left corner
     * @param width The width
     * @param height The height
     * @param title The title (shown at the window's top)
     */
    MenuWindow(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, const char *title);

    /**
     * Destructor.
     */
    ~MenuWindow() override = default;

    /**
     * Add an item.
     */
    void AddItem(MenuItem item) {
        m_items.emplace_back(item);
    }

protected:

    /**
     * Overriding function from window.
     */
    void DrawContent() override;

    /**
     * Overriding function from window.
     */
    void HandleKey(int c) override;

private:

    /**
     * Print an item and its subitems, if it is expanded.
     *
     * @param item The item
     * @param posX The x-coordinate inside the window
     * @param posY The y-coordinate inside the window
     * @return
     */
    uint32_t PrintItem(MenuItem &item, uint32_t posX, uint32_t posY);

    /**
     * Calculate the menu's height at its current state (regarding expanded/collapsed items).
     *
     * @param items The menu items
     *
     * @return The calculated height
     */
    uint32_t CalcMenuHeight(std::vector<MenuItem> &items);

    /**
     * Calculate which item is currently selected.
     *
     * @param counter Internal use; Should be set to 0 befor the first call
     * @param items The menu items
     * @param selection The highlighted position
     *
     * @return The calculated item
     */
    MenuItem *CalcSelectedItem(uint32_t &counter, std::vector<MenuItem> &items, uint32_t selection);

private:

    std::vector<MenuItem> m_items;
};

}

#endif
