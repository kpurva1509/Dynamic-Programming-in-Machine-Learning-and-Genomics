#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
// #include <string_view>
#include <vector>
// #include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::endl;
using std::string;

using namespace std;

/* recursion can cause stack overflow for long strings */
void strrev(std::string& str, int start, int end) {
    
    /* terminating condition */
    if(end <= start) return;
    
    std::swap(str[start], str[end]);
    strrev(str, start+1, end-1);

}

/* Returns an array of score until the middle column */
std::vector<int> middle_column_score(int match_score, int mismatch_mu, int sigma, std::string &s, std::string t)
{
    /* Returns the score of the middle column for alignment of s and t */
    int n = s.length();
    int m = t.length();

    /* initialize the first column with indel sigma */
    std::vector<int> scores(n + 1, 0);

    bool first_match = true;
    std::vector<bool> matched(n, false);

    std::vector<char> backtrack;
    backtrack.reserve(n+1);

    /* mismatch_mu is not required */
    /* why are limits of i and j different? */
    /* how to record backtracking information? */
    for (int j = 1; j <= m + 1; j++) {
        for (int i = 0; i < n + 1; i++){
            
            if(i == 0 ){
                scores[i] = -j*sigma;
            } else {
                if (!matched[i]) {
                    if (s[i - 1] == t[j - 1] && first_match) {
                        scores[i] = scores[i - 1] + match_score;
                        first_match = false;
                        matched[i] = true;
                        goto label;
                    } else { /* to accept mismatch or to skip characters here */
                        scores[i] = scores[i - 1]; /* - sigma */
                    }
                }
            }
        }
    label:
        first_match = true;
    }
    return scores;
}

string middle_edge(int match_score, int mismatch_mu, int sigma, std::string &s, std::string &t)
{
    // std::function< <std::pair<int,int>(int, int[], int[], std::string, std::string)>
    int n = s.length();
    int m = t.length();
    int middle = (m+1)/2;
    /* nxm matrix, middle row is m/2 */

    std::vector<int> from_source(n + 1);
    from_source = middle_column_score(match_score, mismatch_mu, sigma, s, t.substr(0, middle));

    /* reverse the strings */
    strrev(s,0,s.length()-1); strrev(t,0,t.length()-1);
    std::vector<int> to_sink(n + 1);
    if(m%2 == 0) {
        to_sink = middle_column_score(match_score, mismatch_mu, sigma, s, t.substr(0, middle+1));
    } else {
        to_sink = middle_column_score(match_score, mismatch_mu, sigma, s, t.substr(0, middle));
    } 

    std::vector<int> scores(n+1);
    std::transform(from_source.begin(), from_source.end(), to_sink.rbegin(), scores.begin(), std::plus<int>());
    int index = std::distance(scores.begin(), std::max_element(scores.begin(), scores.end()));

    /* get scores from middle edge, horz, vert and diag */
    int horizontal, vertical, diagonal;
    horizontal = vertical = *(std::max_element(scores.begin(), scores.end())) - sigma;


    std::pair<int, int> next_node;
    if (index == scores.size() - 1) {
        // TODO
    } else {
        // TODO
    }

    return "("+std::to_string(index)+", "+std::to_string(middle)+")";
}

int main()
{
    int m, mu, sigma;
    cin >> m >> mu >> sigma;
    cin.ignore();
    string s, t;
    getline(cin, s);
    getline(cin, t);

    cout << middle_edge(m, mu, sigma, s, t) << endl;

    return 0;
}
