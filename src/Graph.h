#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Vertex.h"

template <class T>
class Graph {
public:
    ~Graph();
    Vertex<T>* findVertex(const T& in) const;
    bool addVertex(const T& in);
    bool addEdge(const T& sourc, const T& dest, double capacity);
    
    void addBidirectionalEdge(const T& sourc, const T& dest, double capacity);
    
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