#include "../../shared/input.hpp"
#include <iostream>
#include <tuple>

class Tile {
  public:
    int row;
    int col;

    Tile() : row(-1), col(-1){};
    Tile(int r, int c) : row(r), col(c){};

    bool initialized() const {
        return row != -1;
    }

    bool operator==(const Tile& other) const {
        return row == other.row && col == other.col;
    }

    bool operator!=(const Tile& other) const {
        return !(*this == other);
    }

    void next(Tile& prev, char symbol) {
        Tile result;

        if (initialized() == false || prev.initialized() == false) {
            std::cout << "Trying to use un-initialized Tile" << std::endl;
            exit(1);
        }

        switch (symbol) {
        case '-':
            (col < prev.col) ? result = Tile(row, col - 1) : result = Tile(row, col + 1);
            break;
        case '|':
            (row < prev.row) ? result = Tile(row - 1, col) : result = Tile(row + 1, col);
            break;
        case 'F':
            (row != prev.row) ? result = Tile(row, col + 1) : result = Tile(row + 1, col);
            break;
        case '7':
            (row != prev.row) ? result = Tile(row, col - 1) : result = Tile(row + 1, col);
            break;
        case 'L':
            (row != prev.row) ? result = Tile(row, col + 1) : result = Tile(row - 1, col);
            break;
        case 'J':
            (row != prev.row) ? result = Tile(row, col - 1) : result = Tile(row - 1, col);
            break;
        default:
            std::cout << "Bad symbol" << std::endl;
            exit(1);
        }

        prev = *this;
        this->row = result.row;
        this->col = result.col;
    }
};

Tile findStartingTile(const std::vector<std::string>& input) {
    for (int i = 0; i < input.size(); i++) {
        for (int j = 0; j < input[0].size(); j++) {
            if (input[i][j] == 'S') {
                return Tile(i, j);
            }
        }
    }

    std::cout << "Could not find starting position" << std::endl;
    exit(1);
}

Tile findStartingPath(const Tile& start, std::vector<std::string>& input) {
    Tile result;
    Tile second;

    if (start.row - 1 >= 0) {
        switch (input[start.row - 1][start.col]) {
        case '|':
        case 'F':
        case '7': {
            Tile tmp(start.row - 1, start.col);
            second = result;
            result = std::move(tmp);
            break;
        }
        default:
            break;
        }
    }

    if (start.col - 1 >= 0) {
        switch (input[start.row][start.col - 1]) {
        case '-':
        case 'F':
        case 'L': {
            Tile tmp(start.row, start.col - 1);
            second = result;
            result = std::move(tmp);
            break;
        }
        default:
            break;
        }
    }

    if (start.col + 1 < input[0].size()) {
        switch (input[start.row][start.col + 1]) {
        case '-':
        case '7':
        case 'J': {
            Tile tmp(start.row, start.col + 1);
            second = result;
            result = std::move(tmp);
            break;
        }
        default:
            break;
        }
    }

    if (start.row + 1 < input.size()) {
        switch (input[start.row + 1][start.col]) {
        case '|':
        case 'L':
        case 'J': {
            Tile tmp(start.row + 1, start.col);
            second = result;
            result = std::move(tmp);
            break;
        }
        default:
            break;
        }
    }

    // change the S to its appropriate symbol to make our lives easier when determining boundary crossing later
    if (result.row == second.row) {
        input[start.row][start.col] = '-';
    } else if (result.col == second.col) {
        input[start.row][start.col] = '|';
    } else if (result.row < second.row && result.col > second.col) {
        input[start.row][start.col] = 'L';
    } else if (result.row < second.row && result.col < second.col) {
        input[start.row][start.col] = 'J';
    } else if (result.row > second.row && result.col > second.col) {
        input[start.row][start.col] = '7';
    } else if (result.row > second.row && result.col < second.col) {
        input[start.row][start.col] = 'F';
    }

    return result;
}

bool crossedVerticalBoundary(char curr, char& startChar) {
    if (curr == '|') {
        return true;
    }

    if (curr == '-') {
        return false;
    }

    if (curr == 'F') {
        startChar = 'F';
        return false;
    }

    if (curr == 'L') {
        startChar = 'L';
        return false;
    }

    if (curr == '7') {
        if (startChar == 'F') {
            startChar = 'u';
            return false;
        } else {
            startChar = 'u';
            return true;
        }
    }

    if (curr == 'J') {
        if (startChar == 'L') {
            startChar = 'u';
            return false;
        } else {
            startChar = 'u';
            return true;
        }
    }

    std::cout << "Could not determine if a boundary was crossed" << std::endl;
    exit(1);
}

bool tileInLoop(const Tile& t, const std::vector<Tile>& loopTiles) {
    for (const Tile& curr : loopTiles) {
        if (t == curr) {
            return true;
        }
    }

    return false;
}

int main(int argc, char** argv) {
    std::vector<std::string> input = aoc::parseInput(argv[1]);
    std::vector<Tile> loopTiles;
    Tile start = findStartingTile(input);
    Tile curr = findStartingPath(start, input);
    Tile prev = start;
    loopTiles.push_back(start);
    loopTiles.push_back(curr);

    while (curr != start) {
        curr.next(prev, input[curr.row][curr.col]);
        loopTiles.push_back(curr);
    }

    int insideTiles = 0;
    for (int i = 0; i < input.size(); i++) {
        bool inside = false;
        char startChar = 'u';
        int tilesOnRow = 0;
        for (int j = 0; j < input[0].size(); j++) {
            if (tileInLoop(Tile(i, j), loopTiles) == false) {
                if (inside) {
                    insideTiles++;
                    tilesOnRow++;
                }

                continue;
            }

            if (crossedVerticalBoundary(input[i][j], startChar)) {
                inside = !inside;
            }
        }
    }

    std::cout << insideTiles << std::endl;
}
