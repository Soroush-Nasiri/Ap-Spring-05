#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>

using namespace std;

string randomCase(const string& word) {
    string result = word;
    for (char& c : result) {
        if (isalpha((unsigned char)c)) {
            c = (rand() % 2) ? toupper((unsigned char)c) : tolower((unsigned char)c);
        }
    }
    return result;
}

string addPunctuation(const string& word) {
    string puncts = ".,!?;:";
    int r = rand() % 4;
    if (r == 0) return word + puncts[rand() % puncts.size()];
    return word;
}

int main() {
    srand(time(0));

    vector<string> normalWords = {
        "the", "and", "but", "for", "with", "from", "this", "that", "have",
        "hello", "world", "test", "beautiful", "hard", "simple", "complex",
        "nature", "forest", "river", "mountain", "ocean", "sky", "cloud",
        "cat", "dog", "bird", "fish", "lion", "tiger", "horse", "rabbit",
        "apple", "bread", "water", "coffee", "sugar", "rice", "fruit",
        "happy", "sad", "fast", "slow", "bright", "dark", "warm", "cold",
        "run", "jump", "read", "write", "think", "learn", "build", "create",
        "time", "place", "people", "city", "house", "road", "door", "window",
        "book", "page", "word", "line", "code", "program", "data", "file",
        "sun", "moon", "star", "rain", "wind", "fire", "earth", "stone"
    };

    vector<string> punctWords = {
        "don't", "can't", "won't", "it's", "he's", "she's", "they're",
        "part-time", "well-known", "up-to-date", "long-term", "short-term",
        "self-made", "high-quality", "low-cost", "open-source", "real-time"
    };

    vector<string> edgeCaseTokens = {
        "123", "0569", "42", "123.21", "0.1010", "1.11.123", "21-21", "11a?21", "??1??", "00.11.12.14", "275.21.13", "??12.33!!!",
        "!!12.12!!", "00.00", "!", "?", ".", ":", "99!", "83331?", ":9771:", "!!.::", "!!", "no.way", "come!on", "bro",
    };

    vector<string> allWords = normalWords;
    for (auto& w : punctWords) allWords.push_back(w);

    int numTests = 60;

    for (int t = 1; t <= numTests; t++) {
        string filename = "input" + to_string(t) + ".txt";
        ofstream out(filename);

        int numFrequent = 2 + rand() % 3;
        vector<string> frequentWords;
        vector<int> usedIdx;
        for (int i = 0; i < numFrequent; i++) {
            int idx;
            bool dup;
            do {
                idx = rand() % normalWords.size();
                dup = false;
                for (int u : usedIdx) if (u == idx) { dup = true; break; }
            } while (dup);
            usedIdx.push_back(idx);
            frequentWords.push_back(normalWords[idx]);
        }

        vector<string> textTokens;

        for (const auto& fw : frequentWords) {
            int rep = 3 + rand() % 5;
            for (int r = 0; r < rep; r++) {
                textTokens.push_back(fw);
            }
        }

        int remaining = (50 + rand() % 51) - (int)textTokens.size();
        if (remaining < 0) remaining = 0;
        for (int i = 0; i < remaining; i++) {
            int pool = rand() % 10;
            if (pool < 6) {
                textTokens.push_back(allWords[rand() % allWords.size()]);
            } else if (pool < 8) {
                textTokens.push_back(punctWords[rand() % punctWords.size()]);
            } else {
                textTokens.push_back(edgeCaseTokens[rand() % edgeCaseTokens.size()]);
            }
        }

        for (int i = (int)textTokens.size() - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(textTokens[i], textTokens[j]);
        }

        int col = 0;
        for (int i = 0; i < (int)textTokens.size(); i++) {
            if (i > 0 && rand() % 100 < 8) {
                out << "\n\n";
                col = 0;
            } else if (i > 0 && rand() % 100 < 10) {
                out << "\t";
            } else if (i > 0) {
                out << " ";
                col++;
                if (col >= 8 + rand() % 5) {
                    out << "\n";
                    col = 0;
                }
            }

            string token = textTokens[i];
            bool isEdge = false;
            for (auto& e : edgeCaseTokens) if (e == token) { isEdge = true; break; }

            if (!isEdge) {
                int caseType = rand() % 4;
                if (caseType == 0) {
                    for (char& c : token)
                        c = toupper((unsigned char)c);
                } else if (caseType == 1) {
                    if (!token.empty() && isalpha((unsigned char)token[0]))
                        token[0] = toupper((unsigned char)token[0]);
                } else if (caseType == 2) {
                    token = randomCase(token);
                }
                token = addPunctuation(token);
            }

            out << token;
        }

        out << "\n###END###\n";

        vector<string> stopWordPool;
        for (const auto& tk : textTokens) {
            bool isEdge = false;
            for (auto& e : edgeCaseTokens) if (e == tk) { isEdge = true; break; }
            if (!isEdge) {
                string clean;
                for (char c : tk) {
                    if (c != ';' && c != '?' && c != '!' && c != '.' &&
                        c != ',' && c != '\'' && c != ':') {
                        clean += tolower((unsigned char)c);
                    }
                }
                if (!clean.empty()) stopWordPool.push_back(clean);
            }
        }

        sort(stopWordPool.begin(), stopWordPool.end());
        stopWordPool.erase(unique(stopWordPool.begin(), stopWordPool.end()), stopWordPool.end());

        int numStop = 5 + rand() % 11;
        vector<string> stopWords;

        for (int i = 0; i < numStop && !stopWordPool.empty(); i++) {
            int r = rand() % 100;
            string sw;
            if (r < 70 && !stopWordPool.empty()) {
                sw = stopWordPool[rand() % stopWordPool.size()];
            } else {
                sw = normalWords[rand() % normalWords.size()];
            }
            bool dup = false;
            for (auto& s : stopWords) if (s == sw) { dup = true; break; }
            if (!dup) stopWords.push_back(sw);
        }

        for (int i = 0; i < (int)stopWords.size(); i++) {
            if (i > 0) out << " ";
            out << stopWords[i];
        }
        out << "\n";

        out.close();
    }

    cout << "Generated 60 test cases: input1.txt ... input60.txt\n";
    return 0;
}