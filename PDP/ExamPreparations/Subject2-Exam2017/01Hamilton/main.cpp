#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
using namespace std;
ifstream fin("graf.in");

#define N 5
vector<int> graf[N + 5];

void showIfHamiltonianCycle(int next, vector<int> &path, vector<bool> &visited) {
    if(next != 0 || path.size() != N) {
        return;
    }

    for(auto node: path) {
        cout << node << " ";
    }

    cout << "\n";
}

void dfs(int node, vector<int> path, vector<bool> visited) {
    bool first = true;
    vector<thread> threads;

    for(auto next: graf[node]) {
        showIfHamiltonianCycle(next, path, visited);

        if(!visited[next]) {
            if(first) {
                first = false;
                visited[next] = true;
                path.push_back(next);

                dfs(next, path,visited);

                visited[next] = false;
                path.pop_back();
            } else {

                visited[next] = true;
                path.push_back(next);

                threads.emplace_back( [visited, path, next](){
                    dfs(next, path, visited);
                } );

                visited[next] = false;
                path.pop_back();
            }

        }
    }

    for(int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
}

void dfsT(int node, vector<int> path, vector<bool> visited, int threads) {
    bool first = true;

    if(threads == 1) {
        for(auto next: graf[node]) {
            showIfHamiltonianCycle(next, path, visited);

            if(!visited[next]) {
                first = false;
                visited[next] = true;
                path.push_back(next);

                dfsT(next, path,visited, threads);

                visited[next] = false;
                path.pop_back();
            }
        }
    } else {
        // Another thread
        auto t = thread([node, visited, first, path, threads]() mutable {
            for(int i = 0; i < graf[node].size(); i += 2) {
                int next = graf[node][i];

                if(!visited[next]) {
                    first = false;
                    visited[next] = true;
                    path.push_back(next);

                    dfsT(next, path,visited, threads / 2);

                    visited[next] = false;
                    path.pop_back();
                }
            }
        });

        // Crt thread
        for(int i = 1; i < graf[node].size(); i += 2) {
            int next = graf[node][i];

            if(!visited[next]) {
                first = false;
                visited[next] = true;
                path.push_back(next);

                dfsT(next, path,visited, threads / 2);

                visited[next] = false;
                path.pop_back();
            }
        }

        t.join();
    }
}

int main() {
    int m;
    fin >> m;
    for(int i = 1; i <= m; i++) {
        int x, y;
        fin >> x >> y;
        graf[x].push_back(y);
    }

    vector<int> path;
    vector<bool> visited(N + 5, false);
    int start = 0;
    path.push_back(start);
    visited[start] = true;
    dfsT(start, path, visited, 4);
    return 0;
}