#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

using namespace std;

bool isValid(int x, int y) {
    const string keypad[5] = {
        "  1  ",
        " 234 ",
        "56789",
        " ABC ",
        "  D  "
    };
    if (x < 0 || x > 4 || y < 0 || y > 4) return false;
    return keypad[y][x] != ' ';
}

char getKeypadCode(char start, const string &moves) {
    map<char, pair<int,int>> posMap = {
        {'1',{2,0}}, {'2',{1,1}}, {'3',{2,1}}, {'4',{3,1}},
        {'5',{0,2}}, {'6',{1,2}}, {'7',{2,2}}, {'8',{3,2}}, {'9',{4,2}},
        {'A',{1,3}}, {'B',{2,3}}, {'C',{3,3}}, {'D',{2,4}}
    };

    int x = posMap[start].first;
    int y = posMap[start].second;

    for (char move : moves) {
        int nx = x, ny = y;
        if (move == 'U') ny = y - 1;
        else if (move == 'D') ny = y + 1;
        else if (move == 'L') nx = x - 1;
        else if (move == 'R') nx = x + 1;

        if (isValid(nx, ny)) {
            x = nx;
            y = ny;
        }
    }

    const string keypad[5] = {
        "  1  ",
        " 234 ",
        "56789",
        " ABC ",
        "  D  "
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

    cout << "Bathroom keypad code (new keypad): " << code << endl;
    return 0;
}
