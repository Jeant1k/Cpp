#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

class BipartiteGraph {
public:
    BipartiteGraph(int n) : n(n + 1), adj(n + 1), color(n + 1, -1) {}

    void addEdge(int u, int v) {

        adj[u].insert(v);
        adj[v].insert(u);

        if (color[u] == -1 && color[v] == -1) {
            color[u] = 0;
            color[v] = 1;
        } else if (color[u] == -1) {
            color[u] = 1 - color[v];
        } else if (color[v] == -1) {
            color[v] = 1 - color[u];
        }

    }

    void enterGraph() {
        int m;
        cin >> m;
        int a, b;
        for (int i = 0; i < m; ++i) {
            cin >> a >> b;
            addEdge(a, b);
        }
    }

    const set<int>& getAdjVertices(int u) const { return adj[u]; }
    const int getColor(int u) const { return color[u]; }
    const int getNumVertices() const { return n; }

private:
    int n;
    vector<set<int>> adj;
    vector<int> color;
};

class KuhnAlgorithm {
public:
    KuhnAlgorithm(BipartiteGraph& g) 
            : n(g.getNumVertices()) 
            , graph(g) 
            , mt(g.getNumVertices(), -1) 
            , used(g.getNumVertices(), false) 
        {}

    void run() {

        for (int i = 0; i < n; ++i) {
            if (!graph.getColor(i)) {
                used.assign(n, false);
                tryKuhn(i);
            }
        }

        printResult();

    }

private:
    bool tryKuhn(int n) {

        if (used[n]) return false;

        used[n] = true;
        for (auto& to : graph.getAdjVertices(n)) {
            if (mt[to] == -1 || tryKuhn(mt[to])) {
                mt[to] = n;
                return true;
            }
        }

        return false;

    }

    void printResult() {

        map<int, int> result;
        for (int i = 1; i < n; ++i)
            if (mt[i] != -1)
                result[min(mt[i], i)] = max(mt[i], i);
        
        cout << result.size() << endl;
        for (auto& r : result)
            cout << r.first << ' ' << r.second << endl;
        
    }

    int n;
    BipartiteGraph graph;
    vector<int> mt;
    vector<bool> used;
};

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int n;
    cin >> n;

    BipartiteGraph graph(n);
    graph.enterGraph();

    KuhnAlgorithm algorithm(graph);
    algorithm.run();

    return 0;

}
