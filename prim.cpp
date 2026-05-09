//All procedures including main
#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <tuple>
#include <set>
#include <string>
#include <fstream>

using namespace std;

typedef tuple<int, string, string> HeapNode;

pair<int, vector<tuple<string, int, string>>> prims(
    unordered_map<string, vector<pair<int, string>>>& graph,
    string start)
{
    set<string> visited;
    priority_queue<HeapNode, vector<HeapNode>, greater<HeapNode>> minHeap;
    vector<tuple<string, int, string>> mstEdges;
    int totalWeight = 0;

    minHeap.push({0, start, ""});

    while (!minHeap.empty()) {
        auto [weight, node, parent] = minHeap.top();
        minHeap.pop();

        if (visited.count(node)) continue;

        visited.insert(node);
        totalWeight += weight;

        if (parent != "") {
            mstEdges.push_back({parent, weight, node});
        }

        for (auto [edgeWeight, neighbor] : graph[node]) {
            if (!visited.count(neighbor)) {
                minHeap.push({edgeWeight, neighbor, node});
            }
        }
    }

    return {totalWeight, mstEdges};
}

unordered_map<string, vector<pair<int, string>>> loadGraph(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: could not open file " << filename << endl;
        exit(1);
    }

    unordered_map<string, vector<pair<int, string>>> graph;

    int numNodes, numEdges;
    file >> numNodes >> numEdges;

    for (int i = 0; i < numEdges; i++) {
        int u, v, w;
        file >> u >> v >> w;

        string su = to_string(u);
        string sv = to_string(v);

        graph[su].push_back({w, sv});
        graph[sv].push_back({w, su});  // undirected
    }

    return graph;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <graph_file>" << endl;
        return 1;
    }

    auto graph = loadGraph(argv[1]);

    // Start from node "0"
    auto [totalWeight, edges] = prims(graph, "1");

    cout << "MST total weight: " << totalWeight << endl;
    cout << "MST edges:" << endl;
    for (auto [a, b, c] : edges) {
        cout << "   " << a << " - " << c << " : " << b << endl;
    }

    return 0;
}
