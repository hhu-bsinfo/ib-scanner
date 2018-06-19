#include <algorithm>
#include <clocale>
#include <ncurses.h>
#include "WindowManager.h"

namespace CursesLib {

WindowManager *WindowManager::instance = nullptr;

WindowManager::WindowManager() :
        m_terminalWidth(0),
        m_terminalHeight(0),
        m_isRunning(false),
        m_refresh(false),
        m_erase(false) {

}

WindowManager *WindowManager::GetInstance() {
    if (instance == nullptr) {
        instance = new WindowManager();
    }

    return instance;
}

void WindowManager::Initialize() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    timeout(0);
    setlocale(LC_ALL, "");
    fwide(stdout, 1);

    getmaxyx(stdscr, m_terminalHeight, m_terminalWidth);
}

void WindowManager::Start() {
    m_isRunning = true;

    m_uiThread = std::thread(&WindowManager::Run, this);
}

void WindowManager::Stop() {
    m_isRunning = false;

    m_uiThread.join();

    endwin();

    // Restore buffering behaviour of std-streams
    setvbuf(stdout, nullptr, _IOLBF, 0);
    setvbuf(stderr, nullptr, _IONBF, 0);
}

void WindowManager::Run() {
    DrawWindows();

    while (m_isRunning) {
        if (m_refresh) {
            m_refresh = false;
            DrawWindows();
        }

        uint32_t width, height;
        getmaxyx(stdscr, height, width);

        if(m_terminalWidth != width || m_terminalHeight != height) {
            m_terminalWidth = width;
            m_terminalHeight = height;

            m_erase = true;
            m_refresh = true;
        }

        int c = getch();

        if (c == ERR) {
            continue;
        } else if (c >= KEY_F(1) && c <= KEY_F(12)) {
            ExecuteMenuFunction(static_cast<uint8_t>(c - KEY_F(1)));
        } else if (c == 9) {
            if (!m_windows.empty()) {
                Window *lastWindow = m_windows.back();
                m_windows.pop_back();
                m_windows.insert(m_windows.begin(), lastWindow);

                DrawWindows();
            }
        } else {
            if (!m_windows.empty()) {
                m_windows.back()->HandleKey(c);
            }
        }
    }
}

void WindowManager::RegisterWindow(Window *window) {
    if (std::find(m_windows.begin(), m_windows.end(), window) == m_windows.end()) {
        m_windows.emplace_back(window);

        RequestRefresh();
    }
}

void WindowManager::DeregisterWindow(Window *window) {
    if (std::find(m_windows.begin(), m_windows.end(), window) != m_windows.end()) {
        m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), window), m_windows.end());

        m_erase = true;

        RequestRefresh();
    }
}

void WindowManager::SetFocus(Window *window) {
    if (std::find(m_windows.begin(), m_windows.end(), window) != m_windows.end()) {
        m_windows.erase(std::remove(m_windows.begin(), m_windows.end(), window), m_windows.end());

        m_windows.emplace_back(window);

        RequestRefresh();
    }
}

void WindowManager::RequestRefresh() {
    m_refresh = true;
}

void WindowManager::AddMenuFunction(std::string name, std::function<void()> function) {
    if (m_menuFunctions.size() < 12) {
        m_menuFunctions.emplace_back(std::pair<std::string, std::function<void()>>(name, function));
    }

    RequestRefresh();
}

void WindowManager::ExecuteMenuFunction(uint8_t functionNumber) {
    if (functionNumber < m_menuFunctions.size()) {
        m_menuFunctions[functionNumber].second();
    }
}

void WindowManager::DrawWindows() {
    if (m_erase) {
        m_erase = false;
        erase();
    }

    for (Window *window : m_windows) {
        window->DrawContent();

        if (m_windows.back() == window) {
            wborder(window->m_window, 0, 0, 0, 0, '+', '+', '+', '+');
        } else {
            wborder(window->m_window, 0, 0, 0, 0, 0, 0, 0, 0);
        }

        window->EnableAttribute(A_BOLD);
        mvwprintw(window->m_window, 0, static_cast<uint32_t>((window->m_width - window->m_title.length()) / 2),
                  window->m_title.c_str());
        window->DisableAttribute(A_BOLD);
    }

    refresh();

    for (Window *window : m_windows) {
        wrefresh(window->m_window);
    }

    uint32_t posX = 0;

    for (uint32_t i = 0; i < m_menuFunctions.size(); i++) {
        mvprintw(m_terminalHeight - 1, posX, "F%d", i + 1);
        posX += i < 10 ? 2 : 3;

        attron(A_REVERSE);
        mvprintw(m_terminalHeight - 1, posX, m_menuFunctions[i].first.c_str());
        posX += m_menuFunctions[i].first.length();
        attroff(A_REVERSE);
    }

    if (!m_menuFunctions.empty()) {
        attron(A_REVERSE);
        for (; posX < m_terminalWidth; posX++) {
            mvprintw(m_terminalHeight - 1, posX, " ");
        }
        attroff(A_REVERSE);
    }

    refresh();
}

}