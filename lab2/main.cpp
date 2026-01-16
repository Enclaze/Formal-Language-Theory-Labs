#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <random>
#include <regex>
#include <utility>
#include <queue>
#include <unordered_set>

using namespace std;

const vector<string> ALPHABET = {"a", "b", "c"};

// ДКА
const map<string, vector<pair<string, string>>> DFA = {
    {"0", {{"17", "a"}, {"0", "b"}, {"29", "c"}}},
    {"1", {{"10", "a"}, {"11", "c"}}},
    {"2", {{"0", "a"}, {"3", "c"}}},
    {"3", {{"15", "a"}, {"16", "b"}, {"8", "c"}}},
    {"4", {{"5", "a"}, {"11", "c"}}},
    {"5", {{"12", "a"}, {"13", "b"}, {"8", "c"}}},
    {"6", {{"7", "a"}, {"0", "b"}, {"8", "c"}}},
    {"7", {{"1", "a"}, {"22", "b"}}},
    {"8", {{"0", "a"}, {"9", "c"}}},
    {"9", {{"10", "a"}, {"0", "b"}, {"8", "c"}}},
    {"10", {{"1", "a"}, {"4", "b"}}},
    {"11", {{"1", "c"}}},
    {"12", {{"33", "a"}, {"22", "b"}, {"11", "c"}}},
    {"13", {{"14", "a"}, {"0", "b"}, {"2", "c"}}},
    {"14", {{"12", "a"}, {"31", "b"}, {"8", "c"}}},
    {"15", {{"4", "a"}, {"22", "b"}}},
    {"16", {{"17", "a"}, {"0", "b"}, {"2", "c"}}},
    {"17", {{"18", "b"}}},
    {"18", {{"19", "a"}}},
    {"19", {{"20", "c"}}},
    {"20", {{"0", "b"}, {"21", "c"}}},
    {"21", {{"0", "a"}}},
    {"22", {{"23", "a"}, {"11", "c"}}},
    {"23", {{"12", "a"}, {"13", "b"}, {"24", "c"}}},
    {"24", {{"0", "a"}, {"0", "b"}, {"25", "c"}}},
    {"25", {{"26", "a"}, {"0", "b"}, {"8", "c"}}},
    {"26", {{"27", "a"}, {"28", "b"}, {"29", "c"}}},
    {"27", {{"10", "a"}, {"18", "b"}, {"11", "c"}}},
    {"28", {{"14", "a"}, {"0", "b"}, {"8", "c"}}},
    {"29", {{"0", "a"}, {"30", "c"}}},
    {"30", {{"0", "b"}, {"29", "c"}}},
    {"31", {{"32", "a"}, {"0", "b"}, {"2", "c"}}},
    {"32", {{"12", "a"}, {"31", "b"}, {"24", "c"}}},
    {"33", {{"5", "a"}, {"4", "b"}, {"11", "c"}}}
};

// НКА, а также первая часть ПКА
const map<string, vector<pair<string, string>>> AFA1 = {
    {"0", {{"1", "a"}, {"2", "a"}, {"3", "a"}, {"4", "a"}, {"5", "b"}, {"6", "c"}, {"7", "c"}, {"8", "c"}}},
    {"1", {{"9", "a"}}},
    {"2", {{"9", "b"}}},
    {"3", {{"10", "b"}}},
    {"4", {{"11", "b"}}},
    {"5", {{"3", "a"}, {"5", "b"}, {"7", "c"}, {"8", "c"}}},
    {"6", {{"9", "c"}}},
    {"7", {{"12", "c"}}},
    {"8", {{"5", "a"}}},
    {"9", {{"1", "a"}, {"2", "a"}, {"4", "a"}, {"6", "c"}}},
    {"10", {{"13", "a"}}},
    {"11", {{"14", "a"}}},
    {"12", {{"5", "b"}, {"7", "c"}, {"8", "c"}}},
    {"13", {{"15", "c"}}},
    {"14", {{"1", "a"}, {"2", "a"}, {"3", "a"}, {"4", "a"}, {"5", "b"}, {"6", "c"}, {"7", "c"}, {"8", "c"}, {"16", "a"}, {"17", "b"}}},
    {"15", {{"5", "b"}, {"8", "c"}}},
    {"16", {{"11", "a"}}},
    {"17", {{"18", "c"}}},
    {"18", {{"14", "c"}}}
};

// Вторая часть ПКА
const map<string, vector<pair<string, string>>> AFA2 = {
    {"I1_0", {{"I1_0", "a"}, {"I1_0", "c"}, {"I1_1", "b"}}},
    {"I1_1", {{"I1_0", "a"}, {"I1_1", "b"}, {"I1_2", "c"}}},
    {"I1_2", {{"I1_0", "a"}, {"I1_0", "c"}, {"I1_3", "b"}}},
    {"I1_3", {{"I1_3", "a"}, {"I1_3", "b"}, {"I1_3", "c"}}}
};

// Третья часть ПКА
const map<string, vector<pair<string, string>>> AFA3 = {
    {"I2_0", {{"I2_a", "a"}, {"I2_b", "b"}, {"I2_0", "c"}}},
    {"I2_a", {{"I2_a", "a"}, {"I2_b", "b"}, {"I2_ac", "c"}}},
    {"I2_b", {{"I2_a", "a"}, {"I2_b", "b"}, {"I2_bc", "c"}}},
    {"I2_ac", {{"I2_a", "a"}, {"I2_b", "b"}, {"I2_0", "c"}}},
    {"I2_bc", {{"I2_a", "a"}, {"I2_b", "b"}, {"I2_0", "c"}}}
};

