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

#include "WindowManager.h"
#include "MenuWindow.h"

namespace CursesLib {

MenuWindow::MenuWindow(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, const char *title) :
        ListWindow(posX, posY, width, height, title) {

}

void MenuWindow::DrawContent() {
    uint32_t posY = 0;
    int32_t scrollOffsetBackup = m_scrollOffset;

    for (uint32_t i = 0; i < GetWidth(); i++) {
        for (uint32_t j = 0; j < GetHeight(); j++) {
            PrintCharAt(i, j, ' ');
        }
    }

    for (auto &m_item : m_items) {
        posY += PrintItem(m_item, 0, posY);
    }

    m_scrollOffset = scrollOffsetBackup;
}

void MenuWindow::HandleKey(int c) {
    uint32_t counter = 0;
    MenuItem *item = CalcSelectedItem(counter, m_items, m_highlight + m_scrollOffset);

    switch (c) {
        case KEY_UP :
            if (m_highlight > 0) {
                m_highlight--;
            } else if (m_scrollOffset > 0) {
                m_scrollOffset--;
            }
            break;
        case KEY_DOWN:
            if (m_highlight + m_scrollOffset < CalcMenuHeight(m_items) - 1) {
                if (m_highlight < GetHeight() - 1) {
                    m_highlight++;
                } else {
                    m_scrollOffset++;
                }
            }
            break;
        case KEY_RIGHT:
            if (!item->IsExpanded()) {
                item->ToggleExpanded();
            }
            break;
        case KEY_LEFT:
            if (item->IsExpanded()) {
                item->ToggleExpanded();
            }
            break;
        case KEY_ENTER:
        case 10:
        case ' ':
            item->PerformClick();
            break;
        default:
            break;
    }

    WindowManager::GetInstance()->RequestRefresh();
}

uint32_t MenuWindow::PrintItem(MenuItem &item, const uint32_t posX, uint32_t posY) {
    uint32_t ret = 0;

    if (posY > GetHeight() - 1) {
        return 0;
    }

    if (m_scrollOffset <= 0) {
        if (posX > 0) {
            for (uint32_t i = 0; i < posX; i++) {
                if ((i - 1) % 4 == 0) {
                    PrintCharAt(i, posY, ACS_VLINE);
                } else {
                    PrintCharAt(i, posY, ' ');
                }
            }
            PrintCharAt(posX - 3, posY, ACS_LTEE);
            PrintCharAt(posX - 2, posY, ACS_HLINE);
            PrintCharAt(posX - 1, posY, ACS_HLINE);
        }

        if (posY == m_highlight) {
            EnableAttribute(A_REVERSE);
        }

        if (item.GetChildren().empty()) {
            PrintStringAt(posX, posY, "[ ]");
        } else {
            PrintStringAt(posX, posY, item.IsExpanded() ? "[-]" : "[+]");
        }

        PrintStringAt(posX + 3, posY, item.GetName());
        DisableAttribute(A_REVERSE);

        ret++;
    }

    m_scrollOffset--;

    if (item.IsExpanded()) {
        for (MenuItem &subItem : item.GetChildren()) {
            uint32_t printed = PrintItem(subItem, posX + 4, ++posY);

            posY += printed - 1;
            ret += printed;
        }

        if (posY < GetHeight() - 1) {
            if (item.GetChildren().back().IsExpanded()) {
                PrintCharAt(posX + 1, posY - 1, ACS_LLCORNER);
                PrintCharAt(posX + 1, posY, ' ');
            } else {
                PrintCharAt(posX + 1, posY, ACS_LLCORNER);
            }
        }
    }

    return ret;
}

uint32_t MenuWindow::CalcMenuHeight(std::vector<MenuItem> &items) {
    uint32_t height = 0;

    for (auto &item : items) {
        height++;

        if (item.IsExpanded()) {
            height += CalcMenuHeight(item.GetChildren());
        }
    }

    return height;
}

MenuItem *MenuWindow::CalcSelectedItem(uint32_t &counter, std::vector<MenuItem> &items, uint32_t selection) {
    for (auto &item : items) {
        if (counter == selection) {
            return &item;
        }

        counter++;

        if (item.IsExpanded()) {
            MenuItem *tmp = CalcSelectedItem(counter, item.GetChildren(), selection);

            if (tmp != nullptr) {
                return tmp;
            }
        }
    }

    return nullptr;
}

}