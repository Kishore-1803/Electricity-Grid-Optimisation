#include <iostream>
#include <vector>
#include <fstream>
#include <climits>
#include <string>
#include <tuple>
#include <sstream>
#include <ctime>

using namespace std;

void bellmanFord(const vector<tuple<int, int, int>>& edges, int nodes, int source, ofstream& outputFile) {
    int start_s = clock();
    vector<int> dist(nodes, INT_MAX);
    dist[source] = 0;

    for (int i = 0; i < nodes - 1; ++i) {
        for (const auto& edge : edges) {
            int u = get<0>(edge);
            int v = get<1>(edge);
            int weight = get<2>(edge);
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    for (const auto& edge : edges) {
        int u = get<0>(edge);
        int v = get<1>(edge);
        int weight = get<2>(edge);
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            outputFile << "Graph contains a negative weight cycle" << endl;
            return;
        }
    }
     int stop_s = clock();

    outputFile << "Shortest paths from source " << source << " using Bellman-Ford:" << endl;
    for (int i = 0; i < nodes; ++i) {
        outputFile << "Node " << i << ", Distance: " << dist[i] << endl;
    }
    outputFile << "Time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << " seconds" << endl;
}

void loadEdgesFromAdjList(const vector<vector<pair<int, int>>>& adjList, vector<tuple<int, int, int>>& edges) {
    for (size_t u = 0; u < adjList.size(); ++u) {
        for (const auto& neighbor : adjList[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;
            edges.emplace_back(u, v, weight);
        }
    }
}

void loadEdgesFromAdjMatrix(const vector<vector<int>>& adjMatrix, vector<tuple<int, int, int>>& edges) {
    for (size_t u = 0; u < adjMatrix.size(); ++u) {
        for (size_t v = 0; v < adjMatrix[u].size(); ++v) {
            if (adjMatrix[u][v] != 0) {
                edges.emplace_back(u, v, adjMatrix[u][v]);
            }
        }
    }
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
        char colon;
        ss >> u >> colon; // Read the node and the colon
        if (ss.fail()) {
            cerr << "Error parsing line: " << line << endl;
            continue;
        }
        
        while (ss.peek() != EOF) {
            char ignoreChar;
            int v, w;
            ss >> ignoreChar >> v >> ignoreChar >> w >> ignoreChar; 
            if (ss.fail()) {
                break; // Exit the loop if parsing fails
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
    int nodes = 4;
    int source = 0;
    vector<vector<pair<int, int>>> adjList(nodes);
    vector<vector<int>> adjMatrix(nodes, vector<int>(nodes, 0));
    vector<tuple<int, int, int>> edges;

    loadGraphAL(adjList, graphALFilename);
    loadGraphAM(adjMatrix, graphAMFilename);
    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cerr << "Error opening output file: " << outputFilename << endl;
        return 1;
    }
    loadEdgesFromAdjList(adjList, edges);
    outputFile << "Running Bellman-Ford algorithm on Adjacency List representation" << endl;
    bellmanFord(edges, nodes, source, outputFile);
    edges.clear();
    loadEdgesFromAdjMatrix(adjMatrix, edges);
    outputFile << "Running Bellman-Ford algorithm on Adjacency Matrix representation" << endl;
    bellmanFord(edges, nodes, source, outputFile);
    outputFile.close();
    return 0;
}