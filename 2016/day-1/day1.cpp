#include <bits/stdc++.h>
using namespace std;

int main() {
    string line; getline(cin, line);
    int x=0, y=0, dir=0;
    
    line.erase(remove(line.begin(), line.end(), ' '), line.end());
    
    size_t pos = 0;
    while (pos < line.size()) {
        char turn = line[pos++];
        int dist = 0;
        while (pos < line.size() && isdigit(line[pos])) {
            dist = dist * 10 + (line[pos++] - '0');
        }
        if (pos < line.size()) pos++; 
        
        if (turn == 'R') dir = (dir + 1) % 4;
        else if (turn == 'L') dir = (dir + 3) % 4;
        
        if (dir == 0) y += dist;
        else if (dir == 1) x += dist;
        else if (dir == 2) y -= dist;
        else x -= dist;
    }
    cout << abs(x) + abs(y) << endl;
}
