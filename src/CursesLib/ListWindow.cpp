#include "ListWindow.h"
#include "WindowManager.h"

namespace CursesLib {

ListWindow::ListWindow(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, const char *title) :
        Window(posX, posY, width, height, title),
        m_highlight(0),
        m_scrollOffset(0) {

}

void ListWindow::DrawContent() {
    for (uint32_t i = 0; i < GetWidth(); i++) {
        for (uint32_t j = 0; j < GetHeight(); j++) {
            PrintCharAt(i, j, ' ');
        }
    }

    for (uint32_t i = 0; i < m_items.size() && i < GetHeight(); i++) {
        if (i == m_highlight) {
            EnableAttribute(A_REVERSE);
            PrintStringAt(0, i, m_items[i + m_scrollOffset].c_str());
            DisableAttribute(A_REVERSE);
        } else {
            PrintStringAt(0, i, m_items[i + m_scrollOffset].c_str());
        }
    }
}

void ListWindow::HandleKey(int c) {
    switch (c) {
        case KEY_UP :
            if (m_highlight > 0) {
                m_highlight--;
            } else if (m_scrollOffset > 0) {
                m_scrollOffset--;
            }
            break;
        case KEY_DOWN:
            if (m_highlight + m_scrollOffset < m_items.size() - 1) {
                if (m_highlight < GetHeight() - 1) {
                    m_highlight++;
                } else {
                    m_scrollOffset++;
                }
            }
            break;
        default:
            break;
    }

    WindowManager::GetInstance()->RequestRefresh();
}

}