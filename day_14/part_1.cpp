#include <cctype>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

constexpr const int GRID_WIDTH = 101;  // Width
constexpr const int GRID_HEIGHT = 103; // Height

struct PairHash {
  size_t operator()(const std::pair<int, int> &p) const {
    return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
  }
};

class Robots {
public:
  Robots(long start_x, long start_y, long v_x, long v_y)
      : start({start_x, start_y}), velocity({v_x, v_y}) {
    finalPosition();
  };
  std::pair<long, long> final_pos;

private:
  std::pair<long, long> start;
  std::pair<long, long> velocity;

  void wraparound(std::pair<long, long> &pos) {
    pos.first = (pos.first % GRID_WIDTH + GRID_WIDTH) % GRID_WIDTH;
    pos.second = (pos.second % GRID_HEIGHT + GRID_HEIGHT) % GRID_HEIGHT;
  }

  void finalPosition() {
    final_pos.first = (100 * velocity.first) + start.first;
    final_pos.second = (100 * velocity.second) + start.second;
    wraparound(final_pos);
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Needs one command-line argument" << std::endl;
    return 1;
  }

  std::ifstream file(argv[1]);

  if (!file.is_open()) {
    std::cerr << "File couldn't be opened" << std::endl;
    return 2;
  }

  std::vector<std::vector<char>> grid(GRID_HEIGHT,
                                      std::vector<char>(GRID_WIDTH, '.'));
  std::string line;
  std::unordered_map<std::pair<int, int>, int, PairHash> positions;
  std::regex pattern(R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");

  while (std::getline(file, line) && !line.empty()) {
    std::smatch matches;
    if (std::regex_search(line, matches, pattern)) {
      long pos_x = std::stol(matches[1]);
      long pos_y = std::stol(matches[2]);
      long v_x = std::stol(matches[3]);
      long v_y = std::stol(matches[4]);

      Robots guard(pos_x, pos_y, v_x, v_y);
      positions[guard.final_pos]++;
    }
  }

  const int middleRow = GRID_HEIGHT / 2;
  const int middleCol = GRID_WIDTH / 2;
  std::cout << "Middle row: " << middleRow << "\nMiddle column:" << middleCol
            << '\n';

  int sum = 0;

  int top_left = 0, top_right = 0, bottom_left = 0, bottom_right = 0;

  for (auto &[pos, count] : positions) {
    int x = pos.first;
    int y = pos.second;
    grid[y][x] = '0' + count;

    if (y == middleRow || x == middleCol) {
      continue;
    }

    if (y < middleRow && x < middleCol) {
      top_left += count;
    } else if (y < middleRow && x >= middleCol) {
      top_right += count;
    } else if (y >= middleRow && x < middleCol) {
      bottom_left += count;
    } else if (y >= middleRow && x >= middleCol) {
      bottom_right += count;
    }
  }

  int safety_factor = top_left * top_right * bottom_left * bottom_right;

  std::cout << "The safety factor is: " << safety_factor << std::endl;

  return 0;
}
