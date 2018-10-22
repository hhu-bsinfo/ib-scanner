#ifndef IBPERFMON_WINDOW_H
#define IBPERFMON_WINDOW_H

#include <cstdint>
#include <ncurses.h>
#include <string>
#include <unordered_map>
#include <functional>

namespace CursesLib {

class WindowManager;

/**
 * Base class for Windows.
 *
 * Implementations MUST NOT perform any ncurses-calls outside of DrawContent()!
 * To show a window, call WindowManager::GetInstance()->RegisterWindow(window).
 * To hide a window, call WindowManager::GetInstance()->DeregisterWindow(window).
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
*/
class Window {

public:

    friend WindowManager;

    /**
     * Constructor.
     *
     * @param posX X-coordinate of upper left corner
     * @param posY Y-coordinate of upper left corner
     * @param width The width
     * @param height The height
     * @param title The title (shown at the window's top)
     */
    Window(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, const char *title);

    /**
     * Destructor.
     */
    virtual ~Window();

    /**
     * Get the x-coordinate of the upper left corner.
     */
    uint32_t GetPosX() const {
        return m_posX - 2;
    }

    /**
     * Get the y-coordinate of the upper left corner.
     */
    uint32_t GetPosY() const {
        return m_posY - 2;
    }

    /**
     * Get the width.
     */
    uint32_t GetWidth() const {
        return m_width - 2;
    }

    /**
     * Get the height.
     */
    uint32_t GetHeight() const {
        return m_height - 2;
    }

    /**
     * Set the tile.
     */
    void SetTitle(const char *title);

    /**
     * Move the window to another position.
     *
     * @param posX X-coordinate of upper left corner
     * @param posY Y-coordinate of upper left corner
     */
    void Move(uint32_t posX, uint32_t posY);

    /**
     * Changes the window's size.
     *
     * @param width The width
     * @param height The height
     */
    void Resize(uint32_t width, uint32_t height);

    /**
     * Add a handler function, that is called, when a specified key is pressed an the windows is focused.
     *
     * @param c The key
     * @param keyHandler The handler function
     */
    void AddKeyHandler(int c, std::function<void()> keyHandler);

    /**
     * Remove a handler function for a specified key.
     *
     * @param c The key
     */
    void RemoveKeyHandler(int c);

    /**
     * Process a key, which has been pressed by the user.
     * Implementations of Window must call this parent function,
     * or else the manually registered key handlers won't work!
     *
     * @param c The key
     */
    virtual void HandleKey(int c);

protected:

    /**
     * Enable an ncurses-attribute for this window.
     *
     * @param attr The attribute
     */
    void EnableAttribute(uint32_t attr);

    /**
     * Disable an ncurses-attribute for this window.
     *
     * @param attr The attribute
     */
    void DisableAttribute(uint32_t attr);

    /**
     * Print a char inside the window.
     *
     * @param x The char's x-coordinate
     * @param y The char's y-coordinate
     * @param c The char
     */
    void PrintCharAt(uint32_t x, uint32_t y, chtype c);

    /**
     * Print a formatted string inside the window.
     *
     * @param x The string's x-coordinate
     * @param y The string's y-coordinate
     * @param format The string
     */
    void PrintStringAt(uint32_t x, uint32_t y, const char *format...);

protected:

    /**
     * Draw the window's content.
     *
     * Implementations should always call the parent's function first, because it handles moving and resizing the
     * window, if necessary.
     * This is the only method in which ncurses-functions may be called!
     * It is not necessary to call refresh() or wrefresh()!
     */
    virtual void DrawContent();

protected:

    WINDOW *m_window;

    std::unordered_map<int, std::function<void()>> m_keyHandlerMap;

    uint32_t m_posX, m_posY;
    uint32_t m_width, m_height;

    bool m_posChanged;

    std::string m_title;
};

}

#endif
