#include <bits/stdc++.h>
using namespace std;

int main() {
    string line; getline(cin, line);
    line.erase(remove(line.begin(), line.end(), ' '), line.end());
    
    int x=0, y=0, dir=0;
    set<pair<int,int>> visited;
    
    size_t pos = 0;
    while (pos < line.size()) {
        char turn = line[pos++];
        int dist = 0;
        while (pos < line.size() && isdigit(line[pos])) {
            dist = dist * 10 + (line[pos++] - '0');
        }
        if (pos < line.size()) pos++;
        
        if (turn == 'R') dir = (dir + 1) % 4;
        else dir = (dir + 3) % 4;
        
        // Part 2: Check each step for revisit
        for (int step = 1; step <= dist; step++) {
            if (dir == 0) y++;
            else if (dir == 1) x++;
            else if (dir == 2) y--;
            else x--;
            
            pair<int,int> loc = {x, y};
            if (visited.count(loc)) {
                cout << "Part 2: " << abs(x) + abs(y) << endl;
                return 0;
            }
            visited.insert(loc);
        }
    }
    cout << "Part 1: " << abs(x) + abs(y) << endl;
    return 0;
}
