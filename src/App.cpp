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
#include <set>

// ─── Namespace de IDs no grafo de fluxo ──────────────────────────────────────
// Para evitar colisão entre IDs de submissões e revisores (ambos podem começar em 1),
// os revisores são representados no grafo com ID negativo: revNode(id) = -(id + 10)
// Os nós source e sink usam -1 e -2, portanto usamos offset de 10 para não colidir.
// Exemplo: reviewer ID=1 → node ID=-11; reviewer ID=8 → node ID=-18.
static int revNode(int id)  { return -(id + 10); }  // revisor no grafo
static int subNode(int id)  { return id; }           // submissão no grafo (ID real)

// ─── Estrutura auxiliar interna ───────────────────────────────────────────────
struct EdgeMeta {
    int submissionId;
    int reviewerId;
    int matchedDomain;
};

// ─── loadData ─────────────────────────────────────────────────────────────────

/**
 * @brief Carrega os dados do ficheiro fornecido e inicializa a estrutura do grafo.
 * @details Complexidade Temporal: O(L * C + S + R)
 */
bool App::loadData(const std::string& filename) {
    if (!parser.parseFile(filename)) return false;
    buildInitialGraph();
    return true;
}

// ─── buildInitialGraph ────────────────────────────────────────────────────────

/**
 * @brief Constrói o grafo inicial com vértices de submissões e revisores.
 *
 * O masterGraph serve como estrutura de referência para o menu (contagem de vértices).
 * O grafo de fluxo é construído dinamicamente em runFlow() para cada execução.
 *
 * @details Complexidade Temporal: O(S + R)
 */
void App::buildInitialGraph() {
    masterGraph = Graph<int>();
    for (const auto& sub : parser.getSubmissions()) masterGraph.addVertex(sub.id);
    for (const auto& rev : parser.getReviewers())   masterGraph.addVertex(rev.id);
    std::cout << "[Grafo] Vertices criados: " << masterGraph.getVertexSet().size() << std::endl;
}

// ─── runFlow ──────────────────────────────────────────────────────────────────

/**
 * @brief Constrói o grafo de fluxo e executa Edmonds-Karp.
 *
 * Estrutura do grafo:
 *   source(-1) → submissão [cap = minReviewsPerSubmission]
 *   submissão  → revisor   [cap = 1, apenas se domínio compatível com o modo]
 *   revisor    → sink(-2)  [cap = maxReviewsPerReviewer]
 *
 * Modos (GenerateAssignments):
 *   1 → só domínio primário da submissão vs expertise primária do revisor
 *   2 → domínio primário e secundário da submissão vs expertise primária do revisor
 *   3 → domínio primário e secundário da submissão vs expertise primária e secundária do revisor
 *
 * @param excludeIds Revisores a excluir do grafo (análise de risco).
 * @param outAssignments Se não nulo, preenche com as atribuições extraídas.
 * @param mode Modo de compatibilidade de domínios.
 * @return Valor do fluxo máximo.
 * @details Complexidade Temporal: O(V * E^2)
 */
