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
#include "MenuItem.h"

namespace CursesLib {

MenuItem::MenuItem(const char *name, std::function<void()> onClick) :
        m_onClick(std::move(onClick)),
        m_isExpanded(false) {
    m_name = name;
}

void MenuItem::ToggleExpanded() {
    if (m_children.empty()) {
        m_isExpanded = false;
    } else {
        m_isExpanded = !m_isExpanded;
    }
}

void MenuItem::PerformClick() {
    m_onClick();
}

}