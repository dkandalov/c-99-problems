#include <vector>
#include <stdlib.h>
#include <iostream>

using std::vector;
using std::string;
using std::tuple;
using std::make_tuple;

namespace KnightsTour {
    bool isValid(vector<tuple<int, int>> path) {
        return false; // TODO
    }

    vector<tuple<int, int>> allMovesFrom(tuple<int, int> position) {
        return {}; // TODO
    }

    vector<tuple<int, int>> findKnightsPath(int boardSize, vector<tuple<int, int>> path) {
        if (path.size() == boardSize) return {path};
        vector<tuple<int, int>> result;

        auto position = path.back();
        for (auto newPosition : allMovesFrom(position)) {
            auto pathCopy = vector<tuple<int, int>>(path);
            pathCopy.push_back(newPosition);
            if (isValid(pathCopy)) {
                auto subResults = findKnightsPath(boardSize, pathCopy);
                result.insert(result.end(), subResults.begin(), subResults.end());
            }
        }
        return result;
    }

    vector<tuple<int, int>> findKnightsPath(int boardSize) {
        return findKnightsPath(boardSize, {make_tuple(0, 0)});
    }
}

namespace EightQueens {
    string asString(vector<int> solution) {
        string s;
        for (int col = 0; col < solution.size(); col++) {
            for (int row = 0; row < solution.size(); row++) {
                if (solution[col] == row) s += "Q"; else s += "-";
            }
            s += "\n";
        }
        return s;
    }

    bool isValid(vector<int> solution) {
        for (int col1 = 0; col1 < solution.size(); col1++) {
            int row1 = solution[col1];
            for (int col2 = 0; col2 < solution.size(); col2++) {
                int row2 = solution[col2];
                if (col1 == col2) continue;
                if (row1 == row2) return false;
                if (abs(row1 - row2) == abs(col1 - col2)) return false;
            }
        }
        return true;
    }

    vector<vector<int>> solveEightQueensProblem(int boardSize, vector<int> solution) {
        if (solution.size() == boardSize) return { solution };

        vector<vector<int>> result;
        for (int row = 0; row < boardSize; row++) {
            vector<int> solutionCopy(solution);
            solutionCopy.push_back(row);
            if (isValid(solutionCopy)) {
                auto subResult = solveEightQueensProblem(boardSize, solutionCopy);
                result.insert(result.end(), subResult.begin(), subResult.end());
            }
        }
        return result;
    }

    vector<vector<int>> solveEightQueensProblem(int boardSize) {
        return solveEightQueensProblem(boardSize, {});
    }
}
