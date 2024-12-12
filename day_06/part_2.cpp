#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

class GuardGrid {
public:
  GuardGrid(vector<string> grid)
      : original_grid(grid), grid(grid), row(grid.size()), col(grid[0].size()) {
    initializeGuard();
  }

  int findLoopBlockingPositions() {
    int loop_positions = 0;
    for (size_t i = 0; i < row; ++i) {
      for (size_t j = 0; j < col; ++j) {
        if (isGuardStart(original_grid[i][j]))
          continue;

        if (original_grid[i][j] == '#')
          continue;

        if (wouldCauseLoop(i, j)) {
          grid[i][j] = 'o';
          loop_positions++;
        }
      }
    }
    return loop_positions;
  }

  void printGrid() const {
    for (const auto &row : grid) {
      cout << row << endl;
    }
  }

private:
  size_t row, col;
  vector<string> original_grid;
  vector<string> grid;
  pair<int, int> guard_pos;
  pair<int, int> guard_dir;

  bool wouldCauseLoop(size_t block_row, size_t block_col) {

    grid = original_grid;
    initializeGuard();

    grid[block_row][block_col] = '#';

    set<pair<int, int>> visited;
    set<pair<pair<int, int>, pair<int, int>>> state_history;

    int max_iterations = row * col * 2; 
    int iterations = 0;

    while (iterations < max_iterations) {
      auto current_state = make_pair(guard_pos, guard_dir);
      if (state_history.count(current_state)) {
        return true;
      }
      state_history.insert(current_state);

      auto [next_row, next_col] = getNextPosition();

      if (isOutOfBounds(next_row, next_col)) {
        return false;
      }

      if (grid[next_row][next_col] == '#') {
        turnRight();
      } else {
        guard_pos = {next_row, next_col};
      }

      iterations++;
    }
    return false;
  }

  void initializeGuard() {
    for (size_t i = 0; i < row; ++i) {
      for (size_t j = 0; j < col; ++j) {
        if (isGuardStart(grid[i][j])) {
          guard_pos = {i, j};
          guard_dir = getDirection(grid[i][j]);
          return;
        }
      }
    }
  }

  bool isGuardStart(char c) const {
    return c == '^' || c == 'v' || c == '<' || c == '>';
  }

  pair<int, int> getDirection(char arrow) const {
    switch (arrow) {
    case '^':
      return {-1, 0};
    case 'v':
      return {1, 0};
    case '<':
      return {0, -1};
    case '>':
      return {0, 1};
    default:
      return {0, 0};
    }
  }

  pair<int, int> getNextPosition() const {
    return {guard_pos.first + guard_dir.first,
            guard_pos.second + guard_dir.second};
  }

  void turnRight() {
    if (guard_dir == make_pair(-1, 0)) {
      guard_dir = {0, 1};
    } else if (guard_dir == make_pair(0, 1)) {
      guard_dir = {1, 0};
    } else if (guard_dir == make_pair(1, 0)) {
      guard_dir = {0, -1};
    } else if (guard_dir == make_pair(0, -1)) {
      guard_dir = {-1, 0};
    }
  }

  bool isOutOfBounds(int x, int y) const {
    return x < 0 || y < 0 || x >= static_cast<int>(row) ||
           y >= static_cast<int>(col);
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }
  ifstream file(argv[1]);
  if (!file.is_open()) {
    cerr << "File " << argv[1] << " could not be opened\n";
    return 2;
  }
  vector<string> grid;
  string line;
  while (getline(file, line)) {
    if (!line.empty()) {
      grid.push_back(line);
    }
  }
  GuardGrid guard(grid);
  int loop_positions = guard.findLoopBlockingPositions();
  cout << "Positions that would cause the guard to get stuck in a loop: "
       << loop_positions << "\n";
  guard.printGrid();
  return 0;
}
