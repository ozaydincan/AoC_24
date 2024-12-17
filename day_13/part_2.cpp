#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

class Matrix {
public:
  long long int a, b;
  bool validSolution;

  Matrix(std::vector<std::vector<long long>> &input)
      : a(0), b(0), validSolution(false), matrix(input), row(input.size()),
        col(input[0].size()) {
    findPresses();
  }

private:
  std::vector<std::vector<long long>> matrix;
  size_t row, col;

  void findPresses() {
    long long x1 = matrix[0][0], y1 = matrix[0][1], prize_x = matrix[0][2];
    long long x2 = matrix[1][0], y2 = matrix[1][1], prize_y = matrix[1][2];

    long long det = x1 * y2 - x2 * y1;

    if (det == 0) {
      validSolution = false;
      return;
    }

    long long a_num = prize_x * y2 - prize_y * y1;
    long long b_num = x1 * prize_y - x2 * prize_x;

    if (a_num % det == 0 && b_num % det == 0) {
      a = a_num / det;
      b = b_num / det;
      validSolution = true;
    } else {
      validSolution = false;
    }
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Wrong command line arguments\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "File not opened\n";
    return 2;
  }
  std::string line;
  std::regex re(R"(X([+=-]?(\d+)), Y([+=-]?(\d+)))");
  std::vector<std::vector<long long>> buffer(2, std::vector<long long>(3));
  long long sum = 0;
  int i = 0;
  while (std::getline(file, line)) {
    if (line.empty())
      continue;
    std::smatch matches;
    if (std::regex_search(line, matches, re)) {
      long long x_val = std::stoll(matches[2]);
      long long y_val = std::stoll(matches[4]);
      buffer[0][i] = x_val;
      buffer[1][i] = y_val;
      i++;
    }
    if (i == 3) {
      long long mul = 10000000000000;
      buffer[0][2] += mul;
      buffer[1][2] += mul;
      for (auto &row : buffer) {
        for (auto &num : row) {
          std::cout << num << ' ';
        }
        std::cout << '\n';
      }
      std::cout<<'\n';
      Matrix answer(buffer);
      if (answer.validSolution) {
        sum += answer.a * 3 + answer.b;
      }
      i = 0;
    }
  }
  std::cout << "The sum is: " << sum << '\n';
  return 0;
}
