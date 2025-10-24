#include <bits/stdc++.h>
using namespace std;

static std::mt19937 rng(1077);

using Rules = vector<pair<string,string>>;

vector<char> alphabet = {'a','b','c'};

Rules T = {
    {"aabc","bbaa"},
    {"b","ccaa"},
    {"bc","a"},
    {"aac",""}
};

pair<string,string> make_rule(int n){
    string lhs(2*n, 'b');
    lhs += "aa";
    string rhs(n+2, 'a');
    return {lhs, rhs};
}

unordered_set<string> neighbors_fwd(const string &word, const Rules &rules){
    unordered_set<string> outs;
    for (const auto &r : rules){
        const string &lhs = r.first;
        const string &rhs = r.second;
        if (lhs.empty()) continue;
        size_t start = 0;
        while (true){
            size_t i = word.find(lhs, start);
            if (i == string::npos) break;
            string next = word.substr(0,i) + rhs + word.substr(i + lhs.size());
            outs.insert(std::move(next));
            start = i + 1;
        }
    }
    outs.erase(word);
    return outs;
}

string rand_word(int min_len=10, int max_len=20){
    uniform_int_distribution<int> len_dist(min_len, max_len);
    int L = len_dist(rng);
    string s;
    s.reserve(L);
    uniform_int_distribution<int> alph_dist(0, (int)alphabet.size()-1);
    for (int i=0;i<L;++i) s.push_back(alphabet[alph_dist(rng)]);
    return s;
}

vector<string> rand_chain(const string &start_word, const Rules &rules, int min_steps=1, int max_steps=20){
    string w = start_word;
    vector<string> chain;
    chain.push_back(w);
    uniform_int_distribution<int> step_dist(min_steps, max_steps);
    int steps = step_dist(rng);
    for (int k=0;k<steps;++k){
        auto neigh_set = neighbors_fwd(w, rules);
        if (neigh_set.empty()) break;
        vector<string> neigh_vec(neigh_set.begin(), neigh_set.end());
        uniform_int_distribution<int> pick(0, (int)neigh_vec.size()-1);
        w = neigh_vec[pick(rng)];
        chain.push_back(w);
    }
    return chain;
}

pair<bool, vector<string>> bfs(const string &src, 
                               const string &dst, 
                               const Rules &rules, 
                               int max_depth=40, 
                               size_t max_nodes=500000) {

    if (src == dst) return {true, {src}};
    deque<string> q;
    q.push_back(src);
    unordered_map<string, string> parent;
    unordered_map<string, int> depth;
    unordered_set<string> visited;

    parent[src] = string();
    depth[src] = 0;
    visited.insert(src);

    size_t expanded = 0;

    while (!q.empty() && expanded < max_nodes){
        string u = move(q.front()); q.pop_front();
        int d = depth[u];
        if (d >= max_depth) continue;

        auto neighs = neighbors_fwd(u, rules);
        for (const auto &v : neighs){
            ++expanded;
            if (visited.find(v) != visited.end()) continue;
            visited.insert(v);
            parent[v] = u;
            depth[v] = d + 1;
            if (v == dst){
                vector<string> path;
                string cur = v;
                while (!cur.empty() || parent.find(cur) != parent.end()){
                    path.push_back(cur);
                    auto it = parent.find(cur);
                    if (it == parent.end()) break;
                    cur = it->second;
                    if (cur.empty()) break;
                }
                if (path.empty() || path.back() != src) path.push_back(src);
                reverse(path.begin(), path.end());
                return {true, path};
            }
            q.push_back(v);
            if (expanded >= max_nodes) break;
        }
    }

    return {false, {}};
}

struct FailExample {
    string w0;
    string w1;
    vector<string> chain;
    vector<string> path;
};

tuple<int,int,vector<FailExample>> fuzz(int num_trials=100, int show_examples=10){
    cout << "===== Fuzz testing =====\n";
    Rules T_prime = T;
    for (int n=1; n<=5; ++n) T_prime.push_back(make_rule(n));

    int successes = 0;
    int failures = 0;
    vector<FailExample> examples;

    for (int t=0; t<num_trials; ++t){
        string w0 = rand_word(10, 30);
        auto chain = rand_chain(w0, T, 1, 10);
        string w1 = chain.back();
        auto res = bfs(w0, w1, T_prime, 35, 1000000);
        bool ok = res.first;
        auto path = res.second;
        if (ok) ++successes;
        else {
            ++failures;
            if ((int)examples.size() < show_examples){
                examples.push_back({w0, w1, chain, path});
            }
        }
    }
    return {successes, failures, examples};
}

struct Invariants {
    int I1;
    int I2;
};

int count_substr(const string &s, const string &sub){
    if (sub.empty()) return 0;
    int cnt = 0;
    for (size_t i=0;i+sub.size()<=s.size();++i)
        if (s.compare(i, sub.size(), sub)==0)
            ++cnt;
    return cnt;
}

Invariants compute_invariants(const string &s){
    int na=0, nb=0, nc=0;
    for (char ch : s){
        if (ch=='a') ++na;
        else if (ch=='b') ++nb;
        else if (ch=='c') ++nc;
    }
    Invariants inv;
    inv.I1 = (2*na + nb + nc) % 5;
    inv.I2 = (2*na + 6*nb + 6*nc) % 10;
    return inv;
}

void metamorphic_test(int num_trials, int max_steps){
    cout << "===== Metamorphic testing =====\n";
    Rules T_prime = T;
    for (int n=1; n<=5; ++n) T_prime.push_back(make_rule(n));

    int ok_count = 0, fail_count = 0;

    for (int t=0;t<num_trials;++t){
        string w0 = rand_word(10,25);
        auto chain = rand_chain(w0, T_prime, 3, max_steps);
        bool invariants_ok = true;

        Invariants prev = compute_invariants(chain.front());
        for (size_t i=1;i<chain.size();++i){
            Invariants cur = compute_invariants(chain[i]);

            if (cur.I1 != prev.I1 || cur.I2 != prev.I2){
                invariants_ok = false;
                cout << "\nInvarinat failure #" << t+1 << "\n";
                cout << "w["<<i-1<<"] = " << chain[i-1] << "\n";
                cout << "w["<<i<<"] = " << chain[i] << "\n";
                cout << "I1: " << prev.I1 << " -> " << cur.I1
                     << ", I2: " << prev.I2 << " -> " << cur.I2 << "\n";
                break;
            }
            prev = cur;
        }

        if (invariants_ok) ++ok_count;
        else ++fail_count;
    }

    cout << "Trials: " << (ok_count + fail_count)
         << ", Successes: " << ok_count
         << ", Failures: " << fail_count << "\n\n";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto [successes, failures, examples] = fuzz(25, 10);
    cout << "Trials: " << (successes + failures)
         << ", Successes: " << successes
         << ", Failures: " << failures << "\n\n";

    metamorphic_test(25, 10);

    return 0;
}
