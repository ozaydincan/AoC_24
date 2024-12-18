#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

constexpr const int GRID_WIDTH = 101;
constexpr const int GRID_HEIGHT = 103;

struct PairHash {
  size_t operator()(const std::pair<int, int> &p) const {
    return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
  }
};

class Robots {
public:
  Robots(long start_x, long start_y, long v_x, long v_y)
      : start({start_x, start_y}), velocity({v_x, v_y}) {};

  std::pair<long, long> final_pos;

  void finalPosition(size_t second) {
    final_pos.first = (second * velocity.first) + start.first;
    final_pos.second = (second * velocity.second) + start.second;
    wraparound(final_pos);
  }

private:
  std::pair<long, long> start;
  std::pair<long, long> velocity;

  void wraparound(std::pair<long, long> &pos) {
    pos.first = (pos.first % GRID_WIDTH + GRID_WIDTH) % GRID_WIDTH;
    pos.second = (pos.second % GRID_HEIGHT + GRID_HEIGHT) % GRID_HEIGHT;
  }
};

int countNeighbors(const std::pair<long, long> &position,
                   const std::unordered_set<std::pair<long, long>, PairHash>
                       &robot_positions) {
  int neighbor_count = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0)
        continue;
      std::pair<long, long> neighbor_pos = {position.first + dx,
                                            position.second + dy};
      if (robot_positions.count(neighbor_pos) > 0) {
        neighbor_count++;
      }
    }
  }
  return neighbor_count;
}

bool hasChristmasTree(const std::unordered_set<std::pair<long, long>, PairHash>
                          &robot_positions) {
  for (int y = 0; y < GRID_HEIGHT; ++y) {
    std::vector<long> x_coords;
    for (const auto &pos : robot_positions) {
      if (pos.second == y) {
        x_coords.push_back(pos.first);
      }
    }

    std::sort(x_coords.begin(), x_coords.end());

    int consecutive_count = 1;
    for (size_t i = 1; i < x_coords.size(); ++i) {
      if (x_coords[i] == x_coords[i - 1] + 1) {
        consecutive_count++;
      } else {
        consecutive_count = 1;
      }

      if (consecutive_count >= 10) {
        return true;
      }
    }
  }

  return false;
}

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

  std::string line;
  std::regex pattern(R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))");
  std::vector<Robots> guards;

  while (std::getline(file, line) && !line.empty()) {
    std::smatch matches;
    if (std::regex_search(line, matches, pattern)) {
      long pos_x = std::stol(matches[1]);
      long pos_y = std::stol(matches[2]);
      long v_x = std::stol(matches[3]);
      long v_y = std::stol(matches[4]);

      Robots guard(pos_x, pos_y, v_x, v_y);
      guards.push_back(guard);
    }
  }

  for (size_t second = 1; second < GRID_HEIGHT * GRID_WIDTH; second++) {
    std::unordered_set<std::pair<long, long>, PairHash> robot_positions;

    for (auto &guard : guards) {
      guard.finalPosition(second);
      robot_positions.insert(guard.final_pos);
    }

    if (hasChristmasTree(robot_positions)) {
      std::cout << "Christmas tree found at second: " << second << '\n';
      break;
    }
  }

  return 0;
}
