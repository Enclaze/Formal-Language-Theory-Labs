#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Generate segments
vector<string> gen_segments(size_t max_len) {
    vector<string> segs;
    if (max_len == 0) return segs;
    
    string x = "a";
    string y = "a";
    
    while (y.length() <= max_len) {
        segs.push_back(y);
        
        // x = x + "a" + x + "b"
        string next_x = x + "a" + x + "b";
        
        // y = y + next_x
        string next_y = y + next_x;
        
        x = next_x;
        y = next_y;
    }
    
    return segs;
}

// Recursive check for segment splitting (naive approach)
bool go_from(size_t pos, const string& x, const vector<string>& segs) {
    if (pos == x.length()) return true;
    
    for (const auto& s : segs) {
        size_t seg_len = s.length();
        if (pos + seg_len <= x.length() && 
            x.substr(pos, seg_len) == s) {
            if (go_from(pos + seg_len, x, segs)) {
                return true;
            }
        }
    }
    
    return false;
}

// Naive parser: checks if string can be split into segments
bool naive_split_into_segments(const string& x, const vector<string>& segs) {
    return go_from(0, x, segs);
}

// Check if string can be split into segments (right to left, optimized)
bool parse_right_to_left(const string& x, const vector<string>& segs) {
    size_t pos = x.length();
    
    while (pos > 0) {
        size_t amount_b = 0;
        int i = static_cast<int>(pos) - 1;
        
        // Count trailing 'b's
        while (i >= 0 && x[i] == 'b') {
            amount_b++;
            i--;
        }
        
        if (amount_b >= segs.size()) return false;
        
        const string& seg = segs[amount_b];
        size_t seg_len = seg.length();
        
        if (seg_len > pos) return false;
        
        size_t start = pos - seg_len;
        if (x.substr(start, seg_len) != seg) return false;
        
        pos = start;
    }
    
    return true;
}

// Check if string matches the language (naive approach)
bool naive_matches(const string& s) {
    size_t l = s.length();
    
    // String must start with "bab" or "bbbb"
    if (l < 3) return false;
    
    // Check prefix
    if (s.substr(0, 3) == "bab") {
        // Case 1: starts with "bab"
        if (l == 3) return true;  // just "bab"
        
        // Find split position
        size_t split_pos = 3;
        string rest = s.substr(split_pos);
        
        if (rest.length() % 2 != 0) return false;
        
        size_t half = rest.length() / 2;
        string left = rest.substr(0, half);
        string right = rest.substr(half);
        
        if (left != right) return false;
        
        if (left.empty()) return true;
        
        vector<string> segs = gen_segments(left.length());
        return naive_split_into_segments(left, segs);
    }
    else if (l >= 4 && s.substr(0, 4) == "bbbb") {
        // Case 2: starts with "bbbb"
        if (l == 4) return true;  // just "bbbb"
        
        // Find split position
        size_t split_pos = 4;
        string rest = s.substr(split_pos);
        
        if (rest.length() % 2 != 0) return false;
        
        size_t half = rest.length() / 2;
        string left = rest.substr(0, half);
        string right = rest.substr(half);
        
        if (left != right) return false;
        
        if (left.empty()) return true;
        
        vector<string> segs = gen_segments(left.length());
        return naive_split_into_segments(left, segs);
    }
    
    return false;
}

// Check if string matches the language (optimized approach)
bool optimized_matches(const string& s) {
    size_t l = s.length();
    
    // String must start with "bab" or "bbbb"
    if (l < 3) return false;
    
    // Check prefix
    if (s.substr(0, 3) == "bab") {
        // Case 1: starts with "bab"
        if (l == 3) return true;  // just "bab"
        
        // Find split position
        size_t split_pos = 3;
        string rest = s.substr(split_pos);
        
        if (rest.length() % 2 != 0) return false;
        
        size_t half = rest.length() / 2;
        string left = rest.substr(0, half);
        string right = rest.substr(half);
        
        if (left != right) return false;
        
        if (left.empty()) return true;
        
        vector<string> segs = gen_segments(left.length());
        return parse_right_to_left(left, segs);
    }
    else if (l >= 4 && s.substr(0, 4) == "bbbb") {
        // Case 2: starts with "bbbb"
        if (l == 4) return true;  // just "bbbb"
        
        // Find split position
        size_t split_pos = 4;
        string rest = s.substr(split_pos);
        
        if (rest.length() % 2 != 0) return false;
        
        size_t half = rest.length() / 2;
        string left = rest.substr(0, half);
        string right = rest.substr(half);
        
        if (left != right) return false;
        
        if (left.empty()) return true;
        
        vector<string> segs = gen_segments(left.length());
        return parse_right_to_left(left, segs);
    }
    
    return false;
}

