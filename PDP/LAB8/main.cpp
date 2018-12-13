#include <iostream>
#include <fstream>
#include <vector>
#include <future>
using namespace std;
ifstream fin("graf.in");

#define N 5
vector<int> graf[N + 5];

void showIfHamiltonianCycle(int node, int next, vector<int> &path, vector<bool> &visited) {
    int sz = path.size();
    if(next != 0 || path.size() != N) {
        return;
    }

    for(auto node: path) {
        cout << node << " ";
    }

    cout << "\n";
}

void dfs(int node, vector<int> &path, vector<bool> &visited) {
    bool first = true;
    vector<future<void>> futures;
    for(auto next: graf[node]) {
        showIfHamiltonianCycle(node, next, path, visited);

        if(!visited[next]) {

            if(first) {
                first = false;
                visited[next] = true;
                path.push_back(next);
                dfs(next, path,visited);
                path.pop_back();
                visited[next] = false;
            } else {
                vector<bool> copyVisited(visited);
                vector<int> copyPath(path);

                copyVisited[next] = true;
                copyPath.push_back(next);
                futures.emplace_back( std::async(dfs, next, std::ref(copyPath), std::ref(copyVisited)) );
            }

        }
    }

    for(int i = 0; i < futures.size(); i++) {
        futures[i].wait();
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
    dfs(start, path, visited);
    return 0;
}