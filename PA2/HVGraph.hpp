#ifndef HV_GRAPH_HPP
#define HV_GRAPH_HPP

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <set>
#include <queue>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <limits>

#include "Block.hpp"

using namespace std;

// Define a structure for vertex properties
template <class T>
struct VertexProperty
{
    T value;
    VertexProperty(T val) : value(val) {}
    VertexProperty() {} // 預設建構子
};

// Define a structure for edge properties
template <class U>
struct EdgeProperty
{
    U value;
    EdgeProperty() {} // 預設建構子
    EdgeProperty(U val) : value(val) {}

    // Overloading the operator!=
    bool notEquals(const EdgeProperty<U> &other) const
    {
        return value != other.value;
    }
};

// Define a structure for vertices
struct Vertex
{
    int id;
    Vertex(int i) : id(i) {}
    int getId() const { return id; }
};

// Define a class for the graph
template <class T, class U>
class HVGraph
{
private:
    std::vector<std::map<int, float>> adjacencyList;
    std::vector<std::unordered_map<int, EdgeProperty<U>>> edgePropertiesMap;
    EdgeProperty<U> emptyEdgeProperty = EdgeProperty<U>();
    VertexProperty<T> emptyVertexProperty = VertexProperty<T>();
    std::vector<VertexProperty<T>> vertexPropertiesMap; // set every vertex's property(width)
public:
    // Constructor
    HVGraph() {}

    HVGraph(int numNodes)
        : adjacencyList(numNodes),
          edgePropertiesMap(numNodes)
    {
        cout << "start init HVGraph" << endl;
        for (int i = 0; i < numNodes; ++i)
        {
            vertexPropertiesMap.push_back(emptyVertexProperty);
        }
        cout << "finish init HVGraph" << endl;
    }

    ~HVGraph() {}

    // Method to add a bidirected edge
    void addBidirectedEdge(int source, int target, float weight1, float weight2)
    {
        addDirectedEdge(source, target, weight1);
        addDirectedEdge(target, source, weight2);
    }

    void addBidirectedEdge(const Vertex &source, const Vertex &target, float weight1, float weight2)
    {
        addBidirectedEdge(source.getId(), target.getId(), weight1, weight2);
    }

    // Method to add an undirected edge
    void addUndirectedEdge(int source, int target, float weight)
    {
        addBidirectedEdge(source, target, weight, weight);
    }

    void addUndirectedEdge(const Vertex &source, const Vertex &target, float weight)
    {
        addUndirectedEdge(source.getId(), target.getId(), weight);
    }

    // Method to add a directed edge
    void addDirectedEdge(int source, int target, float weight)
    {
        adjacencyList[source][target] = weight;
    }

    void addDirectedEdge(const Vertex &source, const Vertex &target, float weight)
    {
        addDirectedEdge(source.getId(), target.getId(), weight);
    }

    void clearEdgeWeight(int vertex)
    {
        for (int inNeighbor : getInNeighbors(vertex))
        {
            if (inNeighbor != vertex)
            {
                adjacencyList[inNeighbor].erase(vertex);
            }
        }
        adjacencyList[vertex].clear();
    }

    // Method to get edge property
    EdgeProperty<U> getEdgeProperty(int source, int target) const
    {
        auto it = edgePropertiesMap[source].find(target);
        if (it != edgePropertiesMap[source].end())
        {
            return it->second;
        }
        return emptyEdgeProperty;
    }

    EdgeProperty<U> getEdgeProperty(const Vertex &source, const Vertex &target) const
    {
        return getEdgeProperty(source.getId(), target.getId());
    }

    // Method to get edge weight
    float getEdgeWeight(int source, int target) const
    {
        auto it = adjacencyList[source].find(target);
        if (it != adjacencyList[source].end())
        {
            return it->second;
        }
        return -1.0f;
    }

    float getEdgeWeight(const Vertex &source, const Vertex &target) const
    {
        return getEdgeWeight(source.getId(), target.getId());
    }