// Generate all strings of length n from 'a' and 'b'
void gen_all_strings(size_t n, string& cur, vector<string>& out) {
    if (cur.length() == n) {
        out.push_back(cur);
        return;
    }
    
    cur.push_back('a');
    gen_all_strings(n, cur, out);
    cur.pop_back();
    
    cur.push_back('b');
    gen_all_strings(n, cur, out);
    cur.pop_back();
}

int main() {
    // Testing for small lengths
    const size_t MAX_LEN = 12;
    const size_t MIN_LEN = 3;
    
    // Test cases
    vector<string> test_cases = {
        // Starts with bab
        "bab",                    // minimal
        "babaa",                  // bab + aa (empty w)
        "bababab",                // bab + abab (w="ab")
        
        // Starts with bbbb
        "bbbb",                   // minimal
        "bbbbaa",                 // bbbb + aa (empty w)
        
        // Invalid
        "aaa",                    // doesn't start with bab or bbbb
        "baba",                   // odd length after bab
        "bbbba",                  // odd length after bbbb
    };
    
    cout << "Testing individual cases (verification):" << endl;
    for (const auto& test : test_cases) {
        bool naive_result = naive_matches(test);
        bool optimized_result = optimized_matches(test);
        
        if (naive_result != optimized_result) {
            cout << "MISMATCH for \"" << test << "\": naive=" << naive_result 
                 << ", optimized=" << optimized_result << endl;
        } else {
            cout << "\"" << test << "\": " << (naive_result ? "accepted" : "rejected") 
                 << " (both methods agree)" << endl;
        }
    }
    
    // Full enumeration for small lengths to verify correctness
    cout << "\nFull enumeration for lengths " << MIN_LEN << "-" << MAX_LEN 
         << " (verifying both methods):" << endl;
    
    int mismatches = 0;
    int total = 0;
    
    for (size_t l = MIN_LEN; l <= MAX_LEN; l++) {
        vector<string> all_strings;
        string cur = "";
        gen_all_strings(l, cur, all_strings);
        
        for (const auto& s : all_strings) {
            total++;
            bool naive_result = naive_matches(s);
            bool optimized_result = optimized_matches(s);
            
            if (naive_result != optimized_result) {
                mismatches++;
                cout << "MISMATCH: \"" << s << "\": naive=" << naive_result 
                     << ", optimized=" << optimized_result << endl;
            }
        }
    }
    
    if (mismatches == 0) {
        cout << "All " << total << " strings verified successfully!" << endl;
    } else {
        cout << "Found " << mismatches << " mismatches out of " << total << " strings" << endl;
    }
    
    // Performance comparison
    cout << "\nPerformance comparison for lengths 13-20:" << endl;
    
    const size_t PERF_MIN_LEN = 13;
    const size_t PERF_MAX_LEN = 20;
    
    long long naive_time = 0;
    long long optimized_time = 0;
    int total_strings = 0;
    
    for (size_t l = PERF_MIN_LEN; l <= PERF_MAX_LEN; l++) {
        vector<string> all_strings;
        string cur = "";
        gen_all_strings(l, cur, all_strings);
        
        total_strings += all_strings.size();
        
        // Test naive parser
        auto naive_start = high_resolution_clock::now();
        for (const auto& s : all_strings) {
            naive_matches(s);
        }
        auto naive_end = high_resolution_clock::now();
        auto naive_duration = duration_cast<milliseconds>(naive_end - naive_start).count();
        naive_time += naive_duration;
        
        // Test optimized parser
        auto optimized_start = high_resolution_clock::now();
        for (const auto& s : all_strings) {
            optimized_matches(s);
        }
        auto optimized_end = high_resolution_clock::now();
        auto optimized_duration = duration_cast<milliseconds>(optimized_end - optimized_start).count();
        optimized_time += optimized_duration;
        
        cout << "Length " << l << ": naive=" << naive_duration << "ms, "
             << "optimized=" << optimized_duration << "ms, "
             << "speedup=" << (double)naive_duration / optimized_duration << "x" << endl;
    }
    
    cout << "\nPerformance summary:" << endl;
    cout << "Total strings tested: " << total_strings << endl;
    cout << "Total naive time: " << naive_time << "ms" << endl;
    cout << "Total optimized time: " << optimized_time << "ms" << endl;
    cout << "Overall speedup: " << (double)naive_time / optimized_time << "x" << endl;
    cout << "Average time per string (naive): " << (double)naive_time / total_strings << "ms" << endl;
    cout << "Average time per string (optimized): " << (double)optimized_time / total_strings << "ms" << endl;
    
    return 0;
}