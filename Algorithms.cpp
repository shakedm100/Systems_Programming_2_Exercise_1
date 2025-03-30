/*
shaked1mi@gmail.com
Shaked Michael
318772191
*/

#include "Algorithms.hpp"
#include "Graph.hpp"
#include <bits/stdc++.h>

using namespace std;

/**
 * @brief Performs a Breadth-First Search (BFS) on the graph.
 * @param graph The input graph.
 * @param start The starting vertex.
 * @return A spanning tree (Graph) generated by BFS.
 */
Graph BFS(const Graph& graph, const Vertex start)
{
    int vertices = graph.getVertices();
    Queue queue(vertices);
    int* parent = new int[vertices];
    bool* visited = new bool[vertices];
    int* weights = new int[vertices];

    for (size_t i = 0; i < vertices; i++)
    {
        visited[i] = false;
        parent[i] = -1;
        weights[i] = 0;
    }

    const Vertex* const* adj = graph.getAdj();
    visited[start.vertex] = true;
    queue.enqueue(start.vertex);

    while (!queue.isEmpty())
    {
        int curr = queue.peek();
        queue.dequeue();

        const Vertex* currentEdge = adj[curr];
        while (currentEdge != nullptr)
        {
            int neighbor = currentEdge->vertex;
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                parent[neighbor] = curr; 
                weights[neighbor] = currentEdge->data;
                queue.enqueue(neighbor);
            }
            currentEdge = currentEdge->next;
        }
    }

    Graph tree(vertices);
    for (size_t i = 0; i < vertices; i++)
    {
        if(parent[i] != -1)
        {
            tree.addEdge(parent[i], i, weights[i]);
        }
    }

    delete[] weights;
    delete[] parent;
    delete[] visited;

    return tree;
}

/**
 * @brief Recursive helper function for Depth-First Search (DFS).
 * @param graph The input graph.
 * @param start The current vertex index.
 * @param visited Boolean array marking visited vertices.
 * @param DFSTree Graph to store the DFS spanning tree.
 */
void DFSHelper(const Graph& graph, int start, bool* visited, Graph& DFSTree)
{
    visited[start] = true;

    for (int neighbor = 0; neighbor < graph.getVertices(); neighbor++) {
        if (!visited[neighbor] && graph.hasEdge(start, neighbor))
        {
            DFSTree.addEdge(start, neighbor, graph.getWeight(start, neighbor));
            DFSHelper(graph, neighbor, visited, DFSTree);
        }
    }
}

/**
 * @brief Performs a Depth-First Search (DFS) on the graph.
 * @param g The input graph.
 * @param start The starting vertex index.
 * @return A spanning tree (Graph) generated by DFS.
 */
Graph DFS(const Graph& g, int start) 
{
    bool* visited = new bool[g.getVertices()];
    Graph DFSTree(g.getVertices());

    for (size_t i = 0; i < g.getVertices(); i++)
    {
        visited[i] = false;
    }
    
    DFSHelper(g, start, visited, DFSTree);

    delete[] visited;

    return DFSTree;
}

/**
 * @brief Finds the vertex with the minimum distance that hasn't been finalized.
 * @param dist Array of distances.
 * @param shortestPaths Boolean array marking vertices with finalized shortest paths.
 * @param vertices Number of vertices.
 * @return Index of the vertex with the smallest distance.
 */
int minDist(int* dist, bool* shortestPaths, int vertices)
{
    int min = INT_MAX;
    int index = -1;

    for (int i = 0; i < vertices; i++) {
        if (!shortestPaths[i] && dist[i] < min) {
            min = dist[i];
            index = i;
        }
    }
    
    return index;
}

/**
 * @brief Relaxes the edge from vertex u to vertex v.
 * Updates the distance and predecessor if a shorter path is found.
 * @param graph The input graph.
 * @param u The source vertex index.
 * @param v The destination vertex index.
 * @param distances Array of current shortest distances.
 * @param prev Array of previous vertex indices.
 */
void relax(const Graph& graph, int u, int v, int* distances, int* prev)
{
    if (graph.hasEdge(u, v) && distances[u] != INT_MAX) 
    {
        int weight = graph.getWeight(u, v);
        if (distances[u] + weight < distances[v]) 
        {
            distances[v] = distances[u] + weight;
            prev[v] = u;
        }
    }
}

/**
 * @brief Finds the index of the minimum key value not yet included in the MST.
 * @param key Array of key values.
 * @param mstSet Boolean array marking vertices included in the MST.
 * @param vertices Number of vertices.
 * @return Index of the vertex with the smallest key value.
 */
int minKey(int key[], bool mstSet[], int vertices)
{
    int min = INT_MAX, min_index = -1;

    for (size_t v = 0; v < vertices; v++)
    {
        if (!mstSet[v] && key[v] < min)
        {
            min = key[v];
            min_index = v;
        }
    }

    return min_index;
}

/**
 * @brief Implements Dijkstra's algorithm to compute shortest paths.
 * @param graph The input graph.
 * @param start The starting vertex index.
 * @return A graph representing the shortest path tree.
 */
