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

#ifndef IBPERFMON_MENUITEM_H
#define IBPERFMON_MENUITEM_H

#include <string>
#include <vector>
#include <functional>

namespace CursesLib {

/**
 * An item used in MenuWindows.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class MenuItem {

public:

    /**
     * Constructor.
     *
     * @param name The name
     * @param onClick The callback-function, which is called, after the item has been selected
     * @param data Arbitrary data, that can be associated with this menu item
     */
    explicit MenuItem(std::string name, std::function<void()> onClick, void *data = nullptr);

    /**
     * Destructor.
     */
    ~MenuItem() = default;

    /**
     * Get the name.
     */
    const char *GetName() const {
        return m_name.c_str();
    }

    /**
     * Set the name.
     */
    void SetName(const char *name) {
        m_name = name;
    }

    /**
     * Set the data.
     */
    void SetData(void *data) {
        m_data = data;
    }

    /**
     * Get the data.
     */
    void* GetData() {
        return m_data;
    }

    /**
     * Get the subitems.
     */
    std::vector<MenuItem> &GetChildren() {
        return m_children;
    }

    /**
     * Add a subitem.
     */
    void AddSubitem(MenuItem item) {
        m_children.emplace_back(item);
    }

    /**
     * Check if the item is expanded or collapsed.
     */
    bool IsExpanded() const {
        return m_isExpanded;
    }

    /**
     * Expand/collapse the item.
     */
    void ToggleExpanded();

    /**
     * Call the callback function.
     */
    void PerformClick();

protected:

    std::string m_name;

    std::vector<MenuItem> m_children;

    std::function<void()> m_onClick;

    void *m_data;

    bool m_isExpanded;
};

}

#endif
