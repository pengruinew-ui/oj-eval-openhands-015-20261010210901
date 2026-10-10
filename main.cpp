#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>

using namespace std;

const int BUCKET_COUNT = 20;

string bucket_file(const string &idx) {
    size_t h = hash<string>{}(idx);
    int b = h % BUCKET_COUNT;
    return "bucket_" + to_string(b) + ".txt";
}

void ensure_file(const string &path) {
    if (!filesystem::exists(path)) {
        ofstream ofs(path);
        // create empty file
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    string cmd;
    for (int i = 0; i < n; ++i) {
        cin >> cmd;
        if (cmd == "insert") {
            string idx; long long val; // value fits int
            cin >> idx >> val;
            string file = bucket_file(idx);
            ensure_file(file);
            // check duplicate
            bool exists = false;
            {
                ifstream in(file);
                string line;
                while (getline(in, line)) {
                    stringstream ss(line);
                    string i2; long long v2;
                    ss >> i2 >> v2;
                    if (i2 == idx && v2 == val) { exists = true; break; }
                }
            }
            if (!exists) {
                ofstream out(file, ios::app);
                out << idx << ' ' << val << '\n';
            }
        } else if (cmd == "delete") {
            string idx; long long val;
            cin >> idx >> val;
            string file = bucket_file(idx);
            if (!filesystem::exists(file)) continue; // nothing to delete
            string temp = file + ".tmp";
            ifstream in(file);
            ofstream out(temp);
            string line;
            while (getline(in, line)) {
                stringstream ss(line);
                string i2; long long v2;
                ss >> i2 >> v2;
                if (i2 == idx && v2 == val) continue; // skip delete
                out << line << '\n';
            }
            in.close(); out.close();
            filesystem::rename(temp, file);
        } else if (cmd == "find") {
            string idx;
            cin >> idx;
            string file = bucket_file(idx);
            vector<long long> vals;
            if (filesystem::exists(file)) {
                ifstream in(file);
                string line;
                while (getline(in, line)) {
                    stringstream ss(line);
                    string i2; long long v2;
                    ss >> i2 >> v2;
                    if (i2 == idx) vals.push_back(v2);
                }
            }
            if (vals.empty()) {
                cout << "null\n";
            } else {
                sort(vals.begin(), vals.end());
                for (size_t j = 0; j < vals.size(); ++j) {
                    if (j) cout << ' ';
                    cout << vals[j];
                }
                cout << '\n';
            }
        }
    }
    return 0;
}
