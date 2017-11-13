#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <experimental/optional>

typedef unsigned int NodeId;

template <class T>
class Graph {
public:
    Graph();

    const NodeId& addVertex(const T& v);
    void addDirEdge(const NodeId& v, const NodeId& u);
    void addEdge(const NodeId& v, const NodeId& u);

    std::unordered_set<NodeId>& edgesOf(const NodeId& v);
    std::experimental::optional<T&> dataOf(const NodeId& v);
private:
    std::unordered_map<NodeId, T> _vertices;
    std::unordered_map<NodeId, std::unordered_set<NodeId>> _edges;
    NodeId _idx_counter;
};

template <class T>
inline Graph<T>::Graph() : _vertices({}), _edges({}), _idx_counter(0) {
}

template <class T>
inline const NodeId& Graph<T>::addVertex(const T& v) {
    _vertices[_idx_counter] = v;
    return _idx_counter++;
}

template <class T>
inline void Graph<T>::addDirEdge(const NodeId& v, const NodeId& u) {
    assert(_vertices.count(v) && _vertices.count(u));
    _edges[v].insert(_vertices[u]);
}

template <class T>
inline void Graph<T>::addEdge(const NodeId& v, const NodeId& u) {
    addDirEdge(v, u);
    addDirEdge(u, v);
}

template <class T>
inline std::unordered_set<NodeId>& Graph<T>::edgesOf(const NodeId& v) {
    if (_edges.count(v)) {
        return _edges[const_cast<NodeId>(v)];
    } else {
        return {};
    }//return _edges.count(v) ? _edges[const_cast<NodeId>(v)] : {};
}

template <class T>
inline std::experimental::optional<T&> Graph<T>::dataOf(const NodeId& v) {
    if (_edges.count(v)) {
        return _vertices[const_cast<NodeId>(v)];
    } else {
        return {};
    }//return _vertices.count(v) ? _vertices[const_cast<NodeId>(v)] : {};
}

#endif//GRAPH_H
