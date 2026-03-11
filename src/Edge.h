/**
 * @file Edge.h
 * @brief Definição da classe Edge (Aresta) para a estrutura de grafo.
 * * Esta classe armazena informações sobre as ligações entre vértices, incluindo
 * capacidades e fluxos, essenciais para algoritmos de Fluxo Máximo como o Edmonds-Karp.
 */

#ifndef EDGE_H
#define EDGE_H

template <class T> class Vertex;

/**
 * @class Edge
 * @brief Representa uma aresta num grafo direcionado.
 * @tparam T Tipo de dado armazenado nos vértices.
 */

template <class T>
class Edge {
public:
    /**
     * @brief Construtor da classe Edge.
     * @param orig Ponteiro para o vértice de origem.
     * @param dest Ponteiro para o vértice de destino.
     * @param capacity Capacidade máxima de fluxo da aresta.
     * @details Complexidade Temporal: O(1).
     */
    Edge(Vertex<T>* orig, Vertex<T>* dest, double capacity);
    /** @brief Obtém o vértice de destino. @return Ponteiro para o vértice dest. */
    Vertex<T>* getDest() const;
    /** @brief Obtém o vértice de origem. @return Ponteiro para o vértice orig. */
    Vertex<T>* getOrig() const;
    /** @brief Obtém a aresta reversa (residual). @return Ponteiro para a Edge reversa. */
    Edge<T>* getReverse() const;
    /** @brief Define a aresta reversa no grafo residual. @param rev Ponteiro para a aresta reversa. */
    void setReverse(Edge<T>* rev);
    /** @brief Obtém a capacidade da aresta. @return Valor da capacidade. */
    double getCapacity() const;
    /** @brief Obtém o fluxo atual da aresta. @return Valor do fluxo. */
    double getFlow() const;
    /** @brief Define o fluxo da aresta. @param flow Novo valor de fluxo. */
    void setFlow(double flow);

private:
    Vertex<T>* orig;
    Vertex<T>* dest;
    double capacity; 
    double flow;     
    /** * @brief Ponteiro para a aresta correspondente no sentido oposto.
     * Útil para atualizar o fluxo residual no algoritmo de Edmonds-Karp.
     */
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