Graph dijkstra(const Graph& graph, int start)
{
    int vertices = graph.getVertices();
    int* distances = new int[vertices];
    bool* shortestPaths = new bool[vertices];
    int* prev = new int[vertices];
    Graph shortestPathTree(vertices);

    for (size_t i = 0; i < vertices; i++)
    {
        distances[i] = INT_MAX;
        shortestPaths[i] = false;
        prev[i] = -1;
    }
    
    distances[start] = 0;

    for (size_t i = 0; i < vertices-1; i++)
    {
        int u = minDist(distances, shortestPaths, vertices);
        if(u == -1)
            break;
        
        shortestPaths[u] = true;
        for (int v = 0; v < vertices; v++) 
        {
            relax(graph, u, v, distances, prev);
        }
    }

    for (size_t v = 0; v < vertices; v++)
    {
        if (v != start && prev[v] != -1) 
        {
            shortestPathTree.addEdge(prev[v], v, graph.getWeight(prev[v], v));
        }
    }

    delete[] distances;
    delete[] shortestPaths;
    delete[] prev;

    return shortestPathTree;
}

/**
 * @brief Constructs a Minimum Spanning Tree (MST) using Prim's algorithm.
 * @param graph The input graph.
 * @return A graph representing the MST.
 */
Graph primMST(const Graph& graph)
{
    int vertices = graph.getVertices();
    int* parent = new int[vertices];
    int* key = new int[vertices];
    bool* mstSet = new bool[vertices];

    for (size_t i = 0; i < vertices; i++)
    {
        key[i] = INT_MAX;
        mstSet[i] = false;
        parent[i] = -1;
    }
    
    key[0] = 0;

    for (size_t i = 0; i < vertices - 1; i++)
    {
        int u = minKey(key, mstSet, vertices);
        if(u == -1)
            break;

        mstSet[u] = true;

        for (size_t v = 0; v < vertices; v++)
        {
            int currentWeight = graph.getWeight(u, v);
            if (graph.hasEdge(u, v) && !mstSet[v] && currentWeight < key[v])
            {
                parent[v] = u;
                key[v] = currentWeight;
            }
        }   
    }

    Graph minSpanningTree(vertices);
    for (int v = 1; v < vertices; v++) // Skip root (vertex 0)
    {
        if (parent[v] != -1)
        {
            minSpanningTree.addEdge(parent[v], v, graph.getWeight(parent[v], v));
        }
    }

    delete[] parent;
    delete[] key;
    delete[] mstSet;

    return minSpanningTree;
}

/**
 * @brief Swaps the values of two Edge structures.
 * @param a The first edge.
 * @param b The second edge.
 */
void swapEdges(Edge& a, Edge& b) 
{
    Edge temp = a;
    a = b;
    b = temp;
}

/**
 * @brief Partitions an array of edges for quicksort based on weight.
 * @param arr Array of edges.
 * @param low Starting index.
 * @param high Ending index.
 * @return The partition index.
 */
int partition(Edge arr[], int low, int high) 
{
    int pivot = arr[high].weight;
    int i = low - 1;
    for (int j = low; j < high; j++) 
    {
        if (arr[j].weight < pivot) 
        {
            i++;
            swapEdges(arr[i], arr[j]);
        }
    }
    swapEdges(arr[i + 1], arr[high]);
    return i + 1;
}

/**
 * @brief Sorts an array of edges using the quicksort algorithm.
 * @param arr Array of edges.
 * @param low Starting index.
 * @param high Ending index.
 */
void quickSort(Edge arr[], int low, int high) 
{
    if (low < high) 
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/**
 * @brief Constructs an MST using Kruskal's algorithm.
 * @param graph The input graph.
 * @return A graph representing the MST.
 */
Graph kruskalMST(const Graph& graph) 
{
    int vertices = graph.getVertices();
    
    // Count unique edges (avoid duplicates in undirected graph)
    int edgeCount = 0;
    const Vertex* const * adj = graph.getAdj();
    for (int i = 0; i < vertices; i++) 
    {
        const Vertex* curr = adj[i];
        while (curr != nullptr) 
        {
            if (i < curr->vertex) 
            {
                edgeCount++;
            }
            curr = curr->next;
        }
    }
    
    // Store unique edges in an array
    Edge* edges = new Edge[edgeCount];
    int index = 0;
    for (int i = 0; i < vertices; i++) 
    {
        const Vertex* curr = adj[i];
        while (curr != nullptr) 
        {
            if (i < curr->vertex) 
            {
                edges[index].src = i;
                edges[index].dest = curr->vertex;
                edges[index].weight = curr->data;
                index++;
            }
            curr = curr->next;
        }
    }
    
    // Sort edges by weight
    quickSort(edges, 0, edgeCount - 1);
    
    // Use Union-Find to avoid cycles while selecting edges
    UnionFind uf(vertices);
    Graph mstGraph(vertices);
    
    int iEdge = 0; 
    int e = 0;     
    while (e < vertices - 1 && iEdge < edgeCount) 
    {
        Edge nextEdge = edges[iEdge++];
        int x = uf.find(nextEdge.src);
        int y = uf.find(nextEdge.dest);
        
        if (x != y) {
            mstGraph.addEdge(nextEdge.src, nextEdge.dest, nextEdge.weight);
            uf.unionSets(x, y);
            e++;
        }
    }
    
    delete[] edges;
    
    return mstGraph;
}