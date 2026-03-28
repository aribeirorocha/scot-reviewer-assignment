/**
 * @file Menu.h
 * @brief Definição da classe Menu para a interface do utilizador.
 * * Este ficheiro contém a interface para a classe Menu, que gere a interação
 * com o utilizador, seja através de um menu interativo no terminal ou 
 * através de comandos em modo batch (linha de comandos).
 */
#ifndef MENU_H
#define MENU_H

#include <string>
#include "Parser.h"
#include "App.h"

/**
 * @class Menu
 * @brief Classe responsável pela interface de utilizador do sistema SCOT.
 * * Atua como a camada de apresentação, recebendo inputs do utilizador e
 * comunicando com a classe App para executar a lógica de negócio.
 */

class Menu {
    public:
    /**
     * @brief Construtor padrão da classe Menu.
     */
    Menu();
    /**
     * @brief Inicia o modo de menu interativo no terminal.
     * * Permite ao utilizador navegar pelas funcionalidades do programa
     * através de uma estrutura de escolhas numeradas.
     * @details Complexidade Temporal: O(N), onde N é o número de interações.
     */
    void runInteractive ();

    /**
     * @brief Executa o programa em modo batch (processamento automático).
     * * @param inputFile Caminho para o ficheiro CSV de entrada.
     * @param riskFile Caminho para o ficheiro de saída da análise de risco (opcional).
     * @details Complexidade Temporal: O(L + R + S), onde L é o tamanho do ficheiro,
     * R o número de revisores e S o número de submissões.
     */
    void runBatch(const std::string &inputFile, const std::string &riskFile);

    private:
    /** @brief Instância da classe App que gere os dados e o grafo. */
    App app;
    /**
     * @brief Exibe o menu de opções textuais no terminal.
     * @details Complexidade Temporal: O(1).
     */
    void displayOptions() const;
    /**
     * @brief Processa a escolha do utilizador e invoca a funcionalidade correspondente.
     * * @param choice Inteiro que representa a opção escolhida pelo utilizador.
     * @return true se o programa deve continuar a correr, false se deve encerrar.
     * @details Complexidade Temporal: Variável consoante a opção (ver Menu.cpp).
     */
    bool handleChoice(int choice);
};
#endif