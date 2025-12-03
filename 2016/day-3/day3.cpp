#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string line;
    int count = 0;
    
    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int a, b, c;
        if (iss >> a >> b >> c) {
            if (a + b > c && a + c > b && b + c > a) {
                ++count;
            }
        }
    }
    
    std::cout << count << std::endl;
    return 0;
}
