#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int CHESS_BOARD_SIZE = 8;

struct Cell {
    int row;
    int col;

    Cell(int r = -1, int c = -1) : row(r), col(c) {}
};

struct Soldier {
    Cell position;
};

struct Castle {
    Cell position;

    Castle(int r, int c) : position(r, c) {}
};

bool isValidCell(int row, int col) {
    return row >= 0 && row < CHESS_BOARD_SIZE && col >= 0 && col < CHESS_BOARD_SIZE;
}

bool canAttackSoldier(const Castle& castle, const vector<Soldier>& soldiers, int& soldierIndex) {
    for (int i = 0; i < soldiers.size(); ++i) {
        if (castle.position.row == soldiers[i].position.row && castle.position.col == soldiers[i].position.col) {
            soldierIndex = i;
            return true;
        }
    }
    return false;
}

void attackSoldier(Castle& castle, vector<Soldier>& soldiers, int soldierIndex) {
    soldiers.erase(soldiers.begin() + soldierIndex);
}

bool canJumpOverSoldier(const Castle& castle, const vector<Soldier>& soldiers) {
    for (const Soldier& soldier : soldiers) {
        if (soldier.position.row == castle.position.row + 1 && soldier.position.col == castle.position.col) {
            return isValidCell(castle.position.row + 2, castle.position.col);
        }
    }
    return false;
}

void jumpOverSoldier(Castle& castle) {
    castle.position.row += 2; // Jump over the soldier
}

bool isHome(const Castle& castle) {
    return castle.position.row == 1 && castle.position.col == 2; // Home coordinates
}

void findPaths(vector<Soldier>& soldiers, Castle castle, vector<string>& paths, string pathSoFar) {
    pathSoFar += "\nStart (" + to_string(castle.position.row) + "," + to_string(castle.position.col) + ")";

    if (isHome(castle) && soldiers.empty()) {
        pathSoFar += "\nArrive (1,2)";
        paths.push_back(pathSoFar);
        return;
    }

    for (int i = 0; i < soldiers.size(); ++i) {
        if (castle.position.row == soldiers[i].position.row && castle.position.col == soldiers[i].position.col) {
            string newPath = pathSoFar + "\nKill (" + to_string(soldiers[i].position.row) + "," + to_string(soldiers[i].position.col) + "). Turn Left";
            Castle newCastle = castle;
            attackSoldier(newCastle, soldiers, i);
            findPaths(soldiers, newCastle, paths, newPath);
        }
    }

    // Try jumping over a soldier
    if (canJumpOverSoldier(castle, soldiers)) {
        string jumpPath = pathSoFar + "\nJump ("
                          + to_string(castle.position.row + 1) + "," + to_string(castle.position.col) + ")";
        Castle newCastle = castle;
        jumpOverSoldier(newCastle);
        findPaths(soldiers, newCastle, paths, jumpPath);
    }
}

int main() {
    int numSoldiers;
    cout << "Run Program: \nfind_my_home_castle-soldiers ";
    cin >> numSoldiers;

    vector<Soldier> soldiers(numSoldiers);
    for (int i = 0; i < numSoldiers; ++i) {
        int x, y;
        char comma; // Declare comma variable to consume the comma in the input
        cout << "Enter coordinates for soldier " << (i + 1) << ": ";
        cin >> x >> comma >> y; // Input in the format x,y
        soldiers[i] = Soldier{Cell(x, y)};
    }

    int castleRow, castleCol;
    char comma2; // Declare another variable for the castle's input
    cout << "Enter the coordinates for your \"special\" castle: "; // Correct quotation
    cin >> castleRow >> comma2 >> castleCol; // Corrected input reading
    Castle castle(castleRow, castleCol);

    vector<string> paths;
    string initialPath;
    findPaths(soldiers, castle, paths, initialPath);

    cout << "Thanks. There are " << paths.size() << " unique paths for your \"special castle\"" << endl; // Correct quotation

    for (int i = 0; i < paths.size(); ++i) {
        cout << "Path " << (i + 1) << ":\n=======\n" << paths[i] << endl;
    }

    return 0;
}