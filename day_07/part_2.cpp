#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

class Calibration {
public:
  unordered_map<long long int, vector<int>> numbers;
  vector<int> valid_numbers;
  long long int valid_calibrations;

  int fillNumbers(string &line, char delim) {
    ssize_t pos = line.find(delim);
    if (pos == string::npos) {
      cerr << "Invalid input line: " << line << endl;
      return 0;
    }
    long long int key = stoll(line.substr(0, pos));
    long long int val;
    istringstream sline(line.substr(pos + 1));
    int i = 0;
    while (sline >> val) {
      numbers[key].push_back(val);
      i++;
    }
    return i;
  }

  void validCalibrations() {
    valid_calibrations = 0;
    for (const auto &[key, val] : numbers) {
      long int different_ways = possibleVariations(key, val);
      if (different_ways != 0) {
        valid_calibrations += key;
        valid_numbers.push_back(key);
      }
    }
  }

private:
  long long int concatenate(long long int a, long long int b) {
    return stoll(to_string(a) + to_string(b));
  }

  long int possibleVariations(long long int target, const vector<int> &nums) {
    int n = nums.size();
    long int count = 0;

    for (int mask = 0; mask < (1 << (2 * (n - 1))); mask++) {
      long long int result = nums[0];
      for (int i = 0; i < n - 1; i++) {
        int op = (mask >> (2 * i)) & 3;

        if (op == 0) {
          result += nums[i + 1];
        } else if (op == 1) {
          result *= nums[i + 1];
        } else {
          result = concatenate(result, nums[i + 1]);
        }
      }

      if (result == target) {
        count++;
      }
    }
    return count;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Program needs a <input-file>" << endl;
    return 1;
  }
  ifstream file(argv[1]);
  if (!file.is_open()) {
    cerr << "File " << argv[1] << " error" << endl;
    return 2;
  }
  char delim = ':';
  string line;
  Calibration answer;
  int read_nums = 0, lines = 0;
  while (getline(file, line) && !line.empty()) {
    read_nums += answer.fillNumbers(line, delim);
    lines++;
  }
  cout << lines << " lines read with total " << read_nums << " numbers\n";
  answer.validCalibrations();
  cout << "The sum of valid calibrations is " << answer.valid_calibrations
       << '\n';
  return 0;
}
