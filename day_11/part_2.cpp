#include <cstddef>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>

using namespace std;

class Stones {
public:
  unordered_map<long long int, long long int> stone_cycle;
  size_t loop_size;
  Stones(size_t loop_size, fstream &input)
      : stone_cycle(), loop_size(loop_size) {
    initializeMap(input);
  }

  void blinkStones(size_t &loop) {
    for (size_t i = 0; i < loop; i++) {
      unordered_map<long long int, long long int> temp;
      for (auto &[key, val] : stone_cycle) {
        if (key == 0) {
          temp[1] += val;
        } else if (to_string(key).size() % 2 == 0) {
          string stone_str = to_string(key);
          long long int first =
              stoll(stone_str.substr(0, stone_str.size() / 2));
          long long int second = stoll(stone_str.substr(stone_str.size() / 2));
          temp[first] += val;
          temp[second] += val;
        } else {
          temp[key * 2024] += val;
        }
      }
      stone_cycle = temp;
    }
  }

  void printStones() {
    for (auto &[key, val] : stone_cycle) {
      cout << key << "-> " << val << ", ";
    }
    cout << '\n';
  }

private:
  void initializeMap(fstream &file) {
    long int num;
    while (file >> num) {
      stone_cycle[num]++;
    }
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Program needs an <input_file>" << endl;
    return 1;
  }

  fstream file(argv[1]);

  if (!file.is_open()) {
    cerr << "Error opening file" << endl;
    return 2;
  }

  Stones answer(75, file);

  answer.blinkStones(answer.loop_size);
  unsigned long long total = 0;
  for (auto &[key, val] : answer.stone_cycle) {
    total += val;
  }
  cout << "After " << answer.loop_size << " blinks there are " << total
       << " stones.\n";
  return 0;
}
