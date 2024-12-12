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
    moveChunks();
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
  void moveChunks() {
    long long int right= files.size()-1;
    long long int max_id;
    while(files[right]!=-1){
      max_id = files[right];
        right--;
    }

    for (long long int current_id = max_id; current_id >= 0; current_id--) {
      size_t first = -1, last = -1;
      for (size_t i = 0; i < files.size(); i++) {
        if (files[i] == current_id) {
          if (first == -1)
            first = i;
          last = i;
        }
      }

      if (first == -1)
        continue;

      size_t file_length = last - first + 1;

      bool moved = false;
      for (size_t leftmost_free = 0; leftmost_free < first && !moved;
           leftmost_free++) {
        size_t free_space = 0;
        size_t j = leftmost_free;

        while (j < files.size() && files[j] == -1 && free_space < file_length) {
          free_space++;
          j++;
        }

        if (free_space >= file_length) {
          std::vector<long long int> file_chunk;
          for (size_t k = first; k <= last; k++) {
            file_chunk.push_back(files[k]);
            files[k] = -1;
          }

          std::copy(file_chunk.begin(), file_chunk.end(),
                    files.begin() + leftmost_free);

          moved = true;
        }
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
