#include <bits/stdc++.h>

using namespace std;

static int K;
static vector<int> goalGrid;   
static vector<int> goalRow;   
static vector<int> goalCol;   
int dr[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

inline int IDX(int r, int c) { return r * K + c; }

void setupGoal(int k) {
    K = k;
    goalGrid.assign(k * k, 0);
    goalRow.assign(k * k, 0);
    goalCol.assign(k * k, 0);
    for (int i = 0; i < k * k - 1; i++) goalGrid[i] = i + 1;
    goalGrid[k * k - 1] = 0; // blank goes last
    for (int r = 0; r < K; r++)
        for (int c = 0; c < K; c++) {
            int v = goalGrid[IDX(r, c)];
            goalRow[v] = r;
            goalCol[v] = c;
        }
}

using Grid = vector<int>;

double hHamming(const Grid& g) {
    int count = 0;
    for (int i = 0; i < (int)g.size(); i++)
        if (g[i] != 0 && g[i] != goalGrid[i]) count++;
    return count;
}

double hManhattan(const Grid& g) {
    int d = 0;
    for (int r = 0; r < K; r++)
        for (int c = 0; c < K; c++) {
            int v = g[IDX(r, c)];
            if (v == 0) continue;
            d += abs(r - goalRow[v]) + abs(c - goalCol[v]);
        }
    return d;
}

double hEuclidean(const Grid& g) {
    double d = 0.0;
    for (int r = 0; r < K; r++)
        for (int c = 0; c < K; c++) {
            int v = g[IDX(r, c)];
            if (v == 0) continue;
            double dr = r - goalRow[v], dc = c - goalCol[v];
            d += sqrt(dr * dr + dc * dc);
        }
    return d;
}

double hLinearConflict(const Grid& g) {
    double d = hManhattan(g);
    int conflicts = 0;
    for (int r = 0; r < K; r++) {
        vector<int> goalColsInOrder;
        for (int c = 0; c < K; c++) {
            int v = g[IDX(r, c)];
            if (v != 0 && goalRow[v] == r) goalColsInOrder.push_back(goalCol[v]);
        }
        for (uint64_t i = 0; i < goalColsInOrder.size(); i++)
            for (uint64_t j = i + 1; j < goalColsInOrder.size(); j++)
                if (goalColsInOrder[i] > goalColsInOrder[j]) conflicts++;
    }
    for (int c = 0; c < K; c++) {
        vector<int> goalRowsInOrder;
        for (int r = 0; r < K; r++) {
            int v = g[IDX(r, c)];
            if (v != 0 && goalCol[v] == c) goalRowsInOrder.push_back(goalRow[v]);
        }
        for (uint64_t i = 0; i < goalRowsInOrder.size(); i++)
            for (uint64_t j = i + 1; j < goalRowsInOrder.size(); j++)
                if (goalRowsInOrder[i] > goalRowsInOrder[j]) conflicts++;
    }
    return d + 2.0 * conflicts;
}

int lisLength(const vector<int>& seq) {
    vector<int> tails;
    for (int x : seq) {
        auto it = lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    return (int)tails.size();
}

double hCustom(const Grid& g) {
    double d = hManhattan(g);
    int extraTiles = 0;
    for (int r = 0; r < K; r++) {
        vector<int> goalColsInOrder;
        for (int c = 0; c < K; c++) {
            int v = g[IDX(r, c)];
            if (v != 0 && goalRow[v] == r) goalColsInOrder.push_back(goalCol[v]);
        }
        extraTiles += (int)goalColsInOrder.size() - lisLength(goalColsInOrder);
    }
    for (int c = 0; c < K; c++) {
        vector<int> goalRowsInOrder;
        for (int r = 0; r < K; r++) {
            int v = g[IDX(r, c)];
            if (v != 0 && goalCol[v] == c) goalRowsInOrder.push_back(goalRow[v]);
        }
        extraTiles += (int)goalRowsInOrder.size() - lisLength(goalRowsInOrder);
    }
    return d + 2.0 * extraTiles;
}

enum class Heuristic { HAMMING, MANHATTAN, EUCLIDEAN, LINEAR_CONFLICT, CUSTOM };

double computeH(Heuristic h, const Grid& g) {
    if (h == Heuristic::HAMMING) return hHamming(g);
    if (h == Heuristic::MANHATTAN) return hManhattan(g);
    if (h == Heuristic::EUCLIDEAN) return hEuclidean(g);
    if (h == Heuristic::LINEAR_CONFLICT) return hLinearConflict(g);
    if (h == Heuristic::CUSTOM) return hCustom(g);
    return 0.0;
}

bool isSolvable(const Grid& g) {
    vector<int> seq;
    seq.reserve(g.size() - 1);
    int blankRowFromTop = -1;
    for (int r = 0; r < K; r++)
        for (int c = 0; c < K; c++) {
            int v = g[IDX(r, c)];
            if (v == 0) blankRowFromTop = r;
            else seq.push_back(v);
        }
    long long inversions = 0;
    for (uint64_t i = 0; i < seq.size(); i++)
        for (uint64_t j = i + 1; j < seq.size(); j++)
            if (seq[i] > seq[j]) inversions++;

    if (K % 2 == 1) {
        return inversions % 2 == 0;
    } else {
        int blankRowFromBottom = K - blankRowFromTop;
        if (blankRowFromBottom % 2 == 0) {
            return inversions % 2 == 1;
        } else {
            return inversions % 2 == 0;
        }
    }
}

struct SearchNode {
    Grid grid;
    int parent;
    int g;
    double h;
    double f;
};

struct VecHash {
    uint64_t operator()(const vector<int>& v) const {
        uint64_t seed = v.size();
        for (int x : v)
            seed ^= hash<int>{}(x) + 0x9e3779b9u + (seed << 6) + (seed >> 2);
        return seed;
    }
};

struct PQItem {
    double f;
    double h;  
    int nodeIdx;

    bool operator<(const PQItem& other) const {
        if (f != other.f) return f > other.f;
        return h > other.h;
    }
};

vector<Grid> getNeighbors(const Grid& g) {
    vector<Grid> result;
    int zeroIdx = -1;
    for (int i = 0; i < (int)g.size(); i++) if (g[i] == 0) { zeroIdx = i; break; }
    int r = zeroIdx / K, c = zeroIdx % K;
    for (int d = 0; d < 4; d++) {
        int nr = r + dr[d], nc = c + dc[d];
        if (nr < 0 || nr >= K || nc < 0 || nc >= K) continue;
        Grid ng = g;
        swap(ng[zeroIdx], ng[IDX(nr, nc)]);
        result.push_back(move(ng));
    }
    return result;
}

vector<Grid> solveAStar(const Grid& start, Heuristic heur, double W, long long& nodesExpanded) {
    vector<SearchNode> pool;
    pool.reserve(1 << 16);
    
    priority_queue<PQItem> pq;
    unordered_map<Grid, int, VecHash> bestG;
    unordered_set<Grid, VecHash> closed;

    double h0 = computeH(heur, start);
    pool.push_back({start, -1, 0, h0, 0.0 + W * h0});
    bestG[start] = 0;
    pq.push({pool[0].f, pool[0].h, 0});

    nodesExpanded = 0;

    while (!pq.empty()) {
        PQItem top = pq.top();
        pq.pop();
        int curIdx = top.nodeIdx;
        const Grid& curGrid = pool[curIdx].grid;

        if (closed.count(curGrid)) continue;
        auto bIt = bestG.find(curGrid);
        if (bIt != bestG.end() && pool[curIdx].g != bIt->second) continue;

        closed.insert(curGrid);
        nodesExpanded++;

        if (curGrid == goalGrid) {
            vector<Grid> path;
            int idx = curIdx;
            while (idx != -1) {
                path.push_back(pool[idx].grid);
                idx = pool[idx].parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        int curG = pool[curIdx].g;
        for (auto& ng : getNeighbors(curGrid)) {
            if (closed.count(ng)) continue;
            int newG = curG + 1;
            auto it = bestG.find(ng);
            if (it == bestG.end() || newG < it->second) {
                bestG[ng] = newG;
                double h = computeH(heur, ng);
                double f = newG + W * h;
                pool.push_back({ng, curIdx, newG, h, f});
                pq.push({f, h, (int)pool.size() - 1});
            }
        }
    }
    return {};
}

int main() {
    Heuristic heur = Heuristic::LINEAR_CONFLICT;
    
    vector<double> weights = {1.0, 1.2, 2.0, 5.0};

    ifstream inFile("input.txt");
    ofstream outFile("output.txt");

    istream& in = inFile.is_open() ? inFile : cin;
    ostream& out = outFile.is_open() ? outFile : cout;

    if (&in == &cin) {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
    }

    int k;
    int caseNum = 1;
    
    while (in >> k) {
        setupGoal(k);

        Grid start(k * k);
        for (int i = 0; i < k * k; i++) {
            in >> start[i];
        }

        out << "========================================\n";
        out << "              Test Case " << caseNum++ << "\n";
        out << "========================================\n";
        
        if (!isSolvable(start)) {
            out << "Unsolvable puzzle\n\n";
            continue;
        }

        for (double W : weights) {
            long long nodesExpanded = 0;
            vector<Grid> path = solveAStar(start, heur, W, nodesExpanded);
            int cost = path.size() - 1;

            out << "[ W = " << W << " ] Cost: " << cost 
                << " | Nodes Expanded: " << nodesExpanded << "\n";

            // if (W == 1.0) {
            //     out << "\n--- Path for W = 1.0 ---\n";
            //     for (auto& g : path) {
            //         for (int r = 0; r < k; r++) {
            //             for (int c = 0; c < k; c++) {
            //                 out << g[IDX(r, c)];
            //                 if (c + 1 < k) out << ' ';
            //             }
            //             out << "\n";
            //         }
            //         out << "\n";
            //     }
            //     out << "------------------------\n";
            // }
        }
        out << "\n";
    }

    return 0;
}