    // Method to get neighbors of a vertex
    std::vector<int> getNeighbors(int vertex) const
    {
        std::vector<int> neighbors;
        for (const auto &neighbor : adjacencyList[vertex])
        {
            neighbors.push_back(neighbor.first);
        }
        return neighbors;
    }

    std::vector<int> getNeighbors(const Vertex &vertex) const
    {
        return getNeighbors(vertex.getId());
    }

    // Method to get vertex property
    VertexProperty<T> getVertexProperty(int vertex) const
    {
        return vertexPropertiesMap[vertex];
    }

    VertexProperty<T> getVertexProperty(const Vertex &vertex) const
    {
        return getVertexProperty(vertex.getId());
    }

    // Method to set edge property
    void setEdgeProperty(int source, int target, const EdgeProperty<U> &property)
    {
        if (property.notEquals(emptyEdgeProperty))
        {
            edgePropertiesMap[source][target] = property;
        }
    }

    void setEdgeProperty(const Vertex &source, const Vertex &target, const EdgeProperty<U> &property)
    {
        setEdgeProperty(source.getId(), target.getId(), property);
    }

    // Method to set edge weight
    void setEdgeWeight(int source, int target, float weight)
    {
        adjacencyList[source][target] = weight;
    }

    void setEdgeWeight(const Vertex &source, const Vertex &target, float weight)
    {
        setEdgeWeight(source.getId(), target.getId(), weight);
    }

    // Method to set vertex property
    void setVertexProperty(int vertex, const VertexProperty<T> &property)
    {
        vertexPropertiesMap[vertex] = property;
    }

    void setVertexProperty(const Vertex &vertex, const VertexProperty<T> &property)
    {
        setVertexProperty(vertex.getId(), property);
    }

    void swapXVertex(int vertex1, int vertex2)
    {
        // cout << "--------------" << endl;
        // cout << "vertex1: " << vertex1 << " vertex2: " << vertex2 << endl;
        // cout << "vertex1 X: " << getVertexProperty(vertex1).value->getX() << endl;
        // cout << "vertex2 X: " << getVertexProperty(vertex2).value->getX() << endl;

        int temp = getVertexProperty(vertex1).value->getX();
        getVertexProperty(vertex1).value->setX(getVertexProperty(vertex2).value->getX());
        getVertexProperty(vertex2).value->setX(temp);
        //printGraph();

        clearEdgeWeight(vertex1);
        clearEdgeWeight(vertex2);

        //printGraph();

        // cout << "Swap X" << endl;

        // cout << "vertex1 X: " << getVertexProperty(vertex1).value->getX() << endl;
        // cout << "vertex2 X: " << getVertexProperty(vertex2).value->getX() << endl;

        //printGraph();
        //exit(0);
    }

    void swapYVertex(int vertex1, int vertex2)
    {
        // cout << "--------------" << endl;
        // cout << "vertex1: " << vertex1 << " vertex2: " << vertex2 << endl;
        // cout << "vertex1 Y: " << getVertexProperty(vertex1).value->getY() << endl;
        // cout << "vertex2 Y: " << getVertexProperty(vertex2).value->getY() << endl;

        int temp = getVertexProperty(vertex1).value->getY();
        getVertexProperty(vertex1).value->setY(getVertexProperty(vertex2).value->getY());
        getVertexProperty(vertex2).value->setY(temp);
        //printGraph();

        clearEdgeWeight(vertex1);
        clearEdgeWeight(vertex2);

        //printGraph();

        // cout << "Swap Y" << endl;

        // cout << "vertex1 Y: " << getVertexProperty(vertex1).value->getY() << endl;
        // cout << "vertex2 Y: " << getVertexProperty(vertex2).value->getY() << endl;

        //printGraph();
        //exit(0);
    }

