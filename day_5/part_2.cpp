#include <exception>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

class Pages {
public:
  void getOrders(string &line) {
    size_t pos = line.find(delim);
    try {
      if (pos != string::npos) {
        int key = stoi(line.substr(0, pos));
        int val = stoi(line.substr(pos + 1));
        order[key].insert(val);
      }
    } catch (exception &e) {
      cerr << e.what() << '\n';
    }
  }

  int findMiddle(const vector<int> &update) {
    vector<int> corrected = correctOrder(update);
    if (corrected.empty()) {
      return 0;
    }

    if (corrected != update) {
      int middlePage = corrected[corrected.size() / 2];
      return middlePage;
    }

    return 0;
  }

private:
  bool validOrder(const vector<int> &update) {
    for (int i = 0; i < update.size(); i++) {
      for (int j = 0; j < update.size(); j++) {
        if (i == j)
          continue;

        auto before = order.find(update[i]);
        if (before != order.end()) {
          if (before->second.count(update[j]) > 0) {
            auto it_i = find(update.begin(), update.end(), update[i]);
            auto it_j = find(update.begin(), update.end(), update[j]);

            if (it_i > it_j) {
              return false;
            }
          }
        }
      }
    }
    return true;
  }

  vector<int> correctOrder(const vector<int> &update) {
    vector<int> corrected = update;
    if (validOrder(corrected)) {
      return corrected;
    }

    bool swapped = true;
    while (swapped) {
      swapped = false;
      for (size_t i = 0; i < corrected.size() - 1; i++) {
        if (!validOrder({corrected[i], corrected[i + 1]})) {
          swap(corrected[i], corrected[i + 1]);
          swapped = true;
        }
      }
    }
    return corrected;
  }

  char delim = '|';
  unordered_map<int, unordered_set<int>> order;
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << argv[0] << " needs an <input_file>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  if (!file.is_open()) {
    cerr << "File error" << endl;
    return 1;
  }

  Pages manual;
  string line;

  while (getline(file, line)) {
    if (line.empty()) {
      break;
    }
    manual.getOrders(line);
  }

  int mid = 0;
  while (getline(file, line) && !line.empty()) {
    vector<int> update;
    size_t pos = 0, next_pos = 0;

    while ((next_pos = line.find(',', pos)) != string::npos) {
      update.push_back(stoi(line.substr(pos, next_pos - pos)));
      pos = next_pos + 1;
    }
    if (pos < line.length()) {
      update.push_back(stoi(line.substr(pos)));
    }

    mid += manual.findMiddle(update);
  }

  cout << "Sum of middle pages of incorrectly-ordered updates: " << mid << endl;
  return 0;
}
