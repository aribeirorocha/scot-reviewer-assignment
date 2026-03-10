#include "Menu.h"
#include <iostream>
#include <limits>
Menu::Menu() { }

void Menu::runInteractive(){
    bool running = true;
    while (running){
        displayOptions();
        int choice;
        std::cin >> choice;
        // Tratamento de erros: se o utilizador inserir um caracter em vez de número
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[ERRO] Invalid input. Por favor, insira um número." << std::endl;
            continue;
        }
        running = handleChoice(choice);
    }
}

void Menu::displayOptions() const {
    std::cout << "\n=========================================\n";
    std::cout << "  Scientific Conference Organization Tool  \n";
    std::cout << "=========================================\n";
    std::cout << "1. Carregar Dados (Parse CSV)\n";
    std::cout << "2. Ver Submissoes e Revisores\n";
    std::cout << "3. Gerar Atribuicoes de Revisao (Max Flow)\n";
    std::cout << "4. Analise de Risco\n";
    std::cout << "0. Sair\n";
    std::cout << "Escolha uma opcao: ";
}

bool Menu::handleChoice(int choice) {
    switch (choice) {
        case 1:
            std::cout << "[Info] A carregar dados... (Por implementar T1.2)\n";
            break;
        case 2:
            std::cout << "[Info] A mostrar dados... (Por implementar T1.2)\n";
            break;
        case 3:
            std::cout << "[Info] A calcular Max Flow... (Por implementar T2.1)\n";

            break;
        case 4:
            std::cout << "[Info] A executar Analise de Risco... (Por implementar T2.2+)\n";
            break;
        case 0:
            std::cout << "A encerrar o programa!\n";
            return false;
        default:
            std::cerr << "[Erro] Opcao invalida. Escolha um numero entre 0 e 4.\n";
            break;
    }
    return true;
}

void Menu::runBatch(const std::string& inputFile, const std::string& riskFile) {
    std::cout << "[Batch Mode] A iniciar processamento...\n";
    std::cout << "Ficheiro de Input: " << inputFile << "\n";
    
    if (!riskFile.empty()) {
        std::cout << "Ficheiro de Output de Risco: " << riskFile << "\n";
    }


    std::cout << "[Batch Mode] Processamento concluido.\n";
}