double App::runFlow(const std::vector<int>& excludeIds,
                    std::vector<Assignment>* outAssignments,
                    int mode) {
    auto params = parser.getParams();
    const int S = -1; // source
    const int T = -2; // sink

    // --- Construção do grafo de fluxo ---
    // IMPORTANTE: IDs de submissões e revisores podem colidir (ambos começam em 1).
    // Usamos subNode(id)=id para submissões e revNode(id)=-(id+10) para revisores,
    // garantindo namespaces distintos no grafo.
    Graph<int> flowGraph;
    flowGraph.addVertex(S);
    flowGraph.addVertex(T);

    // Revisores → sink
    for (const auto& rev : parser.getReviewers()) {
        if (std::find(excludeIds.begin(), excludeIds.end(), rev.id) != excludeIds.end()) continue;
        flowGraph.addVertex(revNode(rev.id));
        flowGraph.addBidirectionalEdge(revNode(rev.id), T, params.maxReviewsPerReviewer);
    }

    // Metadados das arestas submissão→revisor para extrair o Match correto depois
    std::vector<EdgeMeta> edgeMeta;

    // Source → submissões → revisores
    for (const auto& sub : parser.getSubmissions()) {
        flowGraph.addVertex(subNode(sub.id));
        flowGraph.addBidirectionalEdge(S, subNode(sub.id), params.minReviewsPerSubmission);

        for (const auto& rev : parser.getReviewers()) {
            if (std::find(excludeIds.begin(), excludeIds.end(), rev.id) != excludeIds.end()) continue;

            bool connected = false;
            int matchDomain = -1;

            if (mode == 1) {
                if (sub.primaryDomain == rev.primaryExpertise) {
                    connected = true;
                    matchDomain = sub.primaryDomain;
                }
            } else if (mode == 2) {
                if (sub.primaryDomain == rev.primaryExpertise) {
                    connected = true;
                    matchDomain = sub.primaryDomain;
                } else if (sub.secondaryDomain != -1 && sub.secondaryDomain == rev.primaryExpertise) {
                    connected = true;
                    matchDomain = sub.secondaryDomain;
                }
            } else if (mode == 3) {
                if (sub.primaryDomain == rev.primaryExpertise) {
                    connected = true;
                    matchDomain = sub.primaryDomain;
                } else if (rev.secondaryExpertise != -1 &&
                           sub.primaryDomain == rev.secondaryExpertise) {
                    connected = true;
                    matchDomain = sub.primaryDomain;
                } else if (sub.secondaryDomain != -1 &&
                           sub.secondaryDomain == rev.primaryExpertise) {
                    connected = true;
                    matchDomain = sub.secondaryDomain;
                } else if (sub.secondaryDomain != -1 && rev.secondaryExpertise != -1 &&
                           sub.secondaryDomain == rev.secondaryExpertise) {
                    connected = true;
                    matchDomain = sub.secondaryDomain;
                }
            }

            if (connected) {
                flowGraph.addBidirectionalEdge(subNode(sub.id), revNode(rev.id), 1);
                edgeMeta.push_back({sub.id, rev.id, matchDomain});
            }
        }
    }

    // --- Execução do algoritmo ---
    double maxFlow = flowGraph.edmondsKarp(S, T);

    // --- Extração das atribuições com o Match correto ---
    if (outAssignments != nullptr) {
        outAssignments->clear();

        // Índice rápido: (subNode, revNode) → metadata
        std::map<std::pair<int,int>, EdgeMeta> domainMap;
        for (const auto& em : edgeMeta)
            domainMap[{subNode(em.submissionId), revNode(em.reviewerId)}] = em;

        for (auto v : flowGraph.getVertexSet()) {
            int vid = v->getInfo();
            if (vid <= 0) continue; // só vértices de submissões (IDs positivos, > 0)
            for (auto e : v->getAdj()) {
                int destId = e->getDest()->getInfo();
                if (destId >= 0) continue; // destino deve ser revNode (negativo, != S nem T)
                if (destId == T) continue;
                if (e->getFlow() > 0) {
                    auto it = domainMap.find({vid, destId});
                    if (it != domainMap.end()) {
                        outAssignments->push_back({it->second.submissionId,
                                                   it->second.reviewerId,
                                                   it->second.matchedDomain});
                    }
                }
            }
        }
    }

    return maxFlow;
}

// ─── solveAutomatic ───────────────────────────────────────────────────────────

/**
 * @brief Executa toda a lógica conforme os parâmetros do CSV.
 *
 * Chamado em modo batch e quando o utilizador carrega dados e quer executar tudo.
 * Respeita os parâmetros GenerateAssignments e RiskAnalysis do ficheiro.
 *
 * @details Complexidade Temporal: O(V * E^2) para o flow + O(R * V * E^2) se risk > 0.
 */
