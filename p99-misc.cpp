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
using std::string;

template<typename T>
using p = std::unique_ptr<T>;
template<typename T>
p<T> p_(T* t) { return p<T>(t); }


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
        vector<Link<char>>& links;
        vector<Link<int>> mappedLinks;
        unsigned long nodeAmount;

        Solution(vector<Link<char>>& links): links(links) {
            for (auto link : links) {
                if (std::find(charsLeft.begin(), charsLeft.end(), link.n1) == charsLeft.end()) {
                    charsLeft.push_back(link.n1);
                }
                if (std::find(charsLeft.begin(), charsLeft.end(), link.n2) == charsLeft.end()) {
                    charsLeft.push_back(link.n2);
                }
            }
            nodeAmount = charsLeft.size();
        }

        Solution(const Solution& solution):
                charsLeft(solution.charsLeft), mapping(solution.mapping),
                links(solution.links), mappedLinks(solution.mappedLinks),
                nodeAmount(solution.nodeAmount) {}

        Solution(Solution&& solution) = default;

        void operator=(const Solution& solution) {
            charsLeft = vector<char>(solution.charsLeft);
            mapping = map<char, int>(solution.mapping);
            links = solution.links;
            mappedLinks = solution.mappedLinks;
            nodeAmount = solution.nodeAmount;
        }

        vector<p<Solution>> nextSolutions() {
            vector<p<Solution>> result;

            auto c = charsLeft.back();
            charsLeft.pop_back();

            for (int i = 1; i <= nodeAmount; i++) {
                bool alreadyMapped = false;
                for (auto item : mapping) {
                    if (item.second == i) alreadyMapped = true;
                }
                if (alreadyMapped) continue;

                p<Solution> subSolution = p_(new Solution(*this));
                subSolution->mapping[c] = i;
                subSolution->mappedLinks.clear();
                for (auto link : subSolution->links) {
                    if (subSolution->mapping.find(link.n1) != subSolution->mapping.end() &&
                        subSolution->mapping.find(link.n2) != subSolution->mapping.end()) {
                        subSolution->mappedLinks.push_back(Link<int>(
                                subSolution->mapping[link.n1],
                                subSolution->mapping[link.n2]
                        ));
                    }
                }
                if (subSolution->isValid()) {
                    result.push_back(subSolution);
                }
            }
            return result;
        }

        bool complete() {
            return charsLeft.size() == 0;
        }

        bool isValid() {
            set<int> diffs;
            for (auto link : mappedLinks) {
                int diff = abs(link.n1 - link.n2);
                if (diffs.count(diff) > 0) return false;
                else diffs.insert(diff);
            }
            return true;
        }

        string toString() {
            string s = "";
            int i = 1;
            for (auto link : mappedLinks) {
                s += std::to_string(link.n1) + "->" + std::to_string(link.n2);
                if (i++ < mappedLinks.size()) s += " ";
            }
            return "(" + s + ")";
        }
    };

    vector<p<Solution>> doLabelTree(p<Solution> solution, int depth) {
        if (solution->complete()) return {solution};

        vector<p<Solution>> result = {};
        for (auto subSolution : solution->nextSolutions()) {
            auto subSolutions = doLabelTree(subSolution, depth + 1);
            result.insert(result.begin(), subSolutions.begin(), subSolutions.end());
            if (depth == 0 && result.size() > 0) return result; // TODO remove
        }
        return result;
    }


    vector<p<Solution>> labelTree(vector<Link<char>>& treeLinks) {
        auto start = std::chrono::system_clock::now();

        p<Solution> emptySolution = p_(new Solution(treeLinks));
        vector<p<Solution>> solution = doLabelTree(emptySolution, 0);

        auto end = std::chrono::system_clock::now();
        std::cout << "Duration: " << (end - start).count() / 1000 << "ms\n";

        return solution;
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
