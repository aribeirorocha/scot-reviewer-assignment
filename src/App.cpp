/**
 * @file App.cpp
 * @brief Implementação da classe App, que gere a lógica principal e a estrutura do grafo.
 */

#include "App.h"
#include "Graph.h"
#include <iostream>
#include <fstream>

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

double App::runFlow(std::vector<int> excludeIds) {
    auto params = parser.getParams();
    int s = -1, t = -2;

    Graph<int> flowGraph;
    flowGraph.addVertex(s);
    flowGraph.addVertex(t);

    for (auto& rev : parser.getReviewers()) {
        if (std::find(excludeIds.begin(), excludeIds.end(), rev.id) != excludeIds.end()) continue;
        flowGraph.addVertex(rev.id);
        flowGraph.addBidirectionalEdge(rev.id, t, params.maxReviewsPerReviewer);
    }
    for (auto& sub : parser.getSubmissions()) {
        flowGraph.addVertex(sub.id);
        flowGraph.addBidirectionalEdge(s, sub.id, params.minReviewsPerSubmission);
        for (auto& rev : parser.getReviewers()) {
            if (std::find(excludeIds.begin(), excludeIds.end(), rev.id) != excludeIds.end()) continue;
            if (sub.primaryDomain == rev.primaryExpertise) {
                flowGraph.addBidirectionalEdge(sub.id, rev.id, 1);
            }
        }
    }

    return flowGraph.edmondsKarp(s, t);
}

void App::solveT21() {
    auto params = parser.getParams();
    int required = params.minReviewsPerSubmission * (int)parser.getSubmissions().size();

    double total = runFlow(); // no exclusions

    if (total >= required) {
        std::cout << "\nSucesso! Total de atribuicoes: " << total << std::endl;
    } else {
        std::cout << "\nFalha! Atribuicoes conseguidas: " << total
                  << " / " << required << " necessarias." << std::endl;
    }
}

void App::SolveRisk1() {
    auto params = parser.getParams();
    int required = params.minReviewsPerSubmission * (int)parser.getSubmissions().size();

    double baseFlow = runFlow(); // no exclusions
    if (baseFlow < required) {
        std::cout << "\n[Risk Analysis 1] Base assignment already impossible, skipping risk analysis.\n";
        return;
    }

    std::cout << "\n[Risk Analysis 1] Base assignment feasible (" << baseFlow << " assignments).\n";
    std::cout << "Checking which reviewers are critical...\n";

    std::vector<int> riskyReviewers;

    for (auto& rev : parser.getReviewers()) {
        double flow = runFlow({rev.id}); // exclude this reviewer
        if (flow < required) {
            riskyReviewers.push_back(rev.id);
        }
    }

    std::cout << "\n#Risk Analysis: 1\n";
    if (riskyReviewers.empty()) {
        std::cout << "No critical reviewers found. Assignment is robust.\n";
    } else {
        std::cout << "Critical reviewers (removing any one makes assignment impossible):\n";
        for (int id : riskyReviewers) {
            std::cout << id << "\n";
        }
    }
}