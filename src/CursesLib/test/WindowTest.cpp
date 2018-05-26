#include <csignal>
#include <CursesLib/WindowManager.h>
#include <CursesLib/OkMessageWindow.h>
#include <CursesLib/ListWindow.h>
#include <CursesLib/MenuWindow.h>

static bool isRunning = true;

void sigfunc(int sig) {
    if(sig == SIGINT) {
        isRunning = false;
    }
}

int main(int argc, char **argv) {
    CursesLib::WindowManager* manager = CursesLib::WindowManager::GetInstance();
    
    signal(SIGINT, sigfunc);

    // Always call initialize first, before doing anything else GUI-related!
    manager->Initialize();

    CursesLib::OkMessageWindow helpWindow("Help", "Window Test\n"
                               "A small test program for CursesLib\n"
                               "Copyright (C) 2018 Heinrich-Heine-Universitaet Duesseldorf,\n"
                               "Institute of Computer Science, Department Operating Systems\n"
                               "Licensed under GPL v3\n\n"
                               "Up/Down: Navigate menu\n"
                               "Right/Left: Open/Close menu entry\n"
                               "Tab: Switch window", []{});

    CursesLib::ListWindow listWindow(6, 6, 25, 8, "Simple List");

    CursesLib::MenuWindow menuWindow(20, 8, 40, 15, "Menu");

    for(uint32_t i = 0; i < 10; i++) {
        listWindow.AddItem("Item " + std::to_string(i));
    }

    for(uint32_t i = 0; i < 6; i++) {
        std::string optionName = "Option " + std::to_string(i);

        CursesLib::MenuItem item(optionName.c_str(), []{});
        CursesLib::MenuItem subItem1("Suboption 1", []{});
        CursesLib::MenuItem subItem2("Suboption 2", []{});
        subItem1.AddSubitem(CursesLib::MenuItem("Subsuboption 1", [] {}));
        subItem2.AddSubitem(CursesLib::MenuItem("Subsuboption 1", [] {}));
        item.AddSubitem(subItem1);
        item.AddSubitem(subItem2);

        menuWindow.AddItem(item);
    }

    CursesLib::MessageWindow messageWindow1(70, 2, "Test Message 1", "Simple Message");
    CursesLib::MessageWindow messageWindow2(75, 5, "Test Message 2", "Simple Message");
    CursesLib::MessageWindow messageWindow3(80, 8, "Test Message 3", "Simple Message");
    CursesLib::MessageWindow messageWindow4(85, 11, "Test Message 4", "Simple Message");
    CursesLib::MessageWindow messageWindow5(90, 14, "Test Message 5", "Simple Message");

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