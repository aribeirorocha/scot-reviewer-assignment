#ifndef MENU_H
#define MENU_H

#include <string>
#include "Parser.h"
#include "App.h"

class Menu {
    public:
    Menu();
    void runInteractive ();
    void runBatch (const std::string& inputFile, const std::string& riskFile);

    private:
    App app;
    void displayOptions() const;
    bool handleChoice(int choice);
};
#endif