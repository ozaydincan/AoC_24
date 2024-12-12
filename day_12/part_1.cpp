#include <cstddef>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

class GardenGroups {
public:
  vector<tuple<char, int, int>> plantInfo;
  GardenGroups(const vector<string> &grid)
      : rows(grid.size()), cols(grid[0].size()) {
    populatePlants(grid);
    plantPrices();
  };

  void plantPrices() {
    unsigned long long int sums = 0;
    for (auto &[plant, area, perimeter] : plantInfo) {
      cout << "The price of " << plant << " is " << to_string(area * perimeter)
           << ". Area: " << to_string(area)
           << " Perimeter: " << to_string(perimeter) << endl;
      sums += area * perimeter;
    }

    cout << "Total sum of prices is: " << sums << endl;
  }

private:
  size_t rows, cols;

  void populatePlants(const vector<string> &grid) {
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    for (size_t i = 0; i < rows; i++) {
      for (size_t j = 0; j < cols; j++) {
        if (!visited[i][j]) {
          pair<int, int> result = bfs(visited, grid, i, j);
          plantInfo.push_back({grid[i][j], result.first, result.second});
        }
      }
    }
  }

  pair<int, int> bfs(vector<vector<bool>> &visited,
                     const vector<string> &gardens, size_t x, size_t y) {

    int perimeter = 0;
    char target = gardens[x][y];
    int area = 0;

    queue<pair<size_t, size_t>> q;
    q.push({x, y});
    visited[x][y] = true;

    const vector<vector<ssize_t>> dirs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    while (!q.empty()) {
      size_t current_r = q.front().first;
      size_t current_col = q.front().second;

      q.pop();
      area++;

      for (const vector<ssize_t> &dir_row : dirs) {
        ssize_t newX = current_r + dir_row[0];
        ssize_t newY = current_col + dir_row[1];

        if (newX < 0 || newX >= rows || newY < 0 || newY >= cols ||
            gardens[newX][newY] != target) {
          perimeter++;
        } else if (!visited[newX][newY]) {
          visited[newX][newY] = true;
          q.push({newX, newY});
        }
      }
    }
    return {area, perimeter};
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Program needs an <input-file>!" << endl;
    return 1;
  }

  ifstream file(argv[1]);

  if (!file.is_open()) {
    cerr << "The file " << argv[1] << " couldn't be opened!" << endl;
    return 2;
  }

  string line;
  vector<string> buffer;

  while (getline(file, line), !line.empty()) {
    buffer.push_back(line);
  }

  GardenGroups answer(buffer);
  buffer.clear();

  return 0;
}
