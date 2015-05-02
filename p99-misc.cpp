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


class Combination {
public:
    virtual vector<p<Combination>> subCombinations() = 0;
    virtual bool isComplete() = 0;
    virtual bool isValid() = 0;
    virtual string toString() {
        return "";
    }
};

vector<p<Combination>> findAllValidCombinations(p<Combination>& combination) {
    vector<p<Combination>> result;

    if (combination->isComplete()) {
        result.push_back(std::move(combination));
        return result;
    }

    for (auto& subCombination : combination->subCombinations()) {
        if (!subCombination->isValid()) continue;
        vector<p<Combination>> subCombinations = findAllValidCombinations(subCombination);
        result.insert(result.begin(),
                      std::make_move_iterator(subCombinations.begin()),
                      std::make_move_iterator(subCombinations.end()));
    }
    return result;
}


// aka "Graceful labelling conjecture" (http://en.wikipedia.org/wiki/Graceful_labeling)
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

    // Misc links:
    // http://en.cppreference.com/w/cpp/language/move_constructor
    // http://stackoverflow.com/questions/23071621/llvm-find-if-implicitly-deleted-copy-constructor-with-unique-ptr
    // http://stackoverflow.com/questions/3283778/why-can-i-not-push-back-a-unique-ptr-into-a-vector
    // http://stackoverflow.com/questions/21487714/inserting-a-vector-of-unique-ptr-into-another-vector
    // http://stackoverflow.com/questions/20292682/iterating-through-vectorunique-ptrmytype-using-c11-for-loops
    class LinkMapping : public Combination {
    public:
        vector<char> charsLeft;
        map<char, int> mapping;
        vector<Link<char>>& links;
        vector<Link<int>> mappedLinks;
        unsigned long nodeAmount;

        LinkMapping(vector<Link<char>>& links): links(links) {
            for (auto& link : links) {
                if (std::find(charsLeft.begin(), charsLeft.end(), link.n1) == charsLeft.end()) {
                    charsLeft.push_back(link.n1);
                }
                if (std::find(charsLeft.begin(), charsLeft.end(), link.n2) == charsLeft.end()) {
                    charsLeft.push_back(link.n2);
                }
            }
            nodeAmount = charsLeft.size();
        }

        LinkMapping(const LinkMapping& solution):
                charsLeft(solution.charsLeft), mapping(solution.mapping),
                links(solution.links), mappedLinks(solution.mappedLinks),
                nodeAmount(solution.nodeAmount) {}

        vector<p<Combination>> subCombinations() {
            vector<p<Combination>> result;

            char c = nextUnmappedChar();
            for (int i = 1; i <= nodeAmount; i++) {
                bool alreadyMapped = reverseMapping(i) != 0;
                if (alreadyMapped) continue;
                result.push_back(std::move(this->copyWithMapping(c, i)));
            }
            return result;
        }

        bool isComplete() override {
            return links.size() == mappedLinks.size();
        }

        bool isValid() override {
            set<int> diffs;
            for (auto link : mappedLinks) {
                int diff = abs(link.n1 - link.n2);
                if (diffs.count(diff) > 0) return false;
                else diffs.insert(diff);
            }
            return true;
        }

        string toString() override {
            string s = "";
            int i = 1;
            for (auto& link : mappedLinks) {
                s += std::to_string(link.n1) + "->" + std::to_string(link.n2);
                if (i++ < mappedLinks.size()) s += " ";
            }
            return "(" + s + ")";
        }

    private:
        char nextUnmappedChar() {
            auto c = charsLeft.back();
            if (mappedLinks.size() == 0) return c;
            for (auto mappedLink : mappedLinks) {
                char mappedChar = reverseMapping(mappedLink.n1);
                char unmappedNeighbour = findUnmapped(neighboursOf(mappedChar));
                if (unmappedNeighbour != 0) return unmappedNeighbour;

                mappedChar = reverseMapping(mappedLink.n2);
                unmappedNeighbour = findUnmapped(neighboursOf(mappedChar));
                if (unmappedNeighbour != 0) return unmappedNeighbour;
            }
            return c;
        }

        char reverseMapping(int n) {
            for (auto item : mapping) {
                if (item.second == n) return item.first;
            }
            return 0;
        }

        bool hasMapping(char c) {
            return mapping.find(c) != mapping.end();
        }

        char findUnmapped(const set<char>& chars) {
            for (auto c : chars) {
                if (!hasMapping(c)) return c;
            }
            return 0;
        }

        set<char> neighboursOf(char c) {
            set<char> result;
            for (auto link : links) {
                if (link.n1 == c) result.insert(link.n2);
                else if (link.n2 == c) result.insert(link.n1);
            }
            return result;
        }

        p<Combination> copyWithMapping(char fromChar, int toInt) {
            auto subSolution = new LinkMapping(*this);
            subSolution->charsLeft.pop_back();
            subSolution->mapping[fromChar] = toInt;
            subSolution->mappedLinks.clear();
            for (auto& link : subSolution->links) {
                if (subSolution->hasMapping(link.n1) && subSolution->hasMapping(link.n2)) {
                    subSolution->mappedLinks.push_back(Link<int>(
                            subSolution->mapping[link.n1],
                            subSolution->mapping[link.n2]
                    ));
                }
            }
            return p_<Combination>(subSolution);
        }
    };

    vector<p<Combination>> labelTree(vector<Link<char>>& treeLinks) {
        auto start = std::chrono::system_clock::now();

        p<Combination> emptySolution = p_<Combination>(new LinkMapping(treeLinks));
        vector<p<Combination>> solution = findAllValidCombinations(emptySolution);

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

    class KnightsPath : public Combination {
    public:
        const int boardSize;
        vector<Position> positions;

        KnightsPath(const int boardSize, const vector<Position>& positions) :
                boardSize(boardSize), positions(positions) {}

        KnightsPath(const KnightsPath& knightsPath) :
            boardSize(knightsPath.boardSize), positions(knightsPath.positions) {}

        vector<p<Combination>> subCombinations() override {
            vector<p<Combination>> result;
            for (auto nextPosition : allMovesFrom(positions.back())) {
                auto pathCopy = new KnightsPath(*this);
                pathCopy->positions.push_back(nextPosition);
                result.push_back(p_(pathCopy));
            }
            return result;
        }

        bool isComplete() override {
            return positions.size() == boardSize * boardSize;
        }

        bool isValid() override {
            auto position = positions.back();
            if (position.x < 0 || position.x >= boardSize) return false;
            if (position.y < 0 || position.y >= boardSize) return false;
            for (int i = 0; i < positions.size() - 1; i++) { // note that last element is skipped
                if (positions[i].x == position.x && positions[i].y == position.y) return false;
            }
            return true;
        }

        string toString() override {
            return "";
        }
    };


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