    void maintainH(int vertex1, int vertex2)
    {
        for(int i = 0; i < size(); i++)
        {
            if(i != vertex1)
            {
                checkAndAddEdgeX(vertex1, i);
                checkAndAddEdgeX(i, vertex1);
            }
            if(i != vertex2)
            {
                checkAndAddEdgeX(vertex2, i);
                checkAndAddEdgeX(i, vertex2);
            }
        }

        addDirectedEdge(size(), vertex1, 0);
        addDirectedEdge(vertex1, size() + 1, getVertexProperty(vertex1).value->getWidth());
        addDirectedEdge(size(), vertex2, 0);
        addDirectedEdge(vertex2, size() + 1, getVertexProperty(vertex2).value->getWidth());
    }

    void maintainV(int vertex1, int vertex2)
    {
        for(int i = 0; i < size(); i++)
        {
            if(i != vertex1)
            {
                checkAndAddEdgeY(vertex1, i);
                checkAndAddEdgeY(i, vertex1);
            }
            if(i != vertex2)
            {
                checkAndAddEdgeY(vertex2, i);
                checkAndAddEdgeY(i, vertex2);
            }
        }

        addDirectedEdge(size(), vertex1, 0);
        addDirectedEdge(vertex1, size() + 1, getVertexProperty(vertex1).value->getHeight());
        addDirectedEdge(size(), vertex2, 0);
        addDirectedEdge(vertex2, size() + 1, getVertexProperty(vertex2).value->getHeight());
    }

    void checkAndAddEdgeX(int s, int t)
    {
        int x1 = getVertexProperty(s).value->getX();
        int y1 = getVertexProperty(s).value->getY();
        int x2 = getVertexProperty(t).value->getX();
        int y2 = getVertexProperty(t).value->getY();

        if (x1 < x2 && y1 < y2)
        {
            addDirectedEdge(s, t, getVertexProperty(s).value->getWidth());
        }
    }

    void checkAndAddEdgeY(int s, int t)
    {
        int x1 = getVertexProperty(s).value->getX();
        int y1 = getVertexProperty(s).value->getY();
        int x2 = getVertexProperty(t).value->getX();
        int y2 = getVertexProperty(t).value->getY();

        if (x1 > x2 && y1 < y2)
        {
            addDirectedEdge(s, t, getVertexProperty(s).value->getHeight());
        }
    }

    vector<int> getOutNeighbors(int vertex) const
    {
        vector<int> neighbors;
        for (const auto &neighbor : adjacencyList[vertex])
        {
            neighbors.push_back(neighbor.first);
        }
        return neighbors;
    }

    vector<int> getOutNeighbors(const Vertex &vertex) const
    {
        return getOutNeighbors(vertex.getId());
    }

    std::set<int> getInNeighbors(int vertex) const
    {
        std::set<int> neighbors;
        for (int i = 0; i < size(); ++i)
        {
            auto it = adjacencyList[i].find(vertex);
            if (it != adjacencyList[i].end())
            {
                neighbors.insert(i);
            }
        }
        return neighbors;
    }

    std::set<int> getInNeighbors(const Vertex &vertex) const
    {
        return getInNeighbors(vertex.getId());
    }

    std::map<int, std::map<float, EdgeProperty<U>>> getOutEdges(int vertex) const
    {
        std::set<int> s;
        for (const auto &neighbor : adjacencyList[vertex])
        {
            s.insert(neighbor.first);
        }
        std::map<int, std::map<float, EdgeProperty<U>>> outEdges;
        for (int num : s)
        {
            float weight = adjacencyList[vertex].at(num);
            EdgeProperty<U> edgeProperty = getEdgeProperty(vertex, num);
            std::map<float, EdgeProperty<U>> edgeInfo;
            edgeInfo[weight] = edgeProperty;
            outEdges[num] = edgeInfo;
        }
        return outEdges;
    }

    std::map<int, std::map<float, EdgeProperty<U>>> getOutEdges(const Vertex &vertex) const
    {
        return getOutEdges(vertex.getId());
    }

