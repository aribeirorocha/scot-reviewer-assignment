/**
 * @file Vertex.h
 * @brief Definição da classe Vertex (Vértice) para a estrutura de grafo.
 * * Cada vértice representa uma entidade no sistema (Submissão ou Revisor) e
 * armazena a lista de arestas que partem dele, além de estados auxiliares
 * para algoritmos de percurso como BFS, essenciais para o cálculo de fluxo.
 */
#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include "Edge.h"
/**
 * @class Vertex
 * @brief Representa um nó no grafo.
 * @tparam T Tipo de dado armazenado no vértice (geralmente int para IDs).
 */
template <class T>
class Vertex {
public:
    /**
     * @brief Construtor da classe Vertex.
     * @param in Informação/ID a armazenar no vértice.
     */
    Vertex(T in);
    /** @brief Obtém a informação armazenada no vértice. @return Valor de tipo T. */
    T getInfo() const;
    /** @brief Obtém a lista de arestas adjacentes (saída). @return Vector de ponteiros para Edge. */
    std::vector<Edge<T>*> getAdj() const;
    /** @brief Verifica se o vértice foi visitado durante um percurso. @return true se visitado. */
    bool isVisited() const;
    /** @brief Define o estado de visita do vértice. @param v Novo estado de visita. */
    void setVisited(bool v);
    /** @brief Obtém a aresta que antecede este vértice num caminho. @return Ponteiro para a Edge de origem. */
    Edge<T>* getPath() const;
    /** @brief Define a aresta que leva a este vértice (útil para reconstruir caminhos no BFS). @param path Ponteiro para Edge. */
    void setPath(Edge<T>* path);
    /**
     * @brief Adiciona uma nova aresta de saída a partir deste vértice.
     * @param dest Vértice de destino.
     * @param capacity Capacidade máxima da aresta para o fluxo.
     * @return Ponteiro para a aresta recém-criada.
     * @details Complexidade Temporal: O(1).
     */
    Edge<T>* addEdge(Vertex<T>* dest, double capacity);

private:
    T info;
    std::vector<Edge<T>*> adj;
    bool visited;
    Edge<T>* path; 
};

template <class T>
Vertex<T>::Vertex(T in) : info(in), visited(false), path(nullptr) {}

template <class T>
T Vertex<T>::getInfo() const { return info; }

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const { return adj; }

template <class T>
bool Vertex<T>::isVisited() const { return visited; }

template <class T>
void Vertex<T>::setVisited(bool v) { visited = v; }

template <class T>
Edge<T>* Vertex<T>::getPath() const { return path; }

template <class T>
void Vertex<T>::setPath(Edge<T>* path) { this->path = path; }

template <class T>
Edge<T>* Vertex<T>::addEdge(Vertex<T>* dest, double capacity) {
    Edge<T>* newEdge = new Edge<T>(this, dest, capacity);
    adj.push_back(newEdge);
    return newEdge;
}

#endif