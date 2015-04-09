#include <vector>
#include <stdlib.h>
#include <iostream>
#include <array>
#include <stack>
#include <map>
#include <set>

using std::vector;
using std::array;
using std::stack;
using std::map;
using std::set;
using std::string;
using std::to_string;
using std::tuple;
using std::make_tuple;
using std::get;

namespace VonKochConjecture {
    template<typename T>
    struct Link {
        T n1;
        T n2;
        Link(T n1, T n2) : n1(n1), n2(n2) {}

        bool equalTo(Link link) {
            return n1 == link.n1 && n2 == link.n2;
        }
    };
    class Solution {
    public:
        vector<char> charsLeft;
        map<char, int> mapping;
        vector<Link<char>> links;
        vector<Link<int>> mappedLinks;

        Solution(vector<Link<char>>& links): links(links) {
            for (auto link : links) {
                if (std::find(charsLeft.begin(), charsLeft.end(), link.n1) == charsLeft.end()) {
                    charsLeft.push_back(link.n1);
                }
                if (std::find(charsLeft.begin(), charsLeft.end(), link.n2) == charsLeft.end()) {
                    charsLeft.push_back(link.n2);
                }
            }
        }

        Solution(const Solution& solution):
                charsLeft(solution.charsLeft), mapping(solution.mapping),
                links(solution.links), mappedLinks(solution.mappedLinks) { }

        vector<Solution> nextSolutions() {
            return {};
        }

        bool complete() {
            return links.empty();
        }

        bool isValid() {
            set<int> diffs;
            for (auto link : mappedLinks) {
                diffs.insert(abs(link.n1 - link.n2));
            }
            for (int i = 1; i <= mappedLinks.size(); i++) {
                if (diffs.count(i) != 0) return false;
            }
            return true;
        }
    };

//    vector<Link<int>> allCombinationsOf(vector<int>& unusedLabels) {
//        vector<Link<int>> result;
//        for (int label1 : unusedLabels) {
//            for (int label2 : unusedLabels) {
//                if (label1 != label2) {
//                    result.push_back(Link(label1, label2));
//                }
//            }
//        }
//        return result;
//    }

    vector<Solution> doLabelTree(Solution solution) {
        if (solution.complete()) return {solution};

        vector<Solution> result = {};
        for (auto subSolution : solution.nextSolutions()) {
            if (subSolution.isValid()) {
                auto subSolutions = doLabelTree(solution);
                result.insert(result.begin(), subSolutions.begin(), subSolutions.end());
            }
        }
        return result;
    }

    vector<Solution> labelTree(vector<Link<char>>& treeLinks) {
        Solution emptySolution(treeLinks);
        return doLabelTree(emptySolution);
    }
}


namespace KnightsTour {
    struct Position {
        int x;
        int y;
        Position(int const x, int const y) : x(x), y(y) {}
    };
    struct Path {
        vector<Position> positions;

        Path(vector<Position> const& positions) : positions(positions) { }

        unsigned long size() {
            return positions.size();
        }
    };
    struct Frame {
        vector<Position> path;
        Frame(vector<Position> const& path) : path(path) { }
    };


    string asString(vector<Position>& path) {
        string s;
        for (auto position : path) {
            s += "(" + to_string(position.x) + "," + to_string(position.y) + ")";
        }
        return s;
    }

    bool isValid(Position& position, vector<Position>& path, int boardSize) {
        if (position.x < 0 || position.x >= boardSize) return false;
        if (position.y < 0 || position.y >= boardSize) return false;
        for (auto thatPosition : path) {
            if (thatPosition.x == position.x && thatPosition.y == position.y) return false;
        }
        return true;
    }

    array<Position, 8> allMovesFrom(Position& position) {
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
            if (isValid(newPosition, path, boardSize)) {
                auto pathCopy = vector<Position>(path);
                pathCopy.push_back(newPosition);
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


    class KnightPathLazy {
    private:
        int boardSize;
        stack<Frame> frames;

    public:
        KnightPathLazy(int boardSize) : boardSize(boardSize) {
            frames = stack<Frame>({ Frame({Position(0, 0)}) });
        };

        vector<Position> nextPath() {
            vector<Position> path = {};
            while (!frames.empty() && path.size() != boardSize * boardSize) {
                auto frame = frames.top();
                frames.pop();

                path = frame.path;

                auto position = path.back();
                for (auto newPosition : allMovesFrom(position)) {
                    if (isValid(newPosition, path, boardSize)) {
                        auto pathCopy = vector<Position>(path);
                        pathCopy.push_back(newPosition);
                        frames.push(Frame(pathCopy));
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
