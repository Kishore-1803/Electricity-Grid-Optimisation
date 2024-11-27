#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <climits>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

void dijkstraAdjList(const vector<vector<pair<int, int>>>& adjList, int source, ofstream& outputFile) {
    int start_s = clock();
    int n = adjList.size();
    vector<int> dist(n, INT_MAX);
    dist[source] = 0;

    vector<bool> visited(n, false);  
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    int stop_s = clock();
    outputFile << "Shortest paths from source " << source << " using Adjacency List (Dijkstra's):" << endl;
    for (int i = 0; i < n; ++i) {
        outputFile << "Node " << i << ", Distance: " << dist[i] << endl;
    }
    outputFile << "Time taken: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << " seconds" << endl;
}

void dijkstraAdjMatrix(const vector<vector<int>>& adjMatrix, int source, ofstream& outputFile) {
    int start_s = clock();
    int n = adjMatrix.size();
    vector<int> dist(n, INT_MAX);
    dist[source] = 0;

    vector<bool> visited(n, false);  
    for (int i = 0; i < n; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        visited[u] = true;
        if (dist[u] == INT_MAX) break;
        for (int v = 0; v < n; ++v) {
            if (adjMatrix[u][v] != 0 && dist[u] + adjMatrix[u][v] < dist[v] && !visited[v]) {
                dist[v] = dist[u] + adjMatrix[u][v];
            }
        }
    }

    int stop_s = clock();
    outputFile << "Shortest paths from source " << source << " using Adjacency Matrix (Dijkstra's):" << endl;
    for (int i = 0; i < n; ++i) {
        outputFile << "Node " << i << ", Distance: " << dist[i] << endl;
    }
    outputFile << "Time taken: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) << " seconds" << endl;
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
        ss >> u >> colon; 
        if (ss.fail()) {
            cerr << "Error parsing line: " << line << endl;
            continue;
        }
        
        while (ss.peek() != EOF) {
            char ignoreChar;
            int v, w;
            ss >> ignoreChar >> v >> ignoreChar >> w >> ignoreChar; 
            if (ss.fail()) {
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

    int nodes = 10; 
    int source = 0;
    vector<vector<pair<int, int>>> adjList(nodes);
    vector<vector<int>> adjMatrix(nodes, vector<int>(nodes, 0));
    loadGraphAL(adjList, graphALFilename);
    loadGraphAM(adjMatrix, graphAMFilename);
    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cerr << "Error opening output file: " << outputFilename << endl;
        return 1;
    }
    outputFile << "Running Dijkstras algorithm on Adjacency List" << endl;
    dijkstraAdjList(adjList, source,outputFile);
    outputFile << "\nRunning Dijkstras algorithm on Adjacency Matrix" << endl;
    dijkstraAdjMatrix(adjMatrix, source, outputFile);
    outputFile.close();
    return 0;
}