/**
 * @file App.h
 * @brief Definição da classe App, que atua como o controlador principal do sistema.
 */

#ifndef APP_H
#define APP_H

#include <string>
#include <vector>
#include "Graph.h"
#include "Parser.h"
#include "Models.h"

/**
 * @struct Assignment
 * @brief Representa uma atribuição entre uma submissão e um revisor.
 */
struct Assignment {
    int submissionId;
    int reviewerId;
    int domain; ///< domínio em que o match ocorreu (pode ser primário ou secundário)
};

/**
 * @class App
 * @brief Controlador principal: gere dados, grafo e algoritmos.
 */
class App {
public:
    App() = default;

    /**
     * @brief Carrega e valida o ficheiro CSV, constrói o grafo inicial.
     * @param filename Caminho do ficheiro CSV.
     * @return true se carregado com sucesso.
     */
    bool loadData(const std::string& filename);

    /**
     * @brief Executa toda a lógica conforme os parâmetros do CSV (usado em batch e auto).
     *        Corre o Max Flow e, se configurado, a análise de risco.
     *        Escreve o output para o ficheiro definido em OutputFileName.
     */
    void solveAutomatic(const std::string& overrideOutput = "");

    /**
     * @brief Executa a atribuição de revisões (T2.1) manualmente (modo interativo).
     */
    void solveT21();

    /**
     * @brief Executa a análise de risco = 1 manualmente (modo interativo).
     */
    void solveRisk1();

    const Parser& getParser() const { return parser; }
    const Graph<int>& getGraph() const { return masterGraph; }

private:
    Parser parser;
    Graph<int> masterGraph;

    /**
     * @brief Constrói o grafo inicial com os vértices de submissões e revisores.
     * @details Complexidade Temporal: O(S + R).
     */
    void buildInitialGraph();

    /**
     * @brief Constrói o grafo de fluxo e corre Edmonds-Karp.
     *
     * @param excludeIds Revisores a excluir (para análise de risco).
     * @param outAssignments Se não nulo, preenche com as atribuições extraídas.
     * @param mode GenerateAssignments mode (1=só primários, 2=sub primário+secundário vs rev primário, 3=todos).
     * @return Valor do fluxo máximo obtido.
     * @details Complexidade Temporal: O(V * E^2).
     */
    double runFlow(const std::vector<int>& excludeIds = {},
                   std::vector<Assignment>* outAssignments = nullptr,
                   int mode = 1);

    /**
     * @brief Escreve o ficheiro de output com as atribuições e, se existirem, revisões em falta.
     * @param filename Caminho do ficheiro de saída.
     * @param assignments Lista de atribuições feitas.
     * @details Complexidade Temporal: O(A log A + S).
     */
    void writeOutput(const std::string& filename,
                     const std::vector<Assignment>& assignments);

    /**
     * @brief Anexa a secção de Risk Analysis ao ficheiro de saída.
     * @param filename Caminho do ficheiro de saída.
     * @param riskyReviewers Lista ordenada de revisores críticos (pode estar vazia).
     * @param riskLevel Valor do parâmetro RiskAnalysis usado.
     * @details Complexidade Temporal: O(R).
     */
    void writeRiskOutput(const std::string& filename,
                         const std::vector<int>& riskyReviewers,
                         int riskLevel);
};

#endif