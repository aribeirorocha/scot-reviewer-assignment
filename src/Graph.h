/**
 * @file Graph.h
 * @brief Definição da classe genérica Graph para representação de grafos.
 *
 * Implementa um grafo utilizando listas de adjacências e fornece métodos
 * essenciais para a manipulação de vértices e arestas, incluindo suporte
 * para grafos residuais em algoritmos de fluxo.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
#include "Vertex.h"

/**
 * @class Graph
 * @brief Representa um grafo composto por um conjunto de vértices.
 * @tparam T Tipo de dado a ser armazenado nos vértices (ex: int, string).
 */
template <class T>
class Graph {
public:
    /**
     * @brief Destrutor da classe Graph.
     * @details Liberta a memória de todos os vértices e arestas associados.
     * Complexidade Temporal: O(V + E).
     */
    ~Graph();

    /**
     * @brief Procura um vértice no grafo com base na informação fornecida.
     * @param in Informação a procurar.
     * @return Ponteiro para o vértice encontrado ou nullptr caso não exista.
     * @details Complexidade Temporal: O(V).
     */
    Vertex<T>* findVertex(const T& in) const;

    /**
     * @brief Adiciona um novo vértice ao grafo.
     * @param in Informação do novo vértice.
     * @return true se o vértice foi adicionado, false se já existia.
     * @details Complexidade Temporal: O(V).
     */
    bool addVertex(const T& in);

    /**
     * @brief Adiciona uma aresta direcionada entre dois vértices.
     * @param sourc Informação do vértice de origem.
     * @param dest Informação do vértice de destino.
     * @param capacity Capacidade da aresta.
     * @return true se a aresta foi adicionada, false se algum vértice não existir.
     * @details Complexidade Temporal: O(V).
     */
    bool addEdge(const T& sourc, const T& dest, double capacity);

    /**
     * @brief Adiciona uma aresta direta e a sua reversa residual (capacidade 0).
     * @param sourc Origem.
     * @param dest Destino.
     * @param capacity Capacidade da aresta direta.
     * @details Complexidade Temporal: O(V).
     */
    void addBidirectionalEdge(const T& sourc, const T& dest, double capacity);

    /**
     * @brief Retorna o conjunto de todos os vértices do grafo.
     * @return Vector de ponteiros para os vértices.
     * @details Complexidade Temporal: O(1).
     */
    std::vector<Vertex<T>*> getVertexSet() const;

    /**
     * @brief Executa o algoritmo de Edmonds-Karp (BFS-based Max Flow).
     * @param source ID do vértice fonte.
     * @param target ID do vértice sumidouro.
     * @return Valor do fluxo máximo.
     * @details Complexidade Temporal: O(V * E^2).
     */
    double edmondsKarp(T source, T target);

private:
    std::vector<Vertex<T>*> vertexSet;

    /**
     * @brief BFS auxiliar que encontra um caminho aumentante de s a t.
     * @param s Vértice fonte.
     * @param t Vértice destino.
     * @return true se existe caminho com capacidade residual positiva.
     * @details Complexidade Temporal: O(V + E).
     */
    bool bfs(Vertex<T>* s, Vertex<T>* t);
};

// ─── Destrutor ────────────────────────────────────────────────────────────────

template <class T>
Graph<T>::~Graph() {
    for (auto v : vertexSet) {
        for (auto e : v->getAdj()) delete e;
        delete v;
    }
}

// ─── findVertex ───────────────────────────────────────────────────────────────

template <class T>
Vertex<T>* Graph<T>::findVertex(const T& in) const {
    for (auto v : vertexSet)
        if (v->getInfo() == in) return v;
    return nullptr;
}

// ─── addVertex ────────────────────────────────────────────────────────────────

template <class T>
bool Graph<T>::addVertex(const T& in) {
    if (findVertex(in) != nullptr) return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

// ─── addEdge ──────────────────────────────────────────────────────────────────

template <class T>
bool Graph<T>::addEdge(const T& sourc, const T& dest, double capacity) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (!v1 || !v2) return false;
    v1->addEdge(v2, capacity);
    return true;
}

// ─── addBidirectionalEdge ─────────────────────────────────────────────────────

template <class T>
void Graph<T>::addBidirectionalEdge(const T& sourc, const T& dest, double capacity) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (!v1 || !v2) return;

    auto e1 = v1->addEdge(v2, capacity);
    auto e2 = v2->addEdge(v1, 0); // aresta residual

    e1->setReverse(e2);
    e2->setReverse(e1);
}

// ─── getVertexSet ─────────────────────────────────────────────────────────────

template <class T>
std::vector<Vertex<T>*> Graph<T>::getVertexSet() const {
    return vertexSet;
}

// ─── bfs (membro privado) ─────────────────────────────────────────────────────

template <class T>
bool Graph<T>::bfs(Vertex<T>* s, Vertex<T>* t) {
    for (auto v : vertexSet) {
        v->setVisited(false);
        v->setPath(nullptr);
    }
    std::queue<Vertex<T>*> q;
    q.push(s);
    s->setVisited(true);

    while (!q.empty()) {
        auto v = q.front(); q.pop();
        if (v == t) return true;
        for (auto e : v->getAdj()) {
            auto dest = e->getDest();
            if (!dest->isVisited() && e->getCapacity() - e->getFlow() > 0) {
                dest->setVisited(true);
                dest->setPath(e);
                q.push(dest);
            }
        }
    }
    return false;
}

// ─── edmondsKarp (membro público) ─────────────────────────────────────────────

template <class T>
double Graph<T>::edmondsKarp(T source, T target) {
    Vertex<T>* s = findVertex(source);
    Vertex<T>* t = findVertex(target);
    if (!s || !t) return 0;

    // Reset de todos os fluxos
    for (auto v : vertexSet)
        for (auto e : v->getAdj())
            e->setFlow(0);

    double maxFlow = 0;

    while (bfs(s, t)) {
        // Encontrar o gargalo (bottleneck) ao longo do caminho
        double bottleneck = std::numeric_limits<double>::max();
        for (auto v = t; v != s; ) {
            auto e = v->getPath();
            bottleneck = std::min(bottleneck, e->getCapacity() - e->getFlow());
            v = e->getOrig();
        }

        // Atualizar os fluxos ao longo do caminho
        for (auto v = t; v != s; ) {
            auto e = v->getPath();
            e->setFlow(e->getFlow() + bottleneck);
            e->getReverse()->setFlow(e->getReverse()->getFlow() - bottleneck);
            v = e->getOrig();
        }

        maxFlow += bottleneck;
    }

    return maxFlow;
}

#endif