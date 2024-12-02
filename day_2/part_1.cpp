#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class Reports {
public:
  const std::string input_file = "reports.txt";
  size_t report_count;
  size_t safe_count = 0;
  bool safe_reports() {
    try {
      report_count = readLevels(input_file);
      for (const auto &row : report) {
        if (row.size() < 2) {
          continue;;
        }
        int diff = row[0] - row[1];
        bool flag = true;
        if (std::abs(diff)<1 || std::abs(diff)>3){
          flag = false;
          continue;
        }
        for (int i = 1; i < row.size() - 1; i++) {
          int new_diff = row[i] - row[i + 1];
          if ((new_diff ^ diff) < 0 || std::abs(new_diff) < 1 || std::abs(new_diff) > 3) {
            flag = false;
            break;
          }
        }
        if (flag) {
          safe_count++;
        }
      }
      return true;
    } catch (std::exception &exception) {
      std::cerr << "Error: " << exception.what() << std::endl;
      return false;
    }
  }

private:
  std::vector<std::vector<int>> report;
  size_t readLevels(const std::string filename) {
    std::ifstream file(filename);
    if (!file) {
      std::cerr << "Input file error" << '\n';
      throw std::runtime_error("Input file error");
    }
    std::string line;
    while (std::getline(file, line)) {
      std::vector<int> row;
      std::istringstream line_stream(line);
      int num;
      while (line_stream >> num) {
        row.push_back(num);
      }
      report.push_back(row);
    }

    if (report.back().empty()) {
      report.pop_back();
    }
    return report.size();
  }
};

int main(int argc, char *argv[]) {
  Reports rep;
  if (rep.safe_reports()) {
    std::cout << "There are " << rep.safe_count << '\n';
  }
  else{
    std::cout << "No safe reports" << '\n';
  }
  return 0;
}
