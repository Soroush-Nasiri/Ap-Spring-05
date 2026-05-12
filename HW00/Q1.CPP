#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct WordCount {
    string word;
    int count;
};

bool isPunctuation(char c) {
    string punct = "',.!?;:\"";
    for (int i = 0; i < punct.length(); i++) {
        if (c == punct[i]) return true;
    }
    return false;
}

string sanitizeWord(string word) {
    for (int i = 0; i < word.length(); i++) {
        if (word[i] >= 'A' && word[i] <= 'Z') {
            word[i] = word[i] + ('a' - 'A');
        }
    }
    
    int start = 0;
    while (start < (int)word.length() && isPunctuation(word[start])) {
        start++;
    }
    
    int end = (int)word.length() - 1;
    while (end >= start && isPunctuation(word[end])) {
        end--;
    }
    
    if (start > end) return "";
    
    string result = "";
    for (int i = start; i <= end; i++) {
        result += word[i];
    }

    bool isNumeric = true;
    int dotCount = 0;
    bool hasDigit = false;

    for (int i = 0; i < result.length(); i++) {
        if (result[i] >= '0' && result[i] <= '9') {
            hasDigit = true;
        } else if (result[i] == '.') {
            dotCount++;
        } else {
            isNumeric = false;
            break;
        }
    }

    if (dotCount > 1) isNumeric = false;
    if (!hasDigit) isNumeric = false;

    if (isNumeric) return "";
    
    return result;
}

bool isStopWord(string word, vector<string> stopWords) {
    for (int i = 0; i < stopWords.size(); i++) {
        if (word == stopWords[i]) return true;
    }
    return false;
}

vector<WordCount> updateWordCount(vector<WordCount> frequencies, string word) {
    for (int i = 0; i < frequencies.size(); i++) {
        if (frequencies[i].word == word) {
            frequencies[i].count++;
            return frequencies;
        }
    }
    WordCount wc;
    wc.word = word;
    wc.count = 1;
    frequencies.push_back(wc);
    
    return frequencies;
}

vector<WordCount> sortWordCounts(vector<WordCount> frequencies) {
    int n = frequencies.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool swapNeeded = false;
            if (frequencies[j].count < frequencies[j+1].count) {
                swapNeeded = true;
            } else if (frequencies[j].count == frequencies[j+1].count) {
                if (frequencies[j].word > frequencies[j+1].word) {
                    swapNeeded = true;
                }
            }
            if (swapNeeded) {
                WordCount temp = frequencies[j];
                frequencies[j] = frequencies[j+1];
                frequencies[j+1] = temp;
            }
        }
    }
    
    return frequencies;
}

int main() {
    vector<string> rawTokens;
    vector<string> stopWords;
    vector<WordCount> frequencies;
    string input;
    
    while (cin >> input && input != "###END###") {
        rawTokens.push_back(input);
    }
    
    string lineAfterEnd;
    getline(cin, lineAfterEnd); 

    string stopWordsLine;
    if (getline(cin, stopWordsLine)) {
        string currentWord = "";
        for (int i = 0; i <= (int)stopWordsLine.length(); i++) {
            if (i == stopWordsLine.length() || stopWordsLine[i] == ' ') {
                if (currentWord != "") {
                    string cleanSW = sanitizeWord(currentWord);
                    if (cleanSW != "") {
                        stopWords.push_back(cleanSW);
                    }
                    currentWord = "";
                }
            } else {
                currentWord += stopWordsLine[i];
            }
        }
    }
    
    int validWordsCount = 0;
    for (int i = 0; i < (int)rawTokens.size(); i++) {
        string clean = sanitizeWord(rawTokens[i]);
        if (clean != "" && !isStopWord(clean, stopWords)) {
            validWordsCount++;
            frequencies = updateWordCount(frequencies, clean);
        }
    }
    
    frequencies = sortWordCounts(frequencies);
    
    cout << "Total words original: " << rawTokens.size() << endl;
    cout << "Total words after filter: " << validWordsCount << endl;
    cout << "Word Frequencies:" << endl;
    for (int i = 0; i < (int)frequencies.size(); i++) {
        cout << frequencies[i].word << ": " << frequencies[i].count << endl;
    }
    
    return 0;
}