#ifndef MENU_H
#define MENU_H

#include <string>

class Menu {
    public:
    Menu();
    void runInteractive ();
    void runBatch (const std::string& inputFile, const std::string& riskFile);

    private:
    void displayOptions() const;
    bool handleChoice(int choice);
};
#endif