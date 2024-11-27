#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <climits>
#include <sstream>
#include <ctime>

using namespace std;

void primAdjList(const vector<vector<pair<int, int>>>& adjList, int start, ofstream& outputFile) {
    int start_s=clock();
    int n = adjList.size();
    int costL = 0;
    vector<int> key(n, INT_MAX);
    vector<int> parent(n, -1);
    vector<bool> inMST(n, false);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    key[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (inMST[u]) continue;
        inMST[u] = true;

        for (const auto& edge : adjList[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }
    int stop_s=clock();

    outputFile << "Running Prim's algorithm on Adjacency List" << endl;
    outputFile << "MST using Adjacency List (Prims):" << endl;
    for (int i = 0; i < n; ++i) {
        if (parent[i] != -1) {
            outputFile << "Edge: " << parent[i] << " - " << i << " (Weight: " << key[i] << ")" << endl;
            costL += key[i];
        }
    }
    outputFile << "Cost of MST using Adjacency List: " << costL << endl;
    outputFile << "Time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << " seconds" << endl;
}


void primAdjMatrix(const vector<vector<int>>& adjMatrix, int start, ofstream& outputFile) {
    int start_s=clock();
    int n = adjMatrix.size();
    int costM = 0;
    vector<int> key(n, INT_MAX);
    vector<int> parent(n, -1);
    vector<bool> inMST(n, false);
    key[start] = 0;

    for (int i = 0; i < n - 1; ++i) {
        int u = -1;
        for (int j = 0; j < n; ++j) {
            if (!inMST[j] && (u == -1 || key[j] < key[u])) u = j;
        }

        inMST[u] = true;

        for (int v = 0; v < n; ++v) {
            if (adjMatrix[u][v] && !inMST[v] && adjMatrix[u][v] < key[v]) {
                key[v] = adjMatrix[u][v];
                parent[v] = u;
            }
        }
    }
    int stop_s=clock();
    outputFile << "Running Prim's algorithm on Adjacency Matrix" << endl;
    outputFile << "MST using Adjacency Matrix (Prims):" << endl;
    for (int i = 0; i < n; ++i) {
        if (parent[i] != -1) {
            outputFile << "Edge: " << parent[i] << " - " << i << " (Weight: " << key[i] << ")" << endl;
            costM += key[i];
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

    int nodes = 10000;
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
    primAdjList(adjList,source, outputFile);
    primAdjMatrix(adjMatrix,source, outputFile);
    outputFile.close();
    return 0;
}
