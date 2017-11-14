#ifndef GRAPH_H
#define GRAPH_H

#include <experimental/optional>
#include <functional>
#include <unordered_map>
#include <unordered_set>

typedef unsigned int NodeId;

template <class T>
class Graph {
public:
    Graph();

    NodeId add_vertex(const T& v);
    void add_dir_edge(const NodeId& v, const NodeId& u);
    void add_edge(const NodeId& v, const NodeId& u);

    std::unordered_set<NodeId>& edges(const NodeId& v);
    std::experimental::optional<T> data(const NodeId& v);

    void for_vertex(std::function<void(NodeId)> f);
    void for_edge(std::function<void(NodeId, NodeId)> f);
private:
    std::unordered_map<NodeId, T> _vertices;
    std::unordered_map<NodeId, std::unordered_set<NodeId>> _edges;
    NodeId _idx_counter;
};

template <class T>
inline Graph<T>::Graph() : _vertices({}), _edges({}), _idx_counter(0) {
}

template <class T>
inline NodeId Graph<T>::add_vertex(const T& v) {
    _vertices[_idx_counter] = v;
    return _idx_counter++;
}

template <class T>
inline void Graph<T>::add_dir_edge(const NodeId& v, const NodeId& u) {
    assert(_vertices.count(v) && _vertices.count(u));
    _edges[v].insert(u);
}

template <class T>
inline void Graph<T>::add_edge(const NodeId& v, const NodeId& u) {
    add_dir_edge(v, u);
    add_dir_edge(u, v);
}

template <class T>
inline std::unordered_set<NodeId>& Graph<T>::edges(const NodeId& v) {
    if (_edges.count(v)) {
        return _edges[v];
    } else {
        return {};
    }//return _edges.count(v) ? _edges[const_cast<NodeId>(v)] : {};
}

template <class T>
inline std::experimental::optional<T> Graph<T>::data(const NodeId& v) {
    if (_edges.count(v)) {
        return _vertices[v];
    } else {
        return {};
    }//return _vertices.count(v) ? _vertices[const_cast<NodeId>(v)] : {};
}

template <class T>
inline void Graph<T>::for_vertex(std::function<void(NodeId)> f) {
    for (const auto& vertex : _vertices) {
        f(vertex.first);
    }
}

template <class T>
inline void Graph<T>::for_edge(std::function<void(NodeId, NodeId)> f) {
    for (const auto& from : _edges) {
        for (const auto& to : from.second) {
            f(from.first, to);
        }
    }
}

#endif//GRAPH_H
