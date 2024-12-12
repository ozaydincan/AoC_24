#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

static constexpr const char *filename = "lists.txt";

int readLists(const char *infile, std::vector<int> &rlist,
              std::vector<int> &llist) {
  std::ifstream file(infile);
  if (!file) {
    std::cerr << "Error opening file";
  }
  int l_num, r_num;
  int count = 0;
  while (!file.eof()) {
    if (file >> l_num >> r_num) {
      llist.push_back(l_num);
      rlist.push_back(r_num);
      count++;
    } else {
      file.clear();
      std::string error_line;
      std::getline(file, error_line);
      std::cerr << "Invalid line " << error_line << std::endl;
    }
  }
  return count;
}

int main(int argc, char *argv[]) {
  std::vector<int> left_vec;
  std::vector<int> right_vec;
  int count = readLists(filename, right_vec, left_vec);
  std::sort(left_vec.begin(), left_vec.end());
  std::sort(right_vec.begin(), right_vec.end());
  int sum = 0;
  for (int i = 0; i < count; i++){
    sum += abs(right_vec[i] - left_vec[i]);
  }
  std::cout << "Total distance is "<< sum << std::endl;
  return 0;
}