void App::solveAutomatic(const std::string& overrideOutput) {
    auto params = parser.getParams();
    int required = params.minReviewsPerSubmission * (int)parser.getSubmissions().size();
    int mode = params.generateAssignments;

    // Define o ficheiro final: se o argumento não for vazio, usa o argumento. Senão, usa o do CSV.
    std::string finalOutputFileName = overrideOutput.empty() ? params.outputFileName : overrideOutput;

    // GenerateAssignments == 0: corre mas não reporta atribuições
    std::vector<Assignment> assignments;
    double total = runFlow({}, (mode != 0 ? &assignments : nullptr), (mode == 0 ? 1 : mode));

    if (total >= required) {
        std::cout << "\nSucesso! Total de atribuicoes: " << (int)total << std::endl;
    } else {
        std::cout << "\nAtribuicao incompleta: " << (int)total
                  << " / " << required << " necessarias." << std::endl;
    }

    // Escreve output de atribuições (só se GenerateAssignments != 0)
    if (mode != 0) {
        writeOutput(finalOutputFileName, assignments);
    }

    // Análise de risco (se configurada)
    if (params.riskAnalysis >= 1) {
        int required2 = params.minReviewsPerSubmission * (int)parser.getSubmissions().size();
        double baseFlow = runFlow({}, nullptr, (mode == 0 ? 1 : mode));

        std::vector<int> riskyReviewers;
        if (baseFlow >= required2) {
            for (const auto& rev : parser.getReviewers()) {
                double f = runFlow({rev.id}, nullptr, (mode == 0 ? 1 : mode));
                if (f < required2)
                    riskyReviewers.push_back(rev.id);
            }
        }
        std::sort(riskyReviewers.begin(), riskyReviewers.end());
        writeRiskOutput(finalOutputFileName, riskyReviewers, params.riskAnalysis);
    }
    
}

// ─── solveT21 (modo interativo manual) ────────────────────────────────────────

/**
 * @brief Executa a atribuição de revisões em modo interativo.
 *
 * Usa o modo definido em GenerateAssignments do ficheiro CSV carregado.
 * @details Complexidade Temporal: O(V * E^2)
 */
void App::solve() {
    auto params = parser.getParams();
    int required = params.minReviewsPerSubmission * (int)parser.getSubmissions().size();
    int mode = (params.generateAssignments == 0) ? 1 : params.generateAssignments;

    std::vector<Assignment> assignments;
    double total = runFlow({}, &assignments, mode);

    if (total >= required) {
        std::cout << "\nSucesso! Total de atribuicoes: " << (int)total << std::endl;
    } else {
        std::cout << "\nAtribuicao incompleta: " << (int)total
                  << " / " << required << " necessarias." << std::endl;
    }

    writeOutput(params.outputFileName, assignments);
}

// ─── solveRisk1 (modo interativo manual) ──────────────────────────────────────

/**
 * @brief Executa a análise de risco (K=1) em modo interativo.
 *
 * Para cada revisor, remove-o do grafo e verifica se a atribuição completa
 * ainda é possível. Revisores cuja ausência torna isso impossível são "críticos".
 *
 * @details Complexidade Temporal: O(R * V * E^2)
 */
void App::solveRisk1() {
    auto params = parser.getParams();
    int required = params.minReviewsPerSubmission * (int)parser.getSubmissions().size();
    int mode = (params.generateAssignments == 0) ? 1 : params.generateAssignments;

    double baseFlow = runFlow({}, nullptr, mode);
    if (baseFlow < required) {
        std::cout << "\n[Risk Analysis] A atribuicao base ja e impossivel. Analise de risco ignorada.\n";
        return;
    }

    std::vector<int> riskyReviewers;
    for (const auto& rev : parser.getReviewers()) {
        if (runFlow({rev.id}, nullptr, mode) < required)
            riskyReviewers.push_back(rev.id);
    }
    std::sort(riskyReviewers.begin(), riskyReviewers.end());

    // Mostra no terminal
    std::cout << "\n#Risk Analysis: 1\n";
    if (riskyReviewers.empty()) {
        std::cout << "\n"; // linha vazia = nenhum revisor crítico
    } else {
        for (size_t i = 0; i < riskyReviewers.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << riskyReviewers[i];
        }
        std::cout << "\n";
    }

    // Escreve também no ficheiro
    writeRiskOutput(params.outputFileName, riskyReviewers, 1);
}

