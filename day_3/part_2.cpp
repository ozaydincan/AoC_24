#include <fstream>
#include <iostream>
#include <regex>
#include <string>

enum Command { DO, DONT, DEFAULT };
Command matchPattern(const std::string &cmd) {
  if (cmd == "do()") {
    return DO;
  } else if (cmd == "don't()") {
    return DONT;
  } else {
    return DEFAULT;
  }
}

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
  int sum = 0;

  if (argc != 2) {
    std::cerr << argv[0] << " <filename>" << std::endl;
    return 1;
  }

  std::regex re(R"(((do\(\))|(don't\(\))|(mul\(\d{1,3},\d{1,3}\))))");
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open the file 'multiplications.txt'.\n";
    return 1;
  }

  bool enable = true;
  while (std::getline(file, line)) {
    auto start = std::sregex_iterator(line.begin(), line.end(), re);
    auto end = std::sregex_iterator();
    for (std::sregex_iterator i = start; i != end; i++) {
      std::smatch match = *i;
      std::string mul = match.str();
      std::cout << "Regex match: " << mul << '\n';
      Command cmd = matchPattern(mul);
      switch (cmd) {
      case DO:
        enable = true;
        break;
      case DONT:
        enable = false;
        break;
      default:

        break;
      }
      if (enable && mul != "do()") {
        sum += getMultiplication(mul);
      }
    }
  }

  file.close();
  std::cout << "The total sum of multiplications is: " << sum << std::endl;
  return 0;
}
