#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class FileSystem {
public:
  long long int checksum;
  FileSystem(string &input) : checksum(0), n(input.size()) {
    createFiles(input);
    pushFilesLeft();
    getChecksum();
  }
  void printVec() {
    for (auto num : files) {
      cout << (num == -1 ? "." : to_string(num));
    }
    cout << endl;
  }
  void getChecksum() {
    long long int sol = 0;
    for (long long int i = 0; i < files.size(); i++) {
      if (files[i] == -1) {
        continue;
      }
      sol += files[i] * i;
    }
    checksum = sol;
  }

private:
  size_t n;
  vector<long long int> files;

  void createFiles(string &line) {
    unsigned long long ids = 0;
    for (size_t i = 0; i < n; i++) {
      int num = line[i] - '0';
      if (i % 2 == 0) {
        for (size_t j = 0; j < num; j++) {
          files.push_back(ids);
        }
        ids++;
      } else {
        for (size_t j = 0; j < num; j++) {
          files.push_back(-1);
        }
      }
    }
  }

  void pushFilesLeft() {
    size_t left = 0, right = files.size() - 1;
    while (left <= right) {
      if (files[left] == -1 && files[right] != -1) {
        files[left] = files[right];
        files[right] = -1;
        left++;
        right--;
      } else if (files[left] != -1 && files[right] != -1) {
        left++;
      } else if (files[left] == -1 && files[right] == -1) {
        right--;
      } else {
        left++;
        right--;
      }
    }
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Program needs <input_file>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  if (!file.is_open()) {
    cerr << "Error opening file " << argv[1] << endl;
    return 2;
  }

  string line;
  getline(file, line);
  FileSystem answer(line);
  cout << "Input size is " << line.size() << '\n';
  line.clear();

  cout << "The answer is " << answer.checksum << endl;
  return 0;
}
