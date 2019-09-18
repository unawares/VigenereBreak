#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <cmath>
#include <queue>

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


void recursiveSearch(
    map<char, TrieNode*> wordLetters,
    map<char, TrieNode*> cipherLetters,
    string key,
    vector<string>& keys
) {
    string letters =  "abcdefghijklmnopqrstuvwxyz";
    keys.push_back(key);
    for (char ch1 : letters) {
        if (wordLetters[ch1] != NULL) {
            for (char ch2 : letters) {
                if (cipherLetters[ch2] != NULL) {
                    char ch = (26 + (ch2 - ch1)) % 26 + 'a';
                    recursiveSearch(
                        wordLetters[ch1]->next,
                        cipherLetters[ch2]->next,
                        key + ch,
                        keys
                    );
                }
            }
        }
    }
}


int main() {
    int SIZE = 4;

    vector<string> words = processText(readFile("text.txt"));
    vector<string> cipherWords = processText(readFile("in2.txt"));

    Trie wordsTrie = Trie();
    Trie cipherWordsTrie = Trie();

    for (string word : words) {
        wordsTrie.add(word);
    }

    map<string, int> offsets;
    map<string, bool> counter;
    map<int, int[]> levels;

    int offset = 0;
    for (string word : cipherWords) {
        cipherWordsTrie.add(word);
        offsets[word] = offset % SIZE;
        offset += word.size();
    }

    map<char, TrieNode*> wordLetters = wordsTrie.next("");
    map<char, TrieNode*> cipherLetters = cipherWordsTrie.next("");

    vector<string> keys;
    recursiveSearch(wordLetters, cipherLetters, "", keys);

    vector<string> unique;
    for (string key : keys) {
        if (!counter[key]) {
            unique.push_back(key);
            counter[key] = true;
        }
    }


    ofstream outfile;
    outfile.open("keys.txt");
    for (string key : unique) {
        outfile << key << '\n';
    }
    outfile.close();

    cout << unique.size() << endl;

    return 0;
}