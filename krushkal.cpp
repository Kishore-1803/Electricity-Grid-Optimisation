#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <fstream>
#include <climits>
#include <algorithm>
#include <string> 
#include <sstream>
#include <ctime>

using namespace std;


struct DSU {
    vector<int> parent, rank;

    DSU(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int u) {
        if (u != parent[u]) parent[u] = find(parent[u]);
        return parent[u];
    }

    bool unionSets(int u, int v) {
        int pu = find(u), pv = find(v);
        if (pu == pv) return false;

        if (rank[pu] < rank[pv]) {
            parent[pu] = pv;
        } else if (rank[pu] > rank[pv]) {
            parent[pv] = pu;
        } else {
            parent[pv] = pu;
            rank[pu]++;
        }
        return true;
    }
};

void kruskalAdjList(const vector<vector<pair<int, int>>>& adjList , ofstream& outputFile) {
    int start_s=clock();
    int n = adjList.size();
    vector<tuple<int, int, int>> edges;
    int costL = 0;

    for (int u = 0; u < n; ++u) {
        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (u < v) {  
                edges.push_back(make_tuple(weight, u, v));
            }
        }
    }

    sort(edges.begin(), edges.end());

    DSU dsu(n);
    int stop_s=clock();
    outputFile << "Running Kruskal's algorithm on Adjacency List" << endl;
    outputFile << "MST using Adjacency List (Kruskal's):" << endl;
    for (const auto& edge : edges) {
        int weight, u, v;
        tie(weight, u, v) = edge;
        if (dsu.unionSets(u, v)) {
            outputFile << "Edge: " << u << " - " << v << " (Weight: " << weight << ")" << endl;
            costL += weight;
        }
    }
    outputFile << "Cost of MST using Adjacency List: " << costL << endl;
    outputFile << "Time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << " seconds" << endl;
}

void kruskalAdjMatrix(const vector<vector<int>>& adjMatrix, ofstream& outputFile) {
    int start_s=clock();
    int n = adjMatrix.size();
    vector<tuple<int, int, int>> edges;
    int costM = 0;

    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {  
            if (adjMatrix[u][v] != 0) {
                edges.push_back(make_tuple(adjMatrix[u][v], u, v));
            }
        }
    }

    sort(edges.begin(), edges.end());

    DSU dsu(n);
    int stop_s=clock();
    outputFile << "Running Kruskal's algorithm on Adjacency Matrix" << endl;
    outputFile << "MST using Adjacency Matrix (Kruskal's):" << endl;
    for (const auto& edge : edges) {
        int weight, u, v;
        tie(weight, u, v) = edge;
        if (dsu.unionSets(u, v)) {
            outputFile << "Edge: " << u << " - " << v << " (Weight: " << weight << ")" << endl;
            costM += weight;
        }
    }
    outputFile << "Cost of MST using Adjacency Matrix: " << costM << endl;
    outputFile << "Time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << " seconds" << endl;
}

void loadGraphAL(vector<vector<pair<int, int>>>& adjList, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int u;
        ss >> u; 
        ss.ignore(2); 
        
        while (ss.peek() != EOF) {
            char ignoreChar;
            int v, w;
            ss >> ignoreChar >> v >> ignoreChar >> w >> ignoreChar; 
            if (ss.fail()) {
                cerr << "Error parsing line: " << line << endl;
                break;
            }
            adjList[u].emplace_back(v, w);
        }
    }
    file.close();
}

void loadGraphAM(vector<vector<int>>& adjMatrix, const string& filename) {
    ifstream file(filename);
    for (auto& row : adjMatrix) {
        for (int& val : row) {
            file >> val;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: ./a.out <graph_AL_file> <graph_AM_file> <output_file>" << endl;
        return 1;
    }

    string graphALFilename = argv[1];
    string graphAMFilename = argv[2];
    string outputFilename = argv[3];

    int nodes = 5000; 
    vector<vector<pair<int, int>>> adjList(nodes);
    vector<vector<int>> adjMatrix(nodes, vector<int>(nodes, 0));
    loadGraphAL(adjList, graphALFilename);
    loadGraphAM(adjMatrix, graphAMFilename);
    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cerr << "Error opening output file: " << outputFilename << endl;
        return 1;
    }
    kruskalAdjList(adjList, outputFile);
    kruskalAdjMatrix(adjMatrix, outputFile);
    outputFile.close();
    return 0;
}
