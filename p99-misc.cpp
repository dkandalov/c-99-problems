#include <vector>
#include <stdlib.h>
#include <iostream>

using std::vector;
using std::string;
using std::to_string;
using std::tuple;
using std::make_tuple;
using std::get;

namespace KnightsTour {
    struct Position {
        int x;
        int y;
        Position(int const x, int const y) : x(x), y(y) {}
    };

    string asString(vector<Position>& path) {
        string s;
        for (auto position : path) {
            s += "(" + to_string(position.x) + "," + to_string(position.y) + ")";
        }
        return s;
    }

    bool isValid(vector<Position>& path, int boardSize) {
        for (auto position : path) {
            if (position.x < 0 || position.x >= boardSize) return false;
            if (position.y < 0 || position.y >= boardSize) return false;
        }
        for (auto position1 : path) {
            int count = 0;
            for (auto position2 : path) {
                if (position1.x == position2.x && position1.y == position2.y) count++;
                if (count > 1) return false;
            }
        }
        return true;
    }

    vector<Position> allMovesFrom(Position position) {
        int x = position.x;
        int y = position.y;

        auto upRight = Position(x + 1, y - 2);
        auto upLeft = Position(x - 1, y - 2);
        auto rightUp = Position(x + 2, y - 1);
        auto rightDown = Position(x + 2, y + 1);
        auto leftUp = Position(x - 2, y - 1);
        auto leftDown = Position(x - 2, y + 1);
        auto downRight = Position(x + 1, y + 2);
        auto downLeft = Position(x - 1, y + 2);

        return {upRight, upLeft, rightUp, rightDown, leftUp, leftDown, downRight, downLeft};
    }

    vector<vector<Position>> findKnightsPath(int boardSize, vector<Position>& path) {
        if (path.size() == boardSize * boardSize) return {path};
        vector<vector<Position>> result;

        auto position = path.back();
        for (auto newPosition : allMovesFrom(position)) {
            auto pathCopy = vector<Position>(path);
            pathCopy.push_back(newPosition);
            if (isValid(pathCopy, boardSize)) {
                auto subResults = findKnightsPath(boardSize, pathCopy);
                result.insert(result.end(), subResults.begin(), subResults.end());
            }
        }
        return result;
    }

    vector<vector<Position>> findKnightsPath(int boardSize) {
        vector<Position> path = {Position(0, 0)};
        return findKnightsPath(boardSize, path);
    }


    struct Frame {
        vector<Position> path;
        Frame(vector<Position> const& path) : path(path) { }
    };

    class KnightPathLazy {
    private:
        int boardSize;
        vector<Frame> frames;

    public:
        KnightPathLazy(int boardSize) : boardSize(boardSize) {
            auto frame = Frame({Position(0, 0)});
            frames = { frame };
        };

        vector<Position> nextPath() {
            vector<Position> path = {};
            while (!frames.empty() && path.size() != boardSize * boardSize) {
                auto frame = frames.back();
                frames.pop_back();

                path = frame.path;

                auto position = path.back();
                for (auto newPosition : allMovesFrom(position)) {
                    auto pathCopy = vector<Position>(path);
                    pathCopy.push_back(newPosition);
                    if (isValid(pathCopy, boardSize)) {
                        frames.push_back(Frame(pathCopy));
                    }
                }
            }
            if (path.size() == boardSize * boardSize) return path;
            else return {};
        }
    };
}

namespace EightQueens {
    string asString(vector<int>& solution) {
        string s;
        for (int col = 0; col < solution.size(); col++) {
            for (int row = 0; row < solution.size(); row++) {
                if (solution[col] == row) s += "Q"; else s += "-";
            }
            s += "\n";
        }
        return s;
    }

    bool isValid(vector<int>& solution) {
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

    vector<vector<int>> solveEightQueensProblem(int boardSize, vector<int>& solution) {
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
        vector<int> solution = {};
        return solveEightQueensProblem(boardSize, solution);
    }
}
