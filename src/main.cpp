/**
 * @file main.cpp
 * @brief Ponto de entrada da aplicação Scientific Conference Organization Tool.
 * * Este ficheiro contém a função main que gere o arranque do programa, 
 * permitindo a execução em modo interativo (via menu) ou modo batch 
 * (via linha de comandos com argumentos).
 */
#include <iostream>
#include <string>
#include "Menu.h"

/**
 * @brief Função principal do programa.
 * Analisa os argumentos da linha de comandos para decidir o modo de execução.
 * @param argc Contador de argumentos da linha de comandos.
 * @param argv Vetor de strings contendo os argumentos da linha de comandos.
 * @return int Retorna 0 em caso de sucesso e 1 em caso de erro de argumentos.
 * @details Complexidade Temporal: O(1) para a verificação de argumentos, 
 * seguida da complexidade do modo de execução escolhido.
 */

int main(int argc, char* argv[]) {
    Menu menu;

    // Verificar se existem argumentos suficientes para o modo batch
    if (argc >= 3 && std::string(argv[1]) == "-b") {
        std::string inputFile = argv[2];
        std::string riskFile = "";

        // Se houver um 4º argumento, é o ficheiro de risco
        if (argc == 4) {
            riskFile = argv[3];
        } else if (argc > 4) {
            std::cerr << "[Erro] Demasiados argumentos. Uso correto: myProg -b input.csv [risk.csv]" << std::endl;
            return 1;
        }

        // Executar em modo batch
        menu.runBatch(inputFile, riskFile);
    } 
    // Se não houver a flag '-b' mas houverem argumentos, é um erro de sintaxe
    else if (argc > 1) {
        std::cerr << "[Erro] Argumentos invalidos. Uso correto para batch: myProg -b input.csv [risk.csv]" << std::endl;
        std::cerr << "Para modo interativo, execute sem argumentos." << std::endl;
        return 1;
    } 
    // Caso contrário, executar o modo interativo normal
    else {
        menu.runInteractive();
    }

    return 0;
}