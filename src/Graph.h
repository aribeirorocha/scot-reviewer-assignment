/**
 * @file Graph.h
 * @brief Definição da classe genérica Graph para representação de grafos.
 * * Implementa um grafo utilizando listas de adjacências e fornece métodos 
 * essenciais para a manipulação de vértices e arestas, incluindo suporte 
 * para grafos residuais em algoritmos de fluxo.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
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
     * Complexidade Temporal: O(V + E), onde V é o número de vértices e E o de arestas.
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
     * @details Complexidade Temporal: O(V) devido à chamada de findVertex.
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
     * @brief Adiciona uma aresta e a sua correspondente reversa para grafos de fluxo.
     * @param sourc Origem da aresta direta.
     * @param dest Destino da aresta direta.
     * @param capacity Capacidade da aresta direta.
     * @details Cria uma aresta (sourc->dest) com capacidade 'capacity' e uma 
     * aresta (dest->sourc) com capacidade 0. Essencial para o algoritmo de Edmonds-Karp.
     * Complexidade Temporal: O(V).
     */
    
    void addBidirectionalEdge(const T& sourc, const T& dest, double capacity);
    /**
     * @brief Retorna o conjunto de todos os vértices do grafo.
     * @return Vector de ponteiros para os vértices.
     * @details Complexidade Temporal: O(1).
     */
    
    std::vector<Vertex<T>*> getVertexSet() const;

private:
    std::vector<Vertex<T>*> vertexSet;
};

template <class T>
Graph<T>::~Graph() {
    for (auto v : vertexSet) {
        for (auto e : v->getAdj()) {
            delete e;
        }
        delete v;
    }
}

template <class T>
Vertex<T>* Graph<T>::findVertex(const T& in) const {
    for (auto v : vertexSet) {
        if (v->getInfo() == in) {
            return v;
        }
    }
    return nullptr;
}

template <class T>
bool Graph<T>::addVertex(const T& in) {
    if (findVertex(in) != nullptr) return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

template <class T>
bool Graph<T>::addEdge(const T& sourc, const T& dest, double capacity) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr) return false;
    v1->addEdge(v2, capacity);
    return true;
}

template <class T>
void Graph<T>::addBidirectionalEdge(const T& sourc, const T& dest, double capacity) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr) return;
    
    auto e1 = v1->addEdge(v2, capacity);
    auto e2 = v2->addEdge(v1, 0); // Aresta residual comeca com capacidade 0
    
    e1->setReverse(e2);
    e2->setReverse(e1);
}

template <class T>
std::vector<Vertex<T>*> Graph<T>::getVertexSet() const {
    return vertexSet;
}

#endif