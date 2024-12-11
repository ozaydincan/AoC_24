#include <cctype>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

struct PairHash {
  template <typename T, typename U>
  size_t operator()(const pair<T, U> &p) const {
    return hash<T>{}(p.first) ^ (hash<U>{}(p.second) << 1);
  }
};

class Trails {
public:
  Trails(const vector<vector<int>> &v)
      : hike_trails(v), row(hike_trails.size()), col(hike_trails[0].size()) {
  }

  long int countWays() {
    long int count = 0;
    for (int i = 0; i < row; i++) {
      for (int j = 0; j < col; j++) {
        if (hike_trails[i][j] == 0) {
          vector<vector<bool>> visited(row, vector<bool>(col, false));
          unordered_set<pair<int, int>, PairHash> nines;
          dfs(visited, i, j, 0, count, nines);
          cout << "For the trail head at {" << i << ',' << j
               << "} the score is " << nines.size() << '\n';
        }
      }
    }
    return count;
  }

  void printTrails() {
    for (const auto &i : hike_trails) {
      for (const int &num : i) {
        cout << (num != -1 ? to_string(num) : ".");
      }
      cout << '\n';
    }
  }

private:
  vector<vector<int>> hike_trails;
  size_t row, col;


  void dfs(vector<vector<bool>> visited, int x, int y, int target,
           long int &count,
           unordered_set<pair<int, int>, PairHash> &nine_locations) {
    if (x < 0 || x >= row || y < 0 || y >= col) {
      return;
    }

    if (visited[x][y] || hike_trails[x][y] != target) {
      return;
    } 

    if (target == 9) {
      count++;
      return;
    }

    visited[x][y] = true;
    dfs(visited, x - 1, y, target + 1, count, nine_locations); // up
    dfs(visited, x + 1, y, target + 1, count, nine_locations); // down
    dfs(visited, x, y + 1, target + 1, count, nine_locations); // right
    dfs(visited, x, y - 1, target + 1, count, nine_locations); // left

    visited[x][y] = false;
  }

};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }

  ifstream file(argv[1]);

  if (!file.is_open()) {
    return 2;
  }

  string line;

  vector<vector<int>> buffer;

  while (getline(file, line)) {
    vector<int> row;
    for (char ch : line) {
      if (isdigit(ch)) {
        row.push_back(ch - '0');
      } else {
        row.push_back(-1);
      }
    }
    buffer.push_back(row);
  }

  line.clear();
  file.close();

  Trails answer(buffer);
  buffer.clear();
    
  cout << answer.countWays()<<endl;

  return 0;
}
