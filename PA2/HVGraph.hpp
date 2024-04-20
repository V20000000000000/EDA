#ifndef HV_GRAPH_HPP
#define HV_GRAPH_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <set>
#include "Block.hpp"

// Define a structure for vertex properties
template <class T>
struct VertexProperty{
    T value;
    VertexProperty(T val) : value(val) {}
    VertexProperty(){}// 預設建構子
};

// Define a structure for edge properties
template <class U>
struct EdgeProperty {
    U value;
    EdgeProperty(){} // 預設建構子
    EdgeProperty(U val) : value(val) {}
    
    // Overloading the operator!=
    bool notEquals(const EdgeProperty<U>& other) const {
        return value != other.value;
    }
};


// Define a structure for vertices
struct Vertex {
    int id;
    Vertex(int i) : id(i) {}
    int getId() const { return id; }
};

// Define a class for the graph
template <class T, class U>
class HVGraph {
private:
    std::vector<std::map<int, float>> adjacencyList;
    std::vector<std::unordered_map<int, EdgeProperty<U>>> edgePropertiesMap;
    EdgeProperty<U> emptyEdgeProperty = EdgeProperty<U>();
    VertexProperty<T> emptyVertexProperty = VertexProperty<T>();
    std::vector<VertexProperty<T>> vertexPropertiesMap; // set every vertex's property(width)

public:
    // Constructor
    HVGraph(int numNodes) 
        : adjacencyList(numNodes), 
          edgePropertiesMap(numNodes)
    {
        for (int i = 0; i < numNodes; ++i) {
            vertexPropertiesMap.push_back(emptyVertexProperty);
        }
    }

    // Method to add a bidirected edge
    void addBidirectedEdge(int source, int target, float weight1, float weight2) {
        addDirectedEdge(source, target, weight1);
        addDirectedEdge(target, source, weight2);
    }

    void addBidirectedEdge(const Vertex& source, const Vertex& target, float weight1, float weight2) {
        addBidirectedEdge(source.getId(), target.getId(), weight1, weight2);
    }

    // Method to add an undirected edge
    void addUndirectedEdge(int source, int target, float weight) {
        addBidirectedEdge(source, target, weight, weight);
    }

    void addUndirectedEdge(const Vertex& source, const Vertex& target, float weight) {
        addUndirectedEdge(source.getId(), target.getId(), weight);
    }

    // Method to add a directed edge
    void addDirectedEdge(int source, int target, float weight) {
        adjacencyList[source][target] = weight;
    }

    void addDirectedEdge(const Vertex& source, const Vertex& target, float weight) {
        addDirectedEdge(source.getId(), target.getId(), weight);
    }

    // Method to get edge property
    EdgeProperty<U> getEdgeProperty(int source, int target) const {
        auto it = edgePropertiesMap[source].find(target);
        if (it != edgePropertiesMap[source].end()) {
            return it->second;
        }
        return emptyEdgeProperty;
    }

    EdgeProperty<U> getEdgeProperty(const Vertex& source, const Vertex& target) const {
        return getEdgeProperty(source.getId(), target.getId());
    }

    // Method to get edge weight
    float getEdgeWeight(int source, int target) const {
        auto it = adjacencyList[source].find(target);
        if (it != adjacencyList[source].end()) {
            return it->second;
        }
        return -1.0f;
    }

    float getEdgeWeight(const Vertex& source, const Vertex& target) const {
        return getEdgeWeight(source.getId(), target.getId());
    }

    // Method to get neighbors of a vertex
    std::vector<int> getNeighbors(int vertex) const {
        std::vector<int> neighbors;
        for (const auto& neighbor : adjacencyList[vertex]) {
            neighbors.push_back(neighbor.first);
        }
        return neighbors;
    }

    std::vector<int> getNeighbors(const Vertex& vertex) const {
        return getNeighbors(vertex.getId());
    }

    // Method to get vertex property
    VertexProperty<T> getVertexProperty(int vertex) const {
        return vertexPropertiesMap[vertex];
    }

    VertexProperty<T> getVertexProperty(const Vertex& vertex) const {
        return getVertexProperty(vertex.getId());
    }

    // Method to set edge property
    void setEdgeProperty(int source, int target, const EdgeProperty<U>& property) {
        if (property.notEquals(emptyEdgeProperty)) {
            edgePropertiesMap[source][target] = property;
        }
    }

    void setEdgeProperty(const Vertex& source, const Vertex& target, const EdgeProperty<U>& property) {
        setEdgeProperty(source.getId(), target.getId(), property);
    }

    // Method to set edge weight
    void setEdgeWeight(int source, int target, float weight) {
        adjacencyList[source][target] = weight;
    }

    void setEdgeWeight(const Vertex& source, const Vertex& target, float weight) {
        setEdgeWeight(source.getId(), target.getId(), weight);
    }

    // Method to set vertex property
    void setVertexProperty(int vertex, const VertexProperty<T>& property) {
        vertexPropertiesMap[vertex] = property;
    }

    void setVertexProperty(const Vertex& vertex, const VertexProperty<T>& property) {
        setVertexProperty(vertex.getId(), property);
    }

    std::map<int, std::map<float, EdgeProperty<U>>> getOutEdges(int vertex) const {
        std::set<int> s;
        for (const auto& neighbor : adjacencyList[vertex]) {
            s.insert(neighbor.first);
        }
        std::map<int, std::map<float, EdgeProperty<U>>> outEdges;
        for (int num : s) {
            float weight = adjacencyList[vertex].at(num);
            EdgeProperty<U> edgeProperty = getEdgeProperty(vertex, num);
            std::map<float, EdgeProperty<U>> edgeInfo;
            edgeInfo[weight] = edgeProperty;
            outEdges[num] = edgeInfo;
        }
        return outEdges;
    }

    std::map<int, std::map<float, EdgeProperty<U>>> getOutEdges(const Vertex& vertex) const {
        return getOutEdges(vertex.getId());
    }

    std::map<int, std::map<float, EdgeProperty<U>>> getInEdges(int vertex) const {
        std::map<int, std::map<float, EdgeProperty<U>>> inEdges;
        for (int i = 0; i < adjacencyList.size(); i++) {
            auto it = adjacencyList[i].find(vertex);
            if (it != adjacencyList[i].end()) {
                float weight = it->second;
                EdgeProperty<U> edgeProperty = getEdgeProperty(i, vertex);
                std::map<float, EdgeProperty<U>> edgeInfo;
                edgeInfo[weight] = edgeProperty;
                inEdges[i] = edgeInfo;
            }
        }
        return inEdges;
    }

    std::map<int, std::map<float, EdgeProperty<U>>> getInEdges(const Vertex& vertex) const {
        return getInEdges(vertex.getId());
    }

    // Method to get all vertices
    std::vector<std::pair<int, VertexProperty<T>>> getAllVertices() const {
        std::vector<std::pair<int, VertexProperty<T>>> verticesList;
        for (int i = 0; i < adjacencyList.size(); ++i) {
            verticesList.emplace_back(i, getVertexProperty(i));
        }
        return verticesList;
    }

    // Method to get size
    int size() const {
        return adjacencyList.size();
    }
};

#endif // HV_GRAPH_HPP

