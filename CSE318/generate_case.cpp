#include <iostream>
#include <vector>
#include <set>
#include <random>
#include <algorithm>
#include <fstream>

using namespace std;

vector<int> getValidMoves(int blankIdx) {
    int r = blankIdx / 4;
    int c = blankIdx % 4;
    vector<int> neighbors;
    
    if (r > 0) neighbors.push_back((r - 1) * 4 + c);
    if (r < 3) neighbors.push_back((r + 1) * 4 + c);
    if (c > 0) neighbors.push_back(r * 4 + (c - 1));
    if (c < 3) neighbors.push_back(r * 4 + (c + 1));
    
    return neighbors;
}

int main() {
    random_device rd;
    mt19937 gen(rd());

    set<vector<int>> uniquePuzzles;
    vector<vector<int>> orderedPuzzles;

    vector<int> goal = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0};

    while (uniquePuzzles.size() < 10) {
        vector<int> currentBoard = goal;
        int blankIdx = 15;
        int prevIdx = -1;

        for (int step = 0; step < 60; ++step) {
            vector<int> neighbors = getValidMoves(blankIdx);
            
            if (neighbors.size() > 1 && prevIdx != -1) {
                auto it = find(neighbors.begin(), neighbors.end(), prevIdx);
                if (it != neighbors.end()) {
                    neighbors.erase(it);
                }
            }

            uniform_int_distribution<> dis(0, neighbors.size() - 1);
            int nextIdx = neighbors[dis(gen)];

            swap(currentBoard[blankIdx], currentBoard[nextIdx]);
            prevIdx = blankIdx;
            blankIdx = nextIdx;
        }

        if (currentBoard != goal && uniquePuzzles.find(currentBoard) == uniquePuzzles.end()) {
            uniquePuzzles.insert(currentBoard);
            orderedPuzzles.push_back(currentBoard);
        }
    }

    ofstream outFile("input.txt");
    if (!outFile) {
        cerr << "Error: Failed to create or write to input.txt\n";
        return 1;
    }

    for (const auto& puzzle : orderedPuzzles) {
        outFile << "4\n"; 
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                outFile << puzzle[r * 4 + c];
                if (c < 3) outFile << " ";
            }
            outFile << "\n";
        }
        outFile << "\n"; 
    }
}