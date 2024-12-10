#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

class Antinodes {
public:
  long int unique_locations;
  vector<string> grid;

  Antinodes(const vector<string> &v)
      : grid(v), row(v.size()), col(v[0].size()) {}

  void iterRows() {
    for (int i = 0; i < row; i++) {
      findAntennas(grid[i], i);
    }

    populateAntiNodes();

    unique_locations = antinode_pos.size();
  }

  void printAntennas() {
    for (const auto &freq : antennas) {
      cout << "Frequency " << freq.first << ": ";
      for (const auto &pos : freq.second) {
        cout << "(" << pos.first << "," << pos.second << ") ";
      }
      cout << endl;
    }
  }

private:
  int row, col;

  struct PairHash {
    template <typename T1, typename T2>
    size_t operator()(const std::pair<T1, T2> &p) const {
      auto h1 = std::hash<T1>{}(p.first);
      auto h2 = std::hash<T2>{}(p.second);
      return h1 ^ (h2 << 1); // Combine the two hash values
    }
  };

  unordered_map<char, unordered_set<pair<int, int>, PairHash>> antennas;
  unordered_set<pair<int, int>, PairHash> antinode_pos;

  int findAntennas(const string &line, int row) {
    ssize_t n = line.size();
    int antenna = 0;
    for (int i = 0; i < n; i++) {
      if (line[i] != '.') {
        antennas[line[i]].insert(make_pair(row, i));
        antenna++;
      }
    }
    return antenna;
  }

  void populateAntiNodes() {
    for (const auto &freq_antennas : antennas) {
      char freq = freq_antennas.first;
      const auto &antenna_set = freq_antennas.second;

      for (auto it1 = antenna_set.begin(); it1 != antenna_set.end(); ++it1) {
        for (auto it2 = next(it1); it2 != antenna_set.end(); ++it2) {
          int row1 = it1->first, col1 = it1->second;
          int row2 = it2->first, col2 = it2->second;

          int row_diff = row2 - row1;
          int col_diff = col2 - col1;

          int antinode1_row = row1 - row_diff;
          int antinode1_col = col1 - col_diff;

          int antinode2_row = row2 + row_diff;
          int antinode2_col = col2 + col_diff;

          if (isValidAntinode(antinode1_row, antinode1_col, row1, col1, row2,
                              col2)) {
            antinode_pos.insert(make_pair(antinode1_row, antinode1_col));
          }

          if (isValidAntinode(antinode2_row, antinode2_col, row1, col1, row2,
                              col2)) {
            antinode_pos.insert(make_pair(antinode2_row, antinode2_col));
          }
        }
      }
    }
  }

  bool isValidAntinode(int antinode_row, int antinode_col, int row1, int col1,
                       int row2, int col2) {
    if (antinode_row < 0 || antinode_row >= row || antinode_col < 0 ||
        antinode_col >= col) {
      return false;
    }

    if ((antinode_row >= min(row1, row2) && antinode_row <= max(row1, row2)) &&
        (antinode_col >= min(col1, col2) && antinode_col <= max(col1, col2))) {
      return false;
    }

    char freq1 = grid[row1][col1];
    char freq2 = grid[row2][col2];
    if (freq1 != freq2) {
      return false;
    }

    return true;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Input file needed" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  if (!file.is_open()) {
    cerr << "File error" << endl;
    return 2;
  }

  string line;
  vector<string> mat;
  while (getline(file, line) && !line.empty()) {
    mat.push_back(line);
  }

  Antinodes solution(mat);
  solution.iterRows();
  solution.printAntennas();

  cout << "Unique Antinode Locations: " << solution.unique_locations << endl;

  return 0;
}
