#include "../../shared/input.hpp"
#include "../../shared/printing.hpp"
#include <algorithm>
#include <array>
#include <iostream>

class Tile {
  public:
    int row;
    int col;

    Tile(int r, int c) : row(r), col(c){};

    friend bool operator==(const Tile lhs, const Tile rhs) {
        return lhs.row == rhs.row && lhs.col == rhs.col;
    }
};

enum class Direction { up,
                       down,
                       left,
                       right };

enum class Turn { straight,
                  left,
                  right };

using Step = typename std::pair<Direction, Tile>;
using Grid = typename std::vector<std::string>;
using Bool2D = typename std::vector<std::vector<bool>>;

Tile findNextTile(Direction comingFrom, const Tile& curr, Turn turn) {
    switch (comingFrom) {
    case Direction::left:
        switch (turn) {
        case Turn::straight:
            return Tile(curr.row, curr.col + 1);
        case Turn::left:
            return Tile(curr.row - 1, curr.col);
        case Turn::right:
            return Tile(curr.row + 1, curr.col);
        }

    case Direction::right:
        switch (turn) {
        case Turn::straight:
            return Tile(curr.row, curr.col - 1);
        case Turn::left:
            return Tile(curr.row + 1, curr.col);
        case Turn::right:
            return Tile(curr.row - 1, curr.col);
        }

    case Direction::down:
        switch (turn) {
        case Turn::straight:
            return Tile(curr.row - 1, curr.col);
        case Turn::left:
            return Tile(curr.row, curr.col - 1);
        case Turn::right:
            return Tile(curr.row, curr.col + 1);
        }

    case Direction::up:
        switch (turn) {
        case Turn::straight:
            return Tile(curr.row + 1, curr.col);
        case Turn::left:
            return Tile(curr.row, curr.col + 1);
        case Turn::right:
            return Tile(curr.row, curr.col - 1);
        }
    }

    std::cout << "Shouldn't ever get here" << std::endl;
    exit(1);
}

Turn turnOfForwardMirror(Direction comingFrom) {
    switch (comingFrom) {
    case Direction::left:
    case Direction::right:
        return Turn::left;
    case Direction::up:
    case Direction::down:
        return Turn::right;
    }
    std::cout << "Shouldn't ever get here" << std::endl;
    exit(1);
}

Turn turnOfBackwardMirror(Direction comingFrom) {
    switch (comingFrom) {
    case Direction::left:
    case Direction::right:
        return Turn::right;
    case Direction::up:
    case Direction::down:
        return Turn::left;
    }
    std::cout << "Shouldn't ever get here" << std::endl;
    exit(1);
}

Direction directionFromForwardMirror(Direction comingFrom) {
    switch (comingFrom) {
    case Direction::left:
        return Direction::down;
    case Direction::right:
        return Direction::up;
    case Direction::up:
        return Direction::right;
    case Direction::down:
        return Direction::left;
    }
    std::cout << "Shouldn't ever get here" << std::endl;
    exit(1);
}

Direction directionFromBackwardMirror(Direction comingFrom) {
    switch (comingFrom) {
    case Direction::left:
        return Direction::up;
    case Direction::right:
        return Direction::down;
    case Direction::up:
        return Direction::left;
    case Direction::down:
        return Direction::right;
    }
    std::cout << "Shouldn't ever get here" << std::endl;
    exit(1);
}

void next(Direction comingFrom, const Tile& curr, const Grid& input, Bool2D& energized, std::vector<Step>& previousSteps) {
    if (curr.row >= input.size() || curr.row < 0 || curr.col >= input[0].size() || curr.col < 0) {
        return;
    }

    if (std::find(previousSteps.begin(), previousSteps.end(), std::make_pair(comingFrom, curr)) != previousSteps.end()) {
        return;
    }

    previousSteps.push_back(std::make_pair(comingFrom, curr));
    energized[curr.row][curr.col] = true;

    switch (input[curr.row][curr.col]) {
    case '.':
        next(comingFrom, findNextTile(comingFrom, curr, Turn::straight), input, energized, previousSteps);
        break;
    case '/': {
        Direction nextDir = directionFromForwardMirror(comingFrom);
        Tile nextTile = findNextTile(comingFrom, curr, turnOfForwardMirror(comingFrom));
        next(nextDir, nextTile, input, energized, previousSteps);
        break;
    }
    case '\\': {
        Direction nextDir = directionFromBackwardMirror(comingFrom);
        Tile nextTile = findNextTile(comingFrom, curr, turnOfBackwardMirror(comingFrom));
        next(nextDir, nextTile, input, energized, previousSteps);
        break;
    }
    case '-': {
        if (comingFrom == Direction::left || comingFrom == Direction::right) {
            next(comingFrom, findNextTile(comingFrom, curr, Turn::straight), input, energized, previousSteps);
            return;
        }

        next(Direction::left, Tile(curr.row, curr.col + 1), input, energized, previousSteps);
        next(Direction::right, Tile(curr.row, curr.col - 1), input, energized, previousSteps);
        break;
    }
    case '|': {
        if (comingFrom == Direction::up || comingFrom == Direction::down) {
            next(comingFrom, findNextTile(comingFrom, curr, Turn::straight), input, energized, previousSteps);
            return;
        }

        next(Direction::down, Tile(curr.row - 1, curr.col), input, energized, previousSteps);
        next(Direction::up, Tile(curr.row + 1, curr.col), input, energized, previousSteps);
        break;
    }
    }
}

size_t newAnswer(size_t answer, const Bool2D& energized) {
    size_t totalEnergized = 0;
    for (auto vec : energized) {
        for (bool e : vec) {
            if (e) {
                totalEnergized++;
            }
        }
    }

    if (totalEnergized > answer) {
        return totalEnergized;
    }

    return answer;
}

int main(int argc, char** argv) {
    Grid input = aoc::parseInput(argv[1]);
    Bool2D energized;
    std::vector<Step> previousSteps;
    size_t answer = 0;

    auto tryEdge = [&](Direction comingFrom, std::function<Tile(int)> startingTile, int edgeLength) {
        for (int i = 0; i < edgeLength; i++) {
            energized.clear();
            energized = Bool2D(input.size(), std::vector<bool>(input[0].size(), false));
            previousSteps.clear();
            next(comingFrom, startingTile(i), input, energized, previousSteps);
            answer = newAnswer(answer, energized);
        }
    };

    tryEdge(Direction::left, [](int i) { return Tile(i, 0); }, input.size());
    tryEdge(Direction::right, [&input](int i) { return Tile(i, input[0].size() - 1); }, input.size());
    tryEdge(Direction::up, [](int i) { return Tile(0, i); }, input[0].size());
    tryEdge(Direction::down, [&input](int i) { return Tile(input.size() - 1, i); }, input[0].size());

    std::cout << answer << std::endl;
}
