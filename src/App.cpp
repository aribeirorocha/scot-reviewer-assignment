/**
 * @file App.cpp
 * @brief Implementação da classe App, que gere a lógica principal e a estrutura do grafo.
 */

#include "App.h"
#include <iostream>

/**
 * @brief Carrega os dados do ficheiro fornecido e inicializa a estrutura do grafo.
 * * @param filename Caminho para o ficheiro CSV de entrada.
 * @return true Se o ficheiro foi lido e o grafo construído com sucesso.
 * @return false Se ocorreu um erro na leitura do ficheiro.
 * * @details Complexidade Temporal: O(N * M + S + R), onde:
 * - N é o número de linhas no ficheiro.
 * - M é o comprimento médio de cada linha.
 * - S é o número de submissões.
 * - R é o número de revisores.
 */

bool App::loadData(const std::string& filename) {
    if (!parser.parseFile(filename)) return false;

    buildInitialGraph();
    return true;
}

/**
 * @brief Constrói a estrutura inicial do grafo criando vértices para submissões e revisores.
 * * @details Complexidade Temporal: O(S + R), onde S é o número de submissões 
 * e R o número de revisores carregados pelo parser. O método addVertex tem 
 * complexidade O(V) no pior caso na estrutura fornecida.
 */

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