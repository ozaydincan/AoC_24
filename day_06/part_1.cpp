#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

class GuardGrid {
public:
  GuardGrid(vector<string> &grid)
      : grid(grid), row(grid.size()), col(grid[0].size()), total_visited(0) {
    initializeGuard();
  }

  int predictPath() {
    set<pair<int, int>> visited;
    visited.insert(guard_pos);
    grid[guard_pos.first][guard_pos.second] = 'X';

    while (true) {
      auto [next_row, next_col] = getNextPosition();
      if (isOutOfBounds(next_row, next_col))
        break;

      if (grid[next_row][next_col] == '#') {
        turnRight();
      } else {
        guard_pos = {next_row, next_col};
        visited.insert(guard_pos);
        grid[guard_pos.first][guard_pos.second] = 'X';
      }
    }

    total_visited = visited.size();
    return total_visited;
  }

  void printGrid() const {
    for (const auto &row : grid) {
      cout << row << endl;
    }
  }

private:
  size_t row, col;
  int total_visited;
  vector<string> grid;
  pair<int, int> guard_pos;
  pair<int, int> guard_dir;

  void initializeGuard() {
    for (size_t i = 0; i < row; ++i) {
      for (size_t j = 0; j < col; ++j) {
        if (isGuardStart(grid[i][j])) {
          guard_pos = {i, j};
          guard_dir = getDirection(grid[i][j]);
          grid[i][j] = 'X';
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
  int visited_count = guard.predictPath();
  cout << "Distinct positions visited: " << visited_count << "\n";
  cout << "Final grid after patrol:\n";
  guard.printGrid();

  return 0;
}