    std::map<int, std::map<float, EdgeProperty<U>>> getInEdges(int vertex) const
    {
        std::map<int, std::map<float, EdgeProperty<U>>> inEdges;
        for (int i = 0; i < size(); i++)
        {
            auto it = adjacencyList[i].find(vertex);
            if (it != adjacencyList[i].end())
            {
                float weight = it->second;
                EdgeProperty<U> edgeProperty = getEdgeProperty(i, vertex);
                std::map<float, EdgeProperty<U>> edgeInfo;
                edgeInfo[weight] = edgeProperty;
                inEdges[i] = edgeInfo;
            }
        }
        return inEdges;
    }

    std::map<int, std::map<float, EdgeProperty<U>>> getInEdges(const Vertex &vertex) const
    {
        return getInEdges(vertex.getId());
    }

    // Method to get all vertices
    std::vector<std::pair<int, VertexProperty<T>>> getAllVertices() const
    {
        std::vector<std::pair<int, VertexProperty<T>>> verticesList;
        for (int i = 0; i < size(); ++i)
        {
            verticesList.emplace_back(i, getVertexProperty(i));
        }
        return verticesList;
    }

    // Method to calculate the maximum total edge weight between two vertices
    inline int calculateMaxTotalEdgeWeight(int source, int target) const
    {
        // Initialize a vector to store the maximum total edge weight for each vertex
        std::vector<int> maxWeight(size() + 2, numeric_limits<int>::min());
        maxWeight[source] = 0;
        // Initialize a queue for breadth-first search
        std::queue<int> q;
        q.push(source);
        while (!q.empty())
        {
            int current = q.front();
            q.pop();
            for (int neighbor : getOutNeighbors(current))
            {
                // Calculate the maximum total edge weight to the neighbor
                int weightToNeighbor = maxWeight[current] + getEdgeWeight(current, neighbor);
                // Update the maximum total edge weight for the neighbor if the new weight is greater
                if (weightToNeighbor > maxWeight[neighbor])
                {
                    maxWeight[neighbor] = weightToNeighbor;
                    q.push(neighbor);
                }
            }
        }
        // Return the maximum total edge weight from the source to the target
        return maxWeight[target];
    }

    // 尋找起點集合
    set<int> getSourceSet() const
    {
        set<int> sourceSet;
        for (int i = 0; i < size(); i++)
        {
            if (getInNeighbors(i).empty())
            {
                sourceSet.insert(i);
            }
        }
        return sourceSet;
    }

    // 尋找終點集合
    set<int> getTargetSet() const
    {
        set<int> targetSet;
        for (int i = 0; i < size(); i++)
        {
            if (getOutNeighbors(i).empty())
            {
                targetSet.insert(i);
            }
        }
        return targetSet;
    }

    // Method to rotate the Hblock
    void rotateBlock(int vertex, bool isVertical)
    {
        vertexPropertiesMap[vertex].value->rotate();
        int width = vertexPropertiesMap[vertex].value->getWidth();
        int height = vertexPropertiesMap[vertex].value->getHeight();
        //cout << "width: " << width << " height: " << height << endl;

        // vertex的width和height對調
        if (isVertical)
        {
            for (int neighbor : getOutNeighbors(vertex))
            {
                adjacencyList[vertex][neighbor] = height;
                //cout << vertex << ": " << neighbor << "(" << adjacencyList[vertex][neighbor] << ")" << endl;
            }
        }
        else
        {
            for (int neighbor : getOutNeighbors(vertex))
            {
                adjacencyList[vertex][neighbor] = width;
                //cout << vertex << ": " << neighbor << "(" << adjacencyList[vertex][neighbor] << ")" << endl;
            }
        }
    }

    void rotateBlock(const Vertex &vertex, bool isVertical)
    {
        rotateBlock(vertex.getId(), isVertical);
    }

    void printGraph()
    {
        for (int i = 0; i < size() + 2; i++)
        {
            cout << "Node " << i << ": ";
            for (auto &neighbor : getOutNeighbors(i))
            {
                cout << neighbor << "(" << getEdgeWeight(i, neighbor) << ") ";
            }
            cout << endl;
        }
    }

    // Method to get size
    int size() const
    {
        return adjacencyList.size() - 2;
    }
};

#endif // HV_GRAPH_HPP