// ─── writeOutput ──────────────────────────────────────────────────────────────

/**
 * @brief Escreve o ficheiro de saída com as atribuições e revisões em falta.
 *
 * Secções escritas:
 *   1. #SubmissionId,ReviewerId,Match  (ordenado por submissão, depois revisor)
 *   2. #ReviewerId,SubmissionId,Match  (ordenado por revisor, depois submissão)
 *   3. #Total: N
 *   4. #SubmissionId,Domain,MissingReviews  (apenas para submissões com revisões em falta)
 *
 * @param filename Caminho do ficheiro de saída.
 * @param assignments Lista de atribuições.
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

    // Secção 1: Submission → Reviewer
    auto sorted = assignments;
    std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b) {
        return a.submissionId != b.submissionId ? a.submissionId < b.submissionId
                                                : a.reviewerId < b.reviewerId;
    });
    out << "#SubmissionId,ReviewerId,Match\n";
    for (const auto& a : sorted)
        out << a.submissionId << ", " << a.reviewerId << ", " << a.domain << "\n";

    // Secção 2: Reviewer → Submission
    std::sort(sorted.begin(), sorted.end(), [](const Assignment& a, const Assignment& b) {
        return a.reviewerId != b.reviewerId ? a.reviewerId < b.reviewerId
                                            : a.submissionId < b.submissionId;
    });
    out << "#ReviewerId,SubmissionId,Match\n";
    for (const auto& a : sorted)
        out << a.reviewerId << ", " << a.submissionId << ", " << a.domain << "\n";

    out << "#Total: " << assignments.size() << "\n";

    // Secção 4: submissões com revisões em falta (só se houver)
    std::map<int, int> reviewsGot;
    std::map<int, int> subDomain;
    for (const auto& sub : parser.getSubmissions()) {
        reviewsGot[sub.id] = 0;
        subDomain[sub.id]  = sub.primaryDomain;
    }
    for (const auto& a : assignments) reviewsGot[a.submissionId]++;

    std::vector<int> subIds;
    for (const auto& sub : parser.getSubmissions()) subIds.push_back(sub.id);
    std::sort(subIds.begin(), subIds.end());

    bool anyMissing = false;
    for (int sid : subIds) {
        if (params.minReviewsPerSubmission - reviewsGot[sid] > 0) { anyMissing = true; break; }
    }

    if (anyMissing) {
        out << "#SubmissionId,Domain,MissingReviews\n";
        for (int sid : subIds) {
            int missing = params.minReviewsPerSubmission - reviewsGot[sid];
            if (missing > 0)
                out << sid << ", " << subDomain[sid] << ", " << missing << "\n";
        }
    }

    out.close();
    std::cout << "[Output] Escrito: " << filename << std::endl;
}

// ─── writeRiskOutput ──────────────────────────────────────────────────────────

/**
 * @brief Anexa a secção de Risk Analysis ao ficheiro de saída (modo append).
 *
 * Formato:
 *   #Risk Analysis: M
 *   id1, id2, ...   (ou linha vazia se nenhum revisor é crítico)
 *
 * @param filename Caminho do ficheiro de saída.
 * @param riskyReviewers Lista ordenada de revisores críticos.
 * @param riskLevel Valor do parâmetro RiskAnalysis.
 * @details Complexidade Temporal: O(R)
 */
void App::writeRiskOutput(const std::string& filename,
                          const std::vector<int>& riskyReviewers,
                          int riskLevel) {
    std::ofstream out(filename, std::ios::app); // append ao ficheiro existente
    if (!out.is_open()) {
        std::cerr << "[Erro] Nao foi possivel abrir para risk output: " << filename << std::endl;
        return;
    }

    out << "#Risk Analysis: " << riskLevel << "\n";
    if (!riskyReviewers.empty()) {
        for (size_t i = 0; i < riskyReviewers.size(); ++i) {
            if (i > 0) out << ", ";
            out << riskyReviewers[i];
        }
        out << "\n";
    } else {
        out << "\n"; // linha vazia — nenhum revisor crítico
    }

    out.close();
}