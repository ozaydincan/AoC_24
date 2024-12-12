#include <exception>
#include <fstream>
#include <iostream>
#include <algorithm>
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
    if (!validOrder(update) || update.empty()) {
      return 0;
    }
    cout << "Checking: ";
    for (int page : update) {
      cout << page << ",";
    }
    cout << " - Valid" << endl;
    return update[update.size()  2];
  }

private:
  char delim = '|';
  unordered_map<int, unordered_set<int>> order;

  bool validOrder(const vector<int> &update) {
    for (int i = 0; i < update.size(); i++) {
      for (int j = 0; j < update.size(); j++) {

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
      cout << "First input read moving to the second one" << '\n';
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

  cout << "Sum of middle pages: " << mid << endl;
  return 0;
}
