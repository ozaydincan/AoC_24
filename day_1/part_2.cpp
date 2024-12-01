#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

static constexpr const char *input = "lists.txt";

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
  std::vector<int> left_vec, right_vec;
  int count = readLists(input, right_vec, left_vec);
  std::unordered_map<int, int> similarity_map;
  for (int key : left_vec) {
    similarity_map[key] = 0;
  }
  for (int num : right_vec) {
    auto key = similarity_map.find(num);
    if (key != similarity_map.end()) {
      key->second++;
    }
  }
  int similarity_score = 0;
  for (const auto &kv : similarity_map) {
    similarity_score += (kv.first * kv.second);
  }
  std::cout << "The similarity score is " << similarity_score << '\n';
  return 0;
}
