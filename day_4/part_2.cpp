#include <iostream>
#include <string>
#include <vector>

class xMAS {
public:
  long int count;
  xMAS(std::vector<std::string> &matrix, const char c)
      : grid(matrix), row(matrix.size() - 1), col(matrix[0].size() - 1),
        pivot(c), count(0) {}

  void getMAS() {
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        if (grid[i][j] == 'A') {
          bool found = checkGrid(i, j);

          if (found) {
            count++;
          }
        }
      }
    }
  }

private:
  std::vector<std::string> grid;
  char pivot;
  int row, col;
  int dirs[4][2] = {{-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
  int dir_row = 4, dir_col = 2;
  bool checkGrid(int x, int y) {
    for (int i = 0; i < dir_row; i++) {
      int x_new = x + dirs[i][0];
      int y_new = y + dirs[i][1];
      if (x_new < 0 || x_new > row || y_new > col || y_new < 0)
        return false;
    }
    bool config1 = (grid[x - 1][y - 1] == 'M' && grid[x - 1][y + 1] == 'M' &&
                    grid[x + 1][y - 1] == 'S' && grid[x + 1][y + 1] == 'S');
    bool config2 = (grid[x - 1][y - 1] == 'M' && grid[x - 1][y + 1] == 'S' &&
                    grid[x + 1][y - 1] == 'M' && grid[x + 1][y + 1] == 'S');
    bool config3 = (grid[x - 1][y - 1] == 'S' && grid[x - 1][y + 1] == 'S' &&
                    grid[x + 1][y - 1] == 'M' && grid[x + 1][y + 1] == 'M');
    bool config4 = (grid[x - 1][y - 1] == 'S' && grid[x - 1][y + 1] == 'M' &&
                    grid[x + 1][y - 1] == 'S' && grid[x + 1][y + 1] == 'M');
    return config1 || config2 || config3 || config4;
  }
}

;

int main(int argc, char *argv[]) {
  std::vector<std::string> grid;
  std::string line;
  std::cout << "Enter the input:\n";
  while (std::getline(std::cin, line) && !line.empty()) {
    grid.push_back(line);
  }
  for (auto &l : grid) {
    std::cout << l << '\n';
  }
  xMAS sol(grid, 'A');
  sol.getMAS();
  std::cout << "There are -> " << sol.count << std::endl;

  return 0;
}
