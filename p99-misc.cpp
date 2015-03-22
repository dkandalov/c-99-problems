#include <vector>
#include <stdlib.h>
#include <iostream>

template<typename T>
using Vector = std::vector<T>;
using String = std::string;

namespace EightQueens {
    String asString(Vector<int> solution) {
        String s;
        for (int col = 0; col < solution.size(); col++) {
            for (int row = 0; row < solution.size(); row++) {
                if (solution[col] == row) s += "Q"; else s += "-";
            }
            s += "\n";
        }
        return s;
    }

    bool isValid(Vector<int> solution) {
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

    Vector<Vector<int>> solveEightQueensProblem(int boardSize, Vector<int> solution) {
        if (solution.size() == boardSize) return { solution };

        Vector<Vector<int>> result;
        for (int row = 0; row < boardSize; row++) {
            Vector<int> solutionCopy(solution);
            solutionCopy.push_back(row);
            if (isValid(solutionCopy)) {
                auto subResult = solveEightQueensProblem(boardSize, solutionCopy);
                result.insert(result.end(), subResult.begin(), subResult.end());
            }
        }
        return result;
    }

    Vector<Vector<int>> solveEightQueensProblem(int boardSize) {
        return solveEightQueensProblem(boardSize, {});
    }
}
