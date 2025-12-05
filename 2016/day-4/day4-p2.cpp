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

string decryptName(const string& encrypted, int sector) {
    string decrypted;
    for (char c : encrypted) {
        if (c == '-') {
            decrypted += ' ';
        } else if (isalpha(c)) {
            char base = 'a';  // All lowercase input
            char shifted = ((c - base + sector) % 26) + base;
            decrypted += shifted;
        } else {
            decrypted += c;
        }
    }
    return decrypted;
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
    int northPoleSector = -1;
    
    cout << "Found " << rooms.size() << " rooms" << endl;
    cout << "=== ALL REAL ROOMS ===" << endl;
    
    for (const auto& room : rooms) {
        if (isRealRoom(room)) {
            sum += room.sector;
            string decrypted = decryptName(room.name, room.sector);
            string decrypted_lower = decrypted;
            
            // Convert to lowercase for searching
            for (char& c : decrypted_lower) {
                c = tolower(c);
            }
            
            cout << "Sector " << room.sector << ": \"" << decrypted << "\"" << endl;
            
            // More comprehensive search terms
            if (decrypted_lower.find("north") != string::npos ||
                decrypted_lower.find("pole") != string::npos ||
                decrypted_lower.find("santa") != string::npos ||
                decrypted_lower.find("elf") != string::npos ||
                decrypted_lower.find("reindeer") != string::npos) {
                cout << ">>> POTENTIAL NORTH POLE: " << room.sector << " - " << decrypted << endl;
                northPoleSector = room.sector;
            }
        }
    }
    
    cout << "\nPart 1 - Sum: " << sum << endl;
    cout << "Part 2 - North Pole sector: " << northPoleSector << endl;
    
    return 0;
}
