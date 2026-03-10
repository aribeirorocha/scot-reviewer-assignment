#include "App.h"
#include <iostream>

bool App::loadData(const std::string& filename) {
    if (!parser.parseFile(filename)) return false;

    buildInitialGraph();
    return true;
}

void App::buildInitialGraph() {
    // Reset do grafo (se necessário)
    masterGraph = Graph<int>();


    // Adicionamos cada submissão e cada revisor como um vértice.
    for (const auto& sub : parser.getSubmissions()) {
        masterGraph.addVertex(sub.id);
    }

    for (const auto& rev : parser.getReviewers()) {
        masterGraph.addVertex(rev.id);
    }
    
    std::cout << "[Grafo] Vertices criados: " << masterGraph.getVertexSet().size() << std::endl;
}