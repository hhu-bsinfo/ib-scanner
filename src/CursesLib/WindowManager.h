#ifndef PROJECT_WINDOWMANAGER_H
#define PROJECT_WINDOWMANAGER_H

#include <vector>
#include <thread>
#include <functional>
#include <atomic>
#include "Window.h"

namespace CursesLib {

/**
 * Manages the TUI, reads keyboard input and repaints all windows if necessary in a dedicated thread.
 *
 * The WindowManager is accessible via the Singleton-pattern. Call WindowManager::GetInstance() to get a pointer
 * to the active instance.
 *
 * Before doing anything with the WindowManager, you need to call WindowManager::GetInstance()->Initialize().
 *
 * To start the TUI, call WindowManager::GetInstance()->Start().
 *
 * All windows must be registered to be shown by calling WindowManager::GetInstance()->RegisterWindow(Window*).
 * It is possible to register windows before and after calling Start().
 *
 * To hide a window, call WindowManager::GetInstance()->DeregisterWindow(Window*).
 *
 * To get back to the normal console, call WindowManager::GetInstance()-> Stop().
 * Don't forget to deregister your windows before calling Stop()!
 *
 * The WindowManager also shows a function menu at the terminal's bottom line.
 * To register a function call WindowManager::GetInstance->AddMenuFunction(std::string, std::function).
 * The functions will use the F-keys consecutively (e.g. the first registered function will use F1, the second F2, etc.)
 * The maximum amount of registered functions is 12.
 *
 * @author Fabian Ruhland, Fabian.Ruhland@hhu.de
 * @date May 2018
 */
class WindowManager {

public:

    /**
     * Constructor.
     */
    WindowManager();

    /**
     * Destructor.
     */
    ~WindowManager() = default;

    /**
     * Get the active instance.
     */
    static WindowManager *GetInstance();

    /**
     * Initialize the WindowManager.
     */
    void Initialize();

    /**
     * Start the UI-thread.
     */
    void Start();

    /**
     * Stop the UI-thread and reset the WindowManager.
     */
    void Stop();

    /**
     * Register a window.
     *
     * The UI-thread will refresh all windows as soon as possible, in order for the newly registered window to be shown.
     *
     * @param window The window
     */
    void RegisterWindow(Window *window);

    /**
     * Deregister a window.
     *
     * The UI-thread will refresh all windows as soon as possible, in order for the deregistered window to disappear.
     *
     * @param window The window
     */
    void DeregisterWindow(Window *window);

    /**
     * Set the focus to a specified window.
     *
     * The window has to be registered beforehand.
     *
     * @param window The window
     */
    void SetFocus(Window *window);

    /**
     * Advises the UI-thread to refresh all windows as soon as possible.
     */
    void RequestRefresh();

    /**
     * Get the terminal's width.
     */
    uint32_t GetTerminalWidth() {
        return m_terminalWidth;
    }

    /**
     * Get the terminal's height.
     */
    uint32_t GetTerminalHeight() {
        return m_terminalHeight;
    }

    /**
     * Add a function to the function menu.
     *
     * The functions will use the F-keys consecutively(e.g. the first registered function will use F1,
     * the second F2, etc.). The maximum amount of registered function is 12.
     *
     * @param name The name to be shown in the function menu
     * @param function The function
     */
    void AddMenuFunction(std::string name, std::function<void()> function);

private:
    /**
     * Execute a function from the function menu.
     *
     * @param functionNumber Index of the function to be executed
     */
    void ExecuteMenuFunction(uint8_t functionNumber);

    /**
     * Refresh all windows.
     */
    void DrawWindows();

    /**
     * The UI-thread.
     */
    void Run();

private:

    uint32_t m_terminalWidth, m_terminalHeight;
    bool m_isRunning;

    std::atomic<bool> m_refresh;
    std::atomic<bool> m_erase;

    std::vector<std::pair<std::string, std::function<void()>>> m_menuFunctions;
    std::vector<Window *> m_windows;

    std::thread m_uiThread;

    static WindowManager *instance;
};

}

#endif
