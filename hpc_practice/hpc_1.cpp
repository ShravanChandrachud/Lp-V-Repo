#include <iostream>
#include <stack>
#include <queue>
#include <omp.h>

using namespace std;

class Graph
{
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int v, int w)
    {
        adj[v].push_back(w);
    }

    void parallelDfs(int start)
    {
        vector<bool> visited(V, false);
        parallelDfsUtil(start, visited);
    }
    void parallelDfsUtil(int v, vector<bool> &visited)
    {
        visited[v] = true;
        cout << v << " ";
#pragma omp parallel for
        for (int i = 0; i < adj[v].size(); i++)
        {
            int n = adj[v][i];
            if (!visited[n])
                parallelDfsUtil(n, visited);
        }
    }

    void parallelBfs(int start)
    {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        while (!q.empty())
        {
            int v = q.front();
            q.pop();
            cout << v << " ";
#pragma omp parallel for
            for (int i = 0; i < adj[v].size(); i++)
            {
                int n = adj[v][i];
                if (!visited[n])
                {
                    visited[n] = true;
                    q.push(n);
                }
            }
        }
    }
};

int main()
{
    int V, E;
    cout << "Enter number of verices: ";
    cin >> V;
    Graph g(V);
    cout << "Enter number of edges: ";
    cin >> E;

    cout << "Enter each edge in the format: start_vertex to_vertex" << endl;

    for (int i = 0; i < E; i++)
    {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int startVertex;
    cout << "Enter start vertex for DFS and BFS: ";
    cin >> startVertex;

    cout << "Depth-First Search (DFS): ";
    g.parallelDfs(startVertex);
    cout << endl;

    cout << "Breadth-First Search (BFS): ";
    g.parallelBfs(startVertex);
    cout << endl;
}