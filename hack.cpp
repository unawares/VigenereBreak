#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;


class TrieNode {

    public:
    map<char, TrieNode*> next;

    TrieNode () : next(map<char, TrieNode*>()) {}

};


class Trie {

    private:
    TrieNode trie;

    public:
    Trie () : trie(TrieNode()) {}
    
    void add(string word) {
        TrieNode* trie = &(this->trie);
        for (char& ch : word) {
            if (trie->next[ch] == NULL) {
                trie->next[ch] = new TrieNode();
            }
            trie = trie->next[ch];
        }
    }

    bool check(string word) {
        TrieNode* trie = &(this->trie);
        for (char& ch : word) {
            if (trie->next[ch] != NULL) {
                trie = trie->next[ch];
            } else {
                return false;
            }
        }
        return true;
    }

    int common(string word) {
        int counter = 0;
        TrieNode* trie = &(this->trie);
        for (char& ch : word) {
            if (trie->next[ch] != NULL) {
                trie = trie->next[ch];
                counter += 1;
            } else {
                break;
            }
        }
        return counter;
    }

    map<char, TrieNode*> next(string word) {
        TrieNode* trie = &(this->trie);
        for (char& ch : word) {
            if (trie->next[ch] != NULL) {
                trie = trie->next[ch];
            } else {
                break;
            }
        }
        return trie->next;
    }

    ~Trie () {}

};


string readFile(string filename) {
    string res = "";
    ifstream file(filename);
    if (file.is_open()) {
        while (true) {
            string line;
            getline(file, line);
            res += line + '\n';
            if( file.eof() ) break;
        }
        file.close();
    }
    return res;
}


vector<string> processText(string text) {
    vector<string> res;
    string word = "";
    for (char& ch : text) {
        if (ch >= 'a' && ch <= 'z') {
            word += ch;
        } else if (ch >= 'A' && ch <= 'Z') {
            word += (ch - 'A' + 'a');
        } else if (word.size() > 0) {
            res.push_back(word);
            word = "";
        }
    }
    if (word.size() > 0) {
        res.push_back(word);
    }
    return res;
}


// string nextKey(string key) {
//     int k = 1;
//     string res = "";
//     for (char& ch : key) {
//         int s = ch - 'a' + k;
//         if (s > ('z' - 'a')) {
//             s = 0;
//             k = 1;
//         } else {
//             k = 0;
//         }
//         res += s + 'a';
//     }
//     return res;
// }


int maxCommon(Trie& trie, string cipher, string key) {
    int counter = 0;
    for (int i = 0; i < cipher.size(); i++) {
        string maybe = cipher.substr(i, key.size());
        for (int j = 0; j < key.size(); j++) {
            maybe[j] = (26 + maybe[j] - key[(j + i) % key.size()]) % 26 + 'a';
        }
        counter += trie.common(maybe);
    }
    return counter;
}

string decrypt(string cipher, string key) {
    string res = "";
    for (int i = 0; i < cipher.size(); i++) {
        res += (26 + cipher[i] - key[i % key.size()]) % 26 + 'a';
    }
    return res;
}


int main() {
    
    Trie trie = Trie();
    vector<string> words = processText(readFile("text.txt"));
    for (string word : words) {
        trie.add(word);
    }
    
    vector<string> cipherWords = processText(readFile("in2.txt"));
    string cipher = "";
    for (string word : cipherWords) {
        cipher += word;
    }

    int maxCount = 0;
    string selKey = "";

    vector<string> keys = processText(readFile("keys.txt"));

    for (string key : keys) {
        int s = maxCommon(trie, cipher, key);
        if (s >= maxCount) {
            cout << key << ": " << decrypt(cipher, key) << '\n' << endl;
            maxCount = s;
            selKey = key;
        }
    }

    return 0;
}