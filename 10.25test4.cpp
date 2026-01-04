#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <set>

using namespace std;

vector<pair<int, int>> edges;
int idx[7][7];
vector<vector<int>> valid_graphs;

void init() {
    for (int i = 0; i < 7; i++) {
        for (int j = i + 1; j < 7; j++) {
            edges.push_back({i, j});
            idx[i][j] = edges.size() - 1;
            idx[j][i] = edges.size() - 1;
        }
    }

    for (int mask = 1; mask < (1 << 21); mask++) {
        vector<int> deg(7, 0);
        vector<vector<int>> adj(7);

        for (int i = 0; i < 21; i++) {
            if (mask & (1 << i)) {
                int u = edges[i].first, v = edges[i].second;
                deg[u]++;
                deg[v]++;
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }

        bool valid = true;
        int odd_deg = 0;
        for (int i = 0; i < 7; i++) {
            if (deg[i] > 2) {
                valid = false;
                break;
            }
            if (deg[i] % 2 == 1) odd_deg++;
        }
        if (!valid || odd_deg > 2) continue;

        int start = -1;
        for (int i = 0; i < 7; i++) {
            if (deg[i] > 0) {
                start = i;
                break;
            }
        }
        if (start == -1) continue;

        vector<bool> visited(7, false);
        vector<int> stack = {start};
        visited[start] = true;
        int visited_count = 0;

        while (!stack.empty()) {
            int u = stack.back();
            stack.pop_back();
            visited_count++;

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    stack.push_back(v);
                }
            }
        }

        int total_vertices = 0;
        for (int i = 0; i < 7; i++) {
            if (deg[i] > 0) total_vertices++;
        }

        if (visited_count == total_vertices) {
            vector<int> graph;
            for (int i = 0; i < 21; i++) {
                if (mask & (1 << i)) graph.push_back(i);
            }
            valid_graphs.push_back(graph);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    init();

    int T;
    cin >> T;
    while (T--) {
        int M;
        cin >> M;
        vector<int> cnt(21, 0);
        for (int i = 0; i < M; i++) {
            int x, y;
            cin >> x >> y;
            x--; y--;
            cnt[idx[x][y]]++;
        }

        long long ans = 0;
        for (auto& graph : valid_graphs) {
            long long ways = 1;
            for (int e : graph) {
                if (cnt[e] == 0) {
                    ways = 0;
                    break;
                }
                ways *= cnt[e];
            }
            ans += ways;
        }
        cout << ans << '\n';
    }

    return 0;
}
//输出还是12 6 6