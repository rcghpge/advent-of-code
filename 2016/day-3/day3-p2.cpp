#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main() {
    std::vector<std::vector<int>> triangles;
    std::string line;
    
    // Read all row triplets
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int a, b, c;
        if (iss >> a >> b >> c) {
            triangles.push_back({a, b, c});
        }
    }
    
    int count = 0;
    // Process vertical triplets
    for (size_t i = 0; i + 2 < triangles.size(); i += 3) {
        // Check three column triangles
        for (int col = 0; col < 3; ++col) {
            int a = triangles[i][col];
            int b = triangles[i + 1][col];
            int c = triangles[i + 2][col];
            if (a + b > c && a + c > b && b + c > a) {
                ++count;
            }
        }
    }
    
    std::cout << count << std::endl;
    return 0;
}
