/**
 * @file App.cpp
 * @brief Implementação da classe App, que gere a lógica principal e a estrutura do grafo.
 */

#include "App.h"
#include "Graph.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

/**
 * @brief Carrega os dados do ficheiro fornecido e inicializa a estrutura do grafo.
 * @details Complexidade Temporal: O(N * M + S + R)
 */
bool App::loadData(const std::string& filename) {
    if (!parser.parseFile(filename)) return false;
    buildInitialGraph();
    return true;
}

/**
 * @brief Constrói a estrutura inicial do grafo criando vértices para submissões e revisores.
 * @details Complexidade Temporal: O(S + R)
 */
void App::buildInitialGraph() {
    masterGraph = Graph<int>();
    for (const auto& sub : parser.getSubmissions()) masterGraph.addVertex(sub.id);
    for (const auto& rev : parser.getReviewers()) masterGraph.addVertex(rev.id);
    std::cout << "[Grafo] Vertices criados: " << masterGraph.getVertexSet().size() << std::endl;
}

/**
 * @brief Corre o fluxo máximo. Se outAssignments não for nulo, extrai as atribuições feitas.
 * @details Complexidade Temporal: O(V * E^2)
 */
double App::runFlow(std::vector<int> excludeIds, std::vector<Assignment>* outAssignments) {
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
            if (sub.primaryDomain == rev.primaryExpertise)
                flowGraph.addBidirectionalEdge(sub.id, rev.id, 1);
        }
    }

    double maxFlow = flowGraph.edmondsKarp(s, t);

    // Extract assignments from residual graph if requested
    if (outAssignments != nullptr) {
        outAssignments->clear();
        std::map<int, int> subDomain;
        for (const auto& sub : parser.getSubmissions()) subDomain[sub.id] = sub.primaryDomain;

        std::vector<int> subIds;
        for (const auto& sub : parser.getSubmissions()) subIds.push_back(sub.id);

        for (auto v : flowGraph.getVertexSet()) {
            int vid = v->getInfo();
            if (std::find(subIds.begin(), subIds.end(), vid) == subIds.end()) continue;
            for (auto e : v->getAdj()) {
                int destId = e->getDest()->getInfo();
                if (destId == s || destId == t) continue;
                if (e->getFlow() > 0)
                    outAssignments->push_back({vid, destId, subDomain[vid]});
            }
        }
    }

    return maxFlow;
}

/**
 * @brief Resolve T2.1 e escreve o ficheiro de saída.
 * @details Complexidade Temporal: O(V * E^2)
 */
void App::solveT21() {
    auto params = parser.getParams();
    int required = params.minReviewsPerSubmission * (int)parser.getSubmissions().size();

    std::vector<Assignment> assignments;
    double total = runFlow({}, &assignments);

    if (total >= required) {
        std::cout << "\nSucesso! Total de atribuicoes: " << total << std::endl;
    } else {
        std::cout << "\nFalha! Atribuicoes conseguidas: " << total
                  << " / " << required << " necessarias." << std::endl;
    }

    if (params.generateAssignments != 0)
        writeOutput(params.outputFileName, assignments);
}

/**
 * @brief Análise de risco: identifica revisores cuja ausência torna a atribuição impossível.
 * @details Complexidade Temporal: O(R * V * E^2)
 */
void App::SolveRisk1() {
    auto params = parser.getParams();
    int required = params.minReviewsPerSubmission * (int)parser.getSubmissions().size();

    double baseFlow = runFlow();
    if (baseFlow < required) {
        std::cout << "\n[Risk Analysis 1] Base assignment already impossible, skipping.\n";
        return;
    }

    std::vector<int> riskyReviewers;
    for (auto& rev : parser.getReviewers()) {
        if (runFlow({rev.id}) < required)
            riskyReviewers.push_back(rev.id);
    }

    std::sort(riskyReviewers.begin(), riskyReviewers.end());

    std::cout << "\n#Risk Analysis: 1\n";
    if (riskyReviewers.empty()) {
        std::cout << "No critical reviewers found.\n";
    } else {
        std::cout << "Critical reviewers: ";
        for (int id : riskyReviewers) std::cout << id << " ";
        std::cout << "\n";
    }
}

/**
 * @brief Escreve o ficheiro de saída com atribuições e revisões em falta.
 *
 * Sempre escreve ambas as secções:
 *   - Atribuições feitas (sub→rev e rev→sub)
 *   - Submissões com revisões em falta
 *
 * @details Complexidade Temporal: O(A log A + S)
 */
void App::writeOutput(const std::string& filename,
                      const std::vector<Assignment>& assignments) {
    auto params = parser.getParams();
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "[Erro] Nao foi possivel abrir: " << filename << std::endl;
        return;
    }
    // ── Secção 1: Submission → Reviewer ─────────────────────────────────────
    auto sorted = assignments;
    std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b) {
        return a.submissionId != b.submissionId ? a.submissionId < b.submissionId
                                                : a.reviewerId < b.reviewerId;
    });
    out << "#SubmissionId,ReviewerId,Match\n";
    for (auto& a : sorted)
        out << a.submissionId << ", " << a.reviewerId << ", " << a.domain << "\n";

    // ── Secção 2: Reviewer → Submission ─────────────────────────────────────
    std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b) {
        return a.reviewerId != b.reviewerId ? a.reviewerId < b.reviewerId
                                            : a.submissionId < b.submissionId;
    });
    out << "#ReviewerId,SubmissionId,Match\n";
    for (auto& a : sorted)
        out << a.reviewerId << ", " << a.submissionId << ", " << a.domain << "\n";

    out << "#Total: " << assignments.size() << "\n";

    // ── Secção 3: Submissões com revisões em falta ───────────────────────────
    std::map<int, int> reviewsGot;
    std::map<int, int> subDomain;
    for (auto& sub : parser.getSubmissions()) {
        reviewsGot[sub.id] = 0;
        subDomain[sub.id] = sub.primaryDomain;
    }
    for (auto& a : assignments) reviewsGot[a.submissionId]++;

    std::vector<int> subIds;
    for (auto& sub : parser.getSubmissions()) subIds.push_back(sub.id);
    std::sort(subIds.begin(), subIds.end());

    out << "#SubmissionId,Domain,MissingReviews\n";
    for (int sid : subIds) {
        int missing = params.minReviewsPerSubmission - reviewsGot[sid];
        if (missing > 0)
            out << sid << ", " << subDomain[sid] << ", " << missing << "\n";
    }

    out.close();
    std::cout << "[Output] Escrito: " << filename << std::endl;
}