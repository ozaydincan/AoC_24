#include <iostream>
#include <string>
#include <vector>

class XmasWordSearch {
private:
  std::vector<std::vector<char>> grid;
  const std::string target = "XMAS";
  int rows, cols;

public:
  XmasWordSearch(const std::vector<std::vector<char>> &inputGrid)
      : grid(inputGrid), rows(inputGrid.size()), cols(inputGrid[0].size()) {}

  int countXmasOccurrences() {
    int count = 0;
    for (int r = 0; r < rows; ++r) {
      for (int c = 0; c < cols; ++c) {
        count += checkXmasAtPosition(r, c);
      }
    }
    return count;
  }

private:
  int checkXmasAtPosition(int startR, int startC) {
    const int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
    const int dy[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    int found = 0;

    for (int k = 0; k < 8; ++k) {
      bool valid = true;
      for (int i = 0; i < 4; ++i) {
        int r = startR + i * dx[k];
        int c = startC + i * dy[k];

        if (r < 0 || r >= rows || c < 0 || c >= cols ||
            grid[r][c] != target[i]) {
          valid = false;
          break;
        }
      }
      if (valid)
        found++;
    }
    return found;
  }
};

int main() {
  std::vector<std::vector<char>> grid;
  std::string line;

  while (std::getline(std::cin, line)) {
    if (line.empty())
      break;
    grid.push_back(std::vector<char>(line.begin(), line.end()));
  }

  if (grid.empty()) {
    std::cout << 0 << std::endl;
    return 0;
  }

  XmasWordSearch wordSearch(grid);
  std::cout << wordSearch.countXmasOccurrences() << std::endl;

  return 0;
}
