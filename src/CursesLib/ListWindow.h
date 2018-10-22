#ifndef IBPERFMON_LISTWINDOW_H
#define IBPERFMON_LISTWINDOW_H

#include <algorithm>
#include <vector>
#include "Window.h"

namespace CursesLib {

/**
 * A Window, which contains a scrollable list of strings.
 *
 * The list can be navigated through with the up and down arrow keys.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class ListWindow : public Window {

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
    ListWindow(uint32_t posX, uint32_t posY, uint32_t width, uint32_t height, const char *title);

    /**
     * Destructor.
     */
    ~ListWindow() override = default;

    /**
     * Add an item.
     */
    void AddItem(std::string item) {
        m_items.emplace_back(item);
    }

protected:

    /**
     * Overriding function from Window.
     */
    void HandleKey(int c) override;

    /**
     * Overriding function from Window.
     */
    void DrawContent() override;

protected:

    std::vector<std::string> m_items;

    uint32_t m_highlight;
    int32_t m_scrollOffset;
};

}

#endif
