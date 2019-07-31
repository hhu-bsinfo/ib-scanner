#include <csignal>
#include <curses/WindowManager.h>
#include <curses/OkMessageWindow.h>
#include <curses/ListWindow.h>
#include <curses/MenuWindow.h>

static bool isRunning = true;

void sigfunc(int sig) {
    if(sig == SIGINT) {
        isRunning = false;
    }
}

int main(int argc, char **argv) {
    Curses::WindowManager* manager = Curses::WindowManager::GetInstance();
    
    signal(SIGINT, sigfunc);

    // Always call initialize first, before doing anything else GUI-related!
    manager->Initialize();

    Curses::OkMessageWindow helpWindow("Help", "Window Test\n"
                               "A small test program for curses\n"
                               "Copyright (C) 2018 Heinrich-Heine-Universitaet Duesseldorf,\n"
                               "Institute of Computer Science, Department Operating Systems\n"
                               "Licensed under GPL v3\n\n"
                               "Up/Down: Navigate menu\n"
                               "Right/Left: Open/Close menu entry\n"
                               "Tab: Switch window", []{});

    Curses::ListWindow listWindow(6, 6, 25, 8, "Simple List");

    Curses::MenuWindow menuWindow(20, 8, 40, 15, "Menu");

    for(uint32_t i = 0; i < 10; i++) {
        listWindow.AddItem("Item " + std::to_string(i));
    }

    for(uint32_t i = 0; i < 6; i++) {
        std::string optionName = "Option " + std::to_string(i);

        Curses::MenuItem item(optionName.c_str(), []{});
        Curses::MenuItem subItem1("Suboption 1", []{});
        Curses::MenuItem subItem2("Suboption 2", []{});
        subItem1.AddSubitem(Curses::MenuItem("Subsuboption 1", [] {}));
        subItem2.AddSubitem(Curses::MenuItem("Subsuboption 1", [] {}));
        item.AddSubitem(subItem1);
        item.AddSubitem(subItem2);

        menuWindow.AddItem(item);
    }

    Curses::MessageWindow messageWindow1(70, 2, "Test Message 1", "Simple Message");
    Curses::MessageWindow messageWindow2(75, 5, "Test Message 2", "Simple Message");
    Curses::MessageWindow messageWindow3(80, 8, "Test Message 3", "Simple Message");
    Curses::MessageWindow messageWindow4(85, 11, "Test Message 4", "Simple Message");
    Curses::MessageWindow messageWindow5(90, 14, "Test Message 5", "Simple Message");

    manager->AddMenuFunction("Help", [&]{ manager->RegisterWindow(&helpWindow); });
    manager->AddMenuFunction("Exit", [&]{ isRunning = false; });

    manager->Start();

    // Show windows
    manager->RegisterWindow(&messageWindow5);
    manager->RegisterWindow(&messageWindow4);
    manager->RegisterWindow(&messageWindow3);
    manager->RegisterWindow(&messageWindow2);
    manager->RegisterWindow(&messageWindow1);
    manager->RegisterWindow(&menuWindow);
    manager->RegisterWindow(&listWindow);

    while(isRunning);

    // Close Windows
    manager->DeregisterWindow(&messageWindow5);
    manager->DeregisterWindow(&messageWindow4);
    manager->DeregisterWindow(&messageWindow3);
    manager->DeregisterWindow(&messageWindow2);
    manager->DeregisterWindow(&messageWindow1);
    manager->DeregisterWindow(&menuWindow);
    manager->DeregisterWindow(&listWindow);

    // Back to normal console
    manager->Stop();
}