#include <iostream>
#include <fstream>
#include <regex>
#include <string>

int getMultiplication(const std::string &s) {
    size_t open_bracket = s.find('(');
    size_t comma = s.find(',');
    size_t closed_bracket = s.find(')');
    int mul1 = std::stoi(s.substr(open_bracket + 1, comma - open_bracket - 1));
    int mul2 = std::stoi(s.substr(comma + 1, closed_bracket - comma - 1));
    std::cout << "Mul1: " << mul1 << "\nMul2: " << mul2 << '\n';
    return mul1 * mul2;
}

int main(int argc, char *argv[]) {
    std::string line;
    const std::regex re("mul\\(([0-9]+),([0-9]+)\\)");
    int sum = 0;

    std::ifstream file("multiplications.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file 'multiplications.txt'.\n";
        return 1;
    }
    
    while (std::getline(file, line)) {
        auto start = std::sregex_iterator(line.begin(), line.end(), re);
        auto end = std::sregex_iterator();
        for (std::sregex_iterator i = start; i != end; i++) {
            std::smatch match = *i;
            std::string mul = match.str();
            std::cout << "Regex match: " << mul << '\n';
            sum += getMultiplication(mul);
        }
    }

    file.close();
    std::cout << "The total sum of multiplications is: " << sum << std::endl;
    return 0;
}
