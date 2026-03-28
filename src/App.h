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

struct Assignment {
    int submissionId;
    int reviewerId;
    int domain;
};

class App {
public:
    App() = default;

    bool loadData(const std::string& filename);
    void solveT21();
    void SolveRisk1();
    void writeOutput(const std::string& filename,
                     const std::vector<Assignment>& assignments);

    const Parser& getParser() const { return parser; }
    const Graph<int>& getGraph() const { return masterGraph; }

private:
    Parser parser;
    Graph<int> masterGraph;

    void buildInitialGraph();
    double runFlow(std::vector<int> excludeIds = {},
                   std::vector<Assignment>* outAssignments = nullptr);
};

#endif