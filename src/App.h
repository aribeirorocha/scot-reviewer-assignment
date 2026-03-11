/**
 * @file App.h
 * @brief Definição da classe App, que atua como o controlador principal do sistema.
 * * Esta classe é responsável por integrar o Parser com a estrutura de dados de Grafo,
 * gerindo o fluxo de informação desde a leitura do ficheiro até à representação em memória.
 */

#ifndef APP_H
#define APP_H

#include "Graph.h"
#include "Parser.h"
#include "Models.h"

/**
 * @class App
 * @brief Classe central que gere a lógica de negócio e as estruturas de dados do projeto.
 */

class App {
public:
    /**
     * @brief Construtor padrão da classe App.
     */
    App() = default;

    /**
     * @brief Carrega os dados de um ficheiro CSV e desencadeia a construção do grafo.
     * * @param filename Caminho relativo ou absoluto para o ficheiro .csv de entrada.
     * @return true se o ficheiro foi lido e os dados carregados com sucesso, false caso contrário.
     * * @details Complexidade Temporal: O(N * M + S + R), onde N é o número de linhas, 
     * M o comprimento das linhas, S o número de submissões e R o de revisores.
     */
    
    void solveT21();
    // Carrega o ficheiro e constrói o Grafo inicial
    bool loadData(const std::string& filename);

    /**
     * @brief Obtém uma referência constante para o objeto Parser.
     * @return Const referência para o Parser.
     * @details Complexidade Temporal: O(1).
     */
    
    // Getters para o Menu consultar
    const Parser& getParser() const { return parser; }

    /**
     * @brief Obtém uma referência constante para o Grafo master.
     * @return Const referência para o objeto Graph<int>.
     * @details Complexidade Temporal: O(1).
     */

    const Graph<int>& getGraph() const { return masterGraph; }
    

private:
    /** @brief Instância do Parser responsável pela leitura dos ficheiros. */
    Parser parser;

    /** @brief Estrutura de Grafo que armazena a relação entre Submissões e Revisores. */
    Graph<int> masterGraph;
    
    /**
     * @brief Constrói a topologia inicial do grafo (vértices) com base nos dados lidos pelo parser.
     * * @details Complexidade Temporal: O(S + R), onde S é o número de submissões e R o número de revisores.
     * Esta função popula o masterGraph criando um vértice para cada ID único encontrado.
     */

    // Constrói a estrutura do grafo com base nos dados lidos
    void buildInitialGraph();
    
};

#endif