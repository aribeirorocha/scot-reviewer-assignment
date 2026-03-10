#ifndef APP_H
#define APP_H

#include "Graph.h"
#include "Parser.h"
#include "Models.h"

class App {
public:
    App() = default;
    
    // Carrega o ficheiro e constrói o Grafo inicial
    bool loadData(const std::string& filename);
    
    // Getters para o Menu consultar
    const Parser& getParser() const { return parser; }
    const Graph<int>& getGraph() const { return masterGraph; }

private:
    Parser parser;
    Graph<int> masterGraph;

    // Constrói a estrutura do grafo com base nos dados lidos
    void buildInitialGraph();
};

#endif