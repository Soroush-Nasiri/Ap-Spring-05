#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <cstring>

using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool isEnd;
    TrieNode() {
        memset(children, 0, sizeof(children));
        isEnd = false;
    }
};

void insertWord(TrieNode* root, const string& word) {
    TrieNode* node = root;
    for (char ch : word) {
        int idx = ch - 'a';
        if (!node->children[idx])
            node->children[idx] = new TrieNode();
        node = node->children[idx];
    }
    node->isEnd = true;
}

void loadDictionary(TrieNode* root, int n) {
    string word;
    for (int i = 0; i < n; ++i) {
        cin >> word;
        insertWord(root, word);
    }
}

void backtrack(TrieNode* node, int freq[26], string& current, set<string>& results) {
    for (int i = 0; i < 26; ++i) {
        if (freq[i] > 0 && node->children[i]) {
            freq[i]--;
            current.push_back('a' + i);
            TrieNode* next = node->children[i];
            if (next->isEnd)
                results.insert(current);
            backtrack(next, freq, current, results);
            current.pop_back();
            freq[i]++;
        }
    }
}

void processQuery(TrieNode* root, const string& line) {
    vector<char> letters;
    stringstream ss(line);
    char ch;
    while (ss >> ch)
        letters.push_back(ch);

    int freq[26] = {0};
    for (char c : letters)
        freq[c - 'a']++;

    set<string> results;
    string current;
    backtrack(root, freq, current, results);

    if (results.empty()) {
        cout << "0\n";
    } else {
        cout << results.size() << "\n";
        for (const string& word : results)
            cout << word << "\n";
    }
}

void runGame() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    TrieNode* root = new TrieNode();
    loadDictionary(root, n);

    int q;
    cin >> q;
    cin.ignore();

    for (int i = 0; i < q; ++i) {
        string line;
        getline(cin, line);
        processQuery(root, line);
    }
}

int main() {
    runGame();
    return 0;
}