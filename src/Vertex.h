#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include "Edge.h"

template <class T>
class Vertex {
public:
    Vertex(T in);
    T getInfo() const;
    std::vector<Edge<T>*> getAdj() const;
    bool isVisited() const;
    void setVisited(bool v);
    Edge<T>* getPath() const;
    void setPath(Edge<T>* path);
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