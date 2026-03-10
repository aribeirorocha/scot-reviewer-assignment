#ifndef EDGE_H
#define EDGE_H

template <class T> class Vertex;
template <class T>
class Edge {
public:
    Edge(Vertex<T>* orig, Vertex<T>* dest, double capacity);
    Vertex<T>* getDest() const;
    Vertex<T>* getOrig() const;
    Edge<T>* getReverse() const;
    void setReverse(Edge<T>* rev);
    double getCapacity() const;
    double getFlow() const;
    void setFlow(double flow);

private:
    Vertex<T>* orig;
    Vertex<T>* dest;
    double capacity; 
    double flow;     
    Edge<T>* reverse = nullptr; 
};

template <class T>
Edge<T>::Edge(Vertex<T>* orig, Vertex<T>* dest, double capacity) {
    this->orig = orig;
    this->dest = dest;
    this->capacity = capacity;
    this->flow = 0;
}

template <class T>
Vertex<T>* Edge<T>::getDest() const { return dest; }

template <class T>
Vertex<T>* Edge<T>::getOrig() const { return orig; }

template <class T>
Edge<T>* Edge<T>::getReverse() const { return reverse; }

template <class T>
void Edge<T>::setReverse(Edge<T>* rev) { reverse = rev; }

template <class T>
double Edge<T>::getCapacity() const { return capacity; }

template <class T>
double Edge<T>::getFlow() const { return flow; }

template <class T>
void Edge<T>::setFlow(double flow) { this->flow = flow; }

#endif