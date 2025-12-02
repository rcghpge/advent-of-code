#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

char getKeypadCode(char start, const string &moves) {
    int x, y;
    switch (start) {
        case '1': x=0; y=0; break;
        case '2': x=1; y=0; break;
        case '3': x=2; y=0; break;
        case '4': x=0; y=1; break;
        case '5': x=1; y=1; break;
        case '6': x=2; y=1; break;
        case '7': x=0; y=2; break;
        case '8': x=1; y=2; break;
        case '9': x=2; y=2; break;
        default: x=1; y=1;
    }

    for (char move : moves) {
        int nx = x, ny = y;
        if (move == 'U') ny = y > 0 ? y - 1 : y;
        else if (move == 'D') ny = y < 2 ? y + 1 : y;
        else if (move == 'L') nx = x > 0 ? x - 1 : x;
        else if (move == 'R') nx = x < 2 ? x + 1 : x;

        if (nx >= 0 && nx <= 2 && ny >= 0 && ny <= 2) {
            x = nx;
            y = ny;
        }
    }

    const char keypad[3][3] = {
        {'1','2','3'},
        {'4','5','6'},
        {'7','8','9'}
    };

    return keypad[y][x];
}

int main() {
    ifstream file("input.txt");
    if (!file) {
        cerr << "Error reading input data" << endl;
        return 1;
    }

    vector<string> input;
    string line;
    while (getline(file, line)) {
        input.push_back(line);
    }
    file.close();

    char keypadcode = '5';
    string code = "";

    for (const string &line : input) {
        keypadcode = getKeypadCode(keypadcode, line);
        code += keypadcode;
    }

    cout << "Bathroom keypad code: " << code << endl;
    return 0;
}
