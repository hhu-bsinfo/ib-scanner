#ifndef PROJECT_YESNOMESSAGEWINDOW_H
#define PROJECT_YESNOMESSAGEWINDOW_H

#include <functional>
#include "MessageWindow.h"

namespace CursesLib {

/**
 * MessageWindow with an Yes- and a No-Button.
 *
 * The user can choose 'Yes' or 'No' by using the left and right arrow keys.
 * When the user presses enter, the chosen button is triggered, a callback-function is called and the window is closed.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class YesNoMessageWindow : public MessageWindow {

public:

    /**
     * Constructor.
     *
     * @param posX X-coordinate of upper left corner
     * @param posY Y-coordinate of upper left corner
     * @param title The window's title
     * @param message The message to be shown
     * @param onClick The callback-function, which is called, after a button has been pressed.
     */
    YesNoMessageWindow(uint32_t posX, uint32_t posY, const char *title, const char *message,
                       std::function<void(bool)> onClick);

    /**
     * Constructor.
     *
     * @param title The window's title
     * @param message The message to be shown
     * @param onClick The callback-function, which is called, after the button has been pressed.
     */
    YesNoMessageWindow(const char *title, const char *message, std::function<void(bool)> onClick);

    /**
     * Destructor.
     */
    ~YesNoMessageWindow() override = default;

private:

    /**
     * Overriding function from Window.
     */
    void DrawContent() override;

    /**
     * Overriding function from Window.
     */
    void HandleKey(int c) override;

private:

    uint32_t m_yesButtonPosX, m_noButtonPosX, m_buttonPosY;

    bool m_choice;

    std::string m_yesButton = " YES ";
    std::string m_noButton = " NO ";

    std::function<void(bool)> m_onClick;
};

}

#endif