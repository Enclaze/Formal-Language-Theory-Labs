#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <random>
#include <regex>
#include <utility>

using namespace std;

const vector<string> ALPHABET = {"a", "b", "c"};

const map<int, vector<pair<int, string>>> DFA = {
    {0, {{17, "a"}, {0, "b"}, {29, "c"}}},
    {1, {{10, "a"}, {11, "c"}}},
    {2, {{0, "a"}, {3, "c"}}},
    {3, {{15, "a"}, {16, "b"}, {8, "c"}}},
    {4, {{5, "a"}, {11, "c"}}},
    {5, {{12, "a"}, {13, "b"}, {8, "c"}}},
    {6, {{7, "a"}, {0, "b"}, {8, "c"}}},
    {7, {{1, "a"}, {22, "b"}}},
    {8, {{0, "a"}, {9, "c"}}},
    {9, {{10, "a"}, {0, "b"}, {8, "c"}}},
    {10, {{1, "a"}, {4, "b"}}},
    {11, {{1, "c"}}},
    {12, {{33, "a"}, {22, "b"}, {11, "c"}}},
    {13, {{14, "a"}, {0, "b"}, {2, "c"}}},
    {14, {{12, "a"}, {31, "b"}, {8, "c"}}},
    {15, {{4, "a"}, {22, "b"}}},
    {16, {{17, "a"}, {0, "b"}, {2, "c"}}},
    {17, {{18, "b"}}},
    {18, {{19, "a"}}},
    {19, {{20, "c"}}},
    {20, {{0, "b"}, {21, "c"}}},
    {21, {{0, "a"}}},
    {22, {{23, "a"}, {11, "c"}}},
    {23, {{12, "a"}, {13, "b"}, {24, "c"}}},
    {24, {{0, "a"}, {0, "b"}, {25, "c"}}},
    {25, {{26, "a"}, {0, "b"}, {8, "c"}}},
    {26, {{27, "a"}, {28, "b"}, {29, "c"}}},
    {27, {{10, "a"}, {18, "b"}, {11, "c"}}},
    {28, {{14, "a"}, {0, "b"}, {8, "c"}}},
    {29, {{0, "a"}, {30, "c"}}},
    {30, {{0, "b"}, {29, "c"}}},
    {31, {{32, "a"}, {0, "b"}, {2, "c"}}},
    {32, {{12, "a"}, {31, "b"}, {24, "c"}}},
    {33, {{5, "a"}, {4, "b"}, {11, "c"}}}
};

// НКА из файла NFA.txt
const map<int, vector<pair<int, string>>> NFA = {
    {0, {{1, "a"}, {2, "a"}, {3, "a"}, {4, "a"}, {5, "b"}, {6, "c"}, {7, "c"}, {8, "c"}}},
    {1, {{9, "a"}}},
    {2, {{9, "b"}}},
    {3, {{10, "b"}}},
    {4, {{11, "b"}}},
    {5, {{3, "a"}, {5, "b"}, {7, "c"}, {8, "c"}}},
    {6, {{9, "c"}}},
    {7, {{12, "c"}}},
    {8, {{5, "a"}}},
    {9, {{1, "a"}, {2, "a"}, {4, "a"}, {6, "c"}}},
    {10, {{13, "a"}}},
    {11, {{14, "a"}}},
    {12, {{5, "b"}, {7, "c"}, {8, "c"}}},
    {13, {{15, "c"}}},
    {14, {{1, "a"}, {2, "a"}, {3, "a"}, {4, "a"}, {5, "b"}, {6, "c"}, {7, "c"}, {8, "c"}, {16, "a"}, {17, "b"}}},
    {15, {{5, "b"}, {8, "c"}}},
    {16, {{11, "a"}}},
    {17, {{18, "c"}}},
    {18, {{14, "c"}}}
};

bool word_in_automata(const string& word, const map<int, vector<pair<int, string>>>& automaton, const set<int>& final_states, int start_state) {
    vector<int> current_states = {start_state};
    
    for (char symbol : word) {
        string s(1, symbol);
        vector<int> new_current_states;
        
        for (int state : current_states) {
            auto it = automaton.find(state);
            if (it != automaton.end()) {
                for (const auto& transition : it->second) {
                    if (transition.second == s) {
                        new_current_states.push_back(transition.first);
                    }
                }
            }
        }
        
        current_states = move(new_current_states);
        if (current_states.empty()) break;
    }
    
    for (int state : current_states) {
        if (final_states.count(state)) {
            return true;
        }
    }
    return false;
}