bool word_in_automata(const string& word, 
                     const map<string, vector<pair<string, string>>>& automaton, 
                     const set<string>& final_states, 
                     const string& start_state) {
    vector<string> current_states = {start_state};
    
    for (char symbol : word) {
        string s(1, symbol);
        vector<string> new_current_states;
        
        for (const string& state : current_states) {
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
    
    for (const string& state : current_states) {
        if (final_states.count(state)) {
            return true;
        }
    }
    return false;
}

bool word_in_intersection(const string& word,
                               const map<string, vector<pair<string, string>>>& automaton1,
                               const set<string>& final1,
                               const string& start1,
                               const map<string, vector<pair<string, string>>>& automaton2,
                               const set<string>& final2,
                               const string& start2,
                               const map<string, vector<pair<string, string>>>& automaton3,
                               const set<string>& final3,
                               const string& start3) {

    vector<tuple<string, string, string>> current_triples = {make_tuple(start1, start2, start3)};
    
    for (char symbol : word) {
        string s(1, symbol);
        vector<tuple<string, string, string>> new_triples;
        
        for (const auto& triple : current_triples) {
            string state1 = get<0>(triple);
            string state2 = get<1>(triple);
            string state3 = get<2>(triple);
            
            vector<string> next_states1, next_states2, next_states3;
            
            auto it1 = automaton1.find(state1);
            if (it1 != automaton1.end()) {
                for (const auto& trans : it1->second) {
                    if (trans.second == s) {
                        next_states1.push_back(trans.first);
                    }
                }
            }
            
            auto it2 = automaton2.find(state2);
            if (it2 != automaton2.end()) {
                for (const auto& trans : it2->second) {
                    if (trans.second == s) {
                        next_states2.push_back(trans.first);
                    }
                }
            }
            
            auto it3 = automaton3.find(state3);
            if (it3 != automaton3.end()) {
                for (const auto& trans : it3->second) {
                    if (trans.second == s) {
                        next_states3.push_back(trans.first);
                    }
                }
            }
            
            if (next_states1.empty() || next_states2.empty() || next_states3.empty()) {
                continue;
            }
            
            for (const string& ns1 : next_states1) {
                for (const string& ns2 : next_states2) {
                    for (const string& ns3 : next_states3) {
                        new_triples.push_back(make_tuple(ns1, ns2, ns3));
                    }
                }
            }
        }
        
        current_triples = move(new_triples);
        if (current_triples.empty()) return false;
    }
    
    for (const auto& triple : current_triples) {
        string state1 = get<0>(triple);
        string state2 = get<1>(triple);
        string state3 = get<2>(triple);
        
        if (final1.count(state1) && final2.count(state2) && final3.count(state3)) {
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
         const map<string, vector<pair<string, string>>>& dfa,
         const map<string, vector<pair<string, string>>>& nfa,
         const map<string, vector<pair<string, string>>>& afa1,
         const map<string, vector<pair<string, string>>>& afa2,
         const map<string, vector<pair<string, string>>>& afa3,
         int num_iters = 100) {
    
    set<string> dfa_finals = {"0", "3", "5", "6", "13", "14", "16", "23", "26", "28", "31", "32"};
    set<string> nfa_finals = {"0", "5", "14"};
    set<string> afa1_finals = {"0", "5", "14"};
    set<string> afa2_finals = {"I1_0", "I1_1", "I1_2"};
    set<string> afa3_finals = {"I2_0", "I2_a", "I2_b"};
    
    string dfa_start = "6";
    string nfa_start = "0";
    string afa1_start = "0";
    string afa2_start = "I1_0";
    string afa3_start = "I2_0";
    
    for (int i = 0; i < num_iters; ++i) {
        string word = random_word();
        
        bool in_dfa = word_in_automata(word, dfa, dfa_finals, dfa_start);
        bool in_nfa = word_in_automata(word, nfa, nfa_finals, nfa_start);
        bool in_afa = word_in_intersection(word, afa1, afa1_finals, afa1_start,
                                                afa2, afa2_finals, afa2_start,
                                                afa3, afa3_finals, afa3_start);
        bool in_regex = regex_match(word, regex_pattern);
        
        if (!(in_dfa == in_nfa && in_nfa == in_afa && in_afa == in_regex)) {
            cout << "Не работает на примере: " << word << endl;
            cout << "Результаты: DFA=" << in_dfa << " NFA=" << in_nfa 
                 << " AFA=" << in_afa << " Regex=" << in_regex << endl;
            return;
        }
    }
    
    for (int i = 0; i < num_iters; ++i) {
        string word = random_regex_word();
        
        bool in_dfa = word_in_automata(word, dfa, dfa_finals, dfa_start);
        bool in_nfa = word_in_automata(word, nfa, nfa_finals, nfa_start);
        bool in_afa = word_in_intersection(word, afa1, afa1_finals, afa1_start,
                                                afa2, afa2_finals, afa2_start,
                                                afa3, afa3_finals, afa3_start);
        bool in_regex = regex_match(word, regex_pattern);
        
        if (!(in_dfa == in_nfa && in_nfa == in_afa && in_afa == in_regex)) {
            cout << "Doesn't work on a word: " << word << endl;
            cout << "Results: DFA=" << in_dfa << " NFA=" << in_nfa 
                 << " AFA=" << in_afa << " Regex=" << in_regex << endl;
            return;
        }
    }
    
    cout << "Fuzzing successful" << endl;
}

int main() {
    regex pattern("^((aa|ab|cc)*aba(aaa|bcc)*)*((abac|(cc)*)(b|ca))*$");
    fuzz(pattern, DFA, AFA1, AFA1, AFA2, AFA3);
    return 0;
}
