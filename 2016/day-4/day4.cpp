#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std;

struct Room {
    string name;
    int sector;
    string checksum;
};

vector<Room> parseRooms(const string& input) {
    vector<Room> rooms;
    stringstream ss(input);
    string line;
    
    while (getline(ss, line)) {
        if (line.empty()) continue;
        
        size_t lastDash = line.rfind('-');
        if (lastDash == string::npos) continue;
        
        size_t bracket = line.find('[', lastDash);
        if (bracket == string::npos) continue;
        
        size_t end = line.find(']', bracket);
        if (end == string::npos) continue;
        
        Room room;
        room.name = line.substr(0, lastDash);
        string sectorStr = line.substr(lastDash + 1, bracket - lastDash - 1);
        
        // Safely convert sector to int
        try {
            room.sector = stoi(sectorStr);
        } catch (const std::invalid_argument& e) {
            continue;
        } catch (const std::out_of_range& e) {
            continue;
        }
        
        room.checksum = line.substr(bracket + 1, end - bracket - 1);
        rooms.push_back(room);
    }
    return rooms;
}

bool isRealRoom(const Room& room) {
    map<char, int> counts;
    for (char c : room.name) {
        if (isalpha(c)) {
            counts[tolower(c)]++;
        }
    }
    
    vector<pair<int, char>> sorted;
    for (auto& p : counts) {
        sorted.emplace_back(-p.second, p.first);
    }
    sort(sorted.begin(), sorted.end());
    
    string calculated;
    for (size_t i = 0; i < 5 && i < sorted.size(); ++i) {
        calculated += sorted[i].second;
    }
    
    return calculated == room.checksum;
}

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cout << "Error: Could not open input.txt" << endl;
        return 1;
    }
    
    string input((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    
    auto rooms = parseRooms(input);
    long long sum = 0;
    
    cout << "Found " << rooms.size() << " rooms" << endl;
    
    for (const auto& room : rooms) {
        if (isRealRoom(room)) {
            sum += room.sector;
            cout << "Real room: " << room.name << "-" << room.sector 
                 << "[" << room.checksum << "]" << endl;
        }
    }
    
    cout << "Sum of real rooms by sector ID: " << sum << endl;
    
    return 0;
}