string random_word(pair<int, int> word_len_range = {3, 20}, const vector<string>& alphabet = ALPHABET) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> len_dist(word_len_range.first, word_len_range.second);
    uniform_int_distribution<> alpha_dist(0, alphabet.size() - 1);
    
    int word_len = len_dist(gen);
    string rand_word;
    
    for (int i = 0; i < word_len; ++i) {
        int idx = alpha_dist(gen);
        rand_word += alphabet[idx];
    }
    
    return rand_word;
}

string random_regex_word(int max_iter = 10) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> iter_dist(0, max_iter);
    uniform_int_distribution<> var_dist(0, 1);
    uniform_int_distribution<> choice_dist(0, 2);
    
    string word;
    
    // ((aa|ab|cc)*aba(aaa|bcc)*)*
    int num_iters = iter_dist(gen);
    for (int i = 0; i < num_iters; ++i) {
        // (aa|ab|cc)*
        int inner_iters = iter_dist(gen);
        for (int j = 0; j < inner_iters; ++j) {
            vector<string> variants = {"aa", "ab", "cc"};
            word += variants[choice_dist(gen)];
        }
        
        // aba
        word += "aba";
        
        // (aaa|bcc)*
        inner_iters = iter_dist(gen);
        for (int j = 0; j < inner_iters; ++j) {
            vector<string> variants = {"aaa", "bcc"};
            word += variants[var_dist(gen)];
        }
    }
    
    // ((abac|(cc)*)(b|ca))*
    num_iters = iter_dist(gen);
    for (int i = 0; i < num_iters; ++i) {
        // (abac|(cc)*)
        if (var_dist(gen) == 0) {
            word += "abac";
        } else {
            int cc_iters = iter_dist(gen);
            for (int j = 0; j < cc_iters; ++j) {
                word += "cc";
            }
        }
        
        // (b|ca)
        vector<string> variants = {"b", "ca"};
        word += variants[var_dist(gen)];
    }
    
    return word;
}

void fuzz(const regex& regex_pattern,
         const map<int, vector<pair<int, string>>>& dfa,
         const map<int, vector<pair<int, string>>>& nfa,
         int num_iters = 1000) {
    
    set<int> dfa_finals = {0, 3, 5, 6, 13, 14, 16, 23, 26, 28, 31, 32};
    set<int> nfa_finals = {0, 5, 14};
    
    int dfa_start = 6;
    int nfa_start = 0;
    
    for (int i = 0; i < num_iters; ++i) {
        string word = random_word();
        
        bool in_dfa = word_in_automata(word, dfa, dfa_finals, dfa_start);
        bool in_nfa = word_in_automata(word, nfa, nfa_finals, nfa_start);
        bool in_regex = regex_match(word, regex_pattern);
        
        if (!(in_dfa == in_nfa && in_nfa == in_regex)) {
            cout << "Doesn't work on an example: " << word << endl;
            cout << "Results: DFA=" << in_dfa << " NFA=" << in_nfa 
                 << " Regex=" << in_regex << endl;
            return;
        }
    }
    
    for (int i = 0; i < num_iters; ++i) {
        string word = random_regex_word();
        
        bool in_dfa = word_in_automata(word, dfa, dfa_finals, dfa_start);
        bool in_nfa = word_in_automata(word, nfa, nfa_finals, nfa_start);
        bool in_regex = regex_match(word, regex_pattern);
        
        if (!(in_dfa == in_nfa && in_nfa == in_regex)) {
            cout << "Doesn't work on an example: " << word << endl;
            cout << "Results: DFA=" << in_dfa << " NFA=" << in_nfa 
                 << " Regex=" << in_regex << endl;
            return;
        }
    }
    
    cout << "Testing successful" << endl;
}

int main() {
    regex pattern("((aa|ab|cc)*aba(aaa|bcc)*)*((abac|(cc)*)(b|ca))*");
    fuzz(pattern, DFA, NFA);
    return 0;
}