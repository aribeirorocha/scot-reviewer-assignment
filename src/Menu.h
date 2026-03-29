/**
 * @file Menu.h
 * @brief Definição da classe Menu para a interface do utilizador.
 *
 * Gere a interação com o utilizador em modo interativo (menu no terminal)
 * ou em modo batch (linha de comandos).
 */
#ifndef MENU_H
#define MENU_H

#include <string>
#include "App.h"

/**
 * @class Menu
 * @brief Camada de apresentação do sistema SCOT.
 *
 * Recebe inputs do utilizador e delega a lógica de negócio à classe App.
 */
class Menu {
public:
    /**
     * @brief Construtor padrão da classe Menu.
     */
    Menu();

    /**
     * @brief Inicia o modo de menu interativo no terminal.
     * @details Complexidade Temporal: O(N * C), onde N é o número de interações.
     */
    void runInteractive();

    /**
     * @brief Executa o programa em modo batch (processamento automático).
     * @param inputFile  Caminho para o ficheiro CSV de entrada.
     * @param outputFile Caminho para o ficheiro de saída (sobrepõe o do CSV se fornecido).
     * @details Complexidade Temporal: O(L * C + V * E^2 + R * V * E^2).
     */
    void runBatch(const std::string& inputFile, const std::string& outputFile);

private:
    App app;

    /**
     * @brief Exibe o menu de opções no terminal.
     * @details Complexidade Temporal: O(1).
     */
    void displayOptions() const;

    /**
     * @brief Processa a escolha do utilizador.
     * @param choice Opção escolhida.
     * @return true para continuar, false para encerrar.
     * @details Complexidade Temporal: variável conforme a opção.
     */
    bool handleChoice(int choice);
};

#endif