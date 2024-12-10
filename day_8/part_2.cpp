#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

class Antinode {
public:
  Antinode(const vector<string> &matrix)
      : grid(matrix), row(grid.size()), col(grid[0].size()) {
    findAntennas();
    findAntinodes();
  }

  size_t getAntinodeCount() const { return antinodes.size(); }

private:
  struct PairHash {
  public:
    template <typename T, typename U>
    size_t operator()(const pair<T, U> &p) const {
      return hash<T>()(p.first) ^ (hash<U>()(p.second) << 1);
    }
  };

  vector<string> grid;
  ssize_t row, col;
  unordered_map<char, unordered_set<pair<int, int>, PairHash>>
      antenna_locations;
  unordered_set<pair<int, int>, PairHash> antinodes;

  void findAntennas() {
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        if (grid[i][j] != '.') {
          antenna_locations[grid[i][j]].insert({i, j});
        }
      }
    }
  }

  void findAntinodes() {
    for (const auto &[_, locations]: antenna_locations) {

      for (const auto &ant1 : locations) {
        for (const auto &ant2 : locations) {
          if (ant1 == ant2)
            continue;

          int dx = ant2.first - ant1.first;
          int dy = ant2.second - ant1.second;

          findAntinodeOnLine(ant1, dx, dy);
          findAntinodeOnLine(ant1, -dx, -dy);
        }
      }
    }
  }

  void findAntinodeOnLine(const pair<int, int> &start, int dx, int dy) {
    int x = start.first + dx;
    int y = start.second + dy;

    while (checkBoundaries(x, y)) {
      antinodes.insert({x, y});
      x += dx;
      y += dy;
    }
  }

  bool checkBoundaries(const int x, const int y) const {
    if (x >= 0 && x < row && y >= 0 && y < col) {
      return true;
    }
    return false;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Program needs an <input_file>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  if (!file.is_open()) {
    cerr << "Error opening file " << argv[1] << endl;
    return 2;
  }

  string line;
  vector<string> v;
  while (getline(file, line) && !line.empty()) {
    v.push_back(line);
  }

  file.close();

  Antinode answer(v);

  cout << "Total Antinodes: " << answer.getAntinodeCount() << endl;
  return 0;
}
