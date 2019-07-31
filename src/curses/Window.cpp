#include <cstdlib>
#include "WindowManager.h"
#include "Window.h"

namespace Curses {

Window::Window(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, const char *title) :
        m_window(nullptr),
        m_posX(posX),
        m_posY(posY),
        m_width(width),
        m_height(height),
        m_posChanged(true) {
    m_title = title;

    if (m_width < m_title.length()) {
        m_width = static_cast<uint32_t>(m_title.length());
    }

    m_window = newwin(m_height, m_width, m_posY, m_posX);
}

Window::~Window() {
    delwin(m_window);
}

void Window::SetTitle(const char *title) {
    m_title = title;
}

void Window::Move(uint32_t posX, uint32_t posY) {
    m_posX = posX;
    m_posY = posY;

    m_posChanged = true;
}

void Window::Resize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;

    m_posChanged = true;
}

void Window::EnableAttribute(uint32_t attr) {
    wattron(m_window, attr);
}

void Window::DisableAttribute(uint32_t attr) {
    wattroff(m_window, attr);
}

void Window::PrintStringAt(uint32_t x, uint32_t y, const char *format...) {
    mvwprintw(m_window, y + 1, x + 1, format);
}

void Window::PrintCharAt(uint32_t x, uint32_t y, const chtype c) {
    wmove(m_window, y + 1, x + 1);
    waddch(m_window, c);
}

void Window::DrawContent() {
    if (m_posChanged) {
        wresize(m_window, m_height, m_width);
        mvwin(m_window, m_posY, m_posX);
        werase(m_window);

        m_posChanged = false;
    }
}

void Window::AddKeyHandler(int c, std::function<void()> keyHandler) {
    m_keyHandlerMap[c] = keyHandler;
}

void Window::RemoveKeyHandler(int c) {
    m_keyHandlerMap.erase(c);
}

void Window::HandleKey(int c) {
    if(m_keyHandlerMap.find(c) != m_keyHandlerMap.end()) {
        m_keyHandlerMap[c]();
    }
}

}