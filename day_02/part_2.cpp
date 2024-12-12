#include <exception>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Reports {
public:
  const std::string input_file;
  size_t report_count;
  size_t safe_count = 0;

  Reports(const std::string &filename) : input_file(filename) {}

  bool safe_reports() {
    try {
      report_count = read_levels(input_file);
      for (const auto &row : report) {
        if (row.size() < 2)
          continue;

        if (is_safe(row)) {
          ++safe_count;
        } else if (is_safe_with_dampener(row)) {
          ++safe_count;
        }
      }
      return true;
    } catch (const std::exception &exception) {
      std::cerr << "Error: " << exception.what() << std::endl;
      return false;
    }
  }

private:
  std::vector<std::vector<int>> report;

  size_t read_levels(const std::string &filename) {
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

  bool is_safe(const std::vector<int> &levels) {
    int n = levels.size();
    int initial_diff = levels[1] - levels[0];

    if (std::abs(initial_diff) < 1 || std::abs(initial_diff) > 3) {
      return false;
    }

    for (int i = 2; i < n; ++i) {
      int diff = levels[i] - levels[i - 1];
      if ((initial_diff > 0 && diff <= 0) || (initial_diff < 0 && diff >= 0) ||
          std::abs(diff) < 1 || std::abs(diff) > 3) {
        return false;
      }
    }
    return true;
  }

  bool is_safe_with_dampener(const std::vector<int> &levels) {
    int n = levels.size();
    if (n <= 2)
      return true;

    for (int i = 0; i < n; ++i) {
      std::vector<int> modified_levels;
      for (int j = 0; j < n; ++j) {
        if (j != i)
          modified_levels.push_back(levels[j]);
      }

      if (is_safe(modified_levels)) {
        return true;
      }
    }

    return false;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  Reports reports(argv[1]);
  if (reports.safe_reports()) {
    std::cout << "There are " << reports.safe_count << " safe reports.\n";
  } else {
    std::cout << "No safe reports.\n";
  }

  return 0;
}
