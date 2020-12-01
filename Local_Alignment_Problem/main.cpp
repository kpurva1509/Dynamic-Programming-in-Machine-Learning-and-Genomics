#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <functional>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <algorithm>
#include <vector>
#include <map>
#include <typeinfo>
#include <type_traits>
#include <queue>
#include <tuple>
// #include <bits/stdc++.h>

using std::cout;
using std::endl;

#define INF 32767

using namespace std;

/*
 * m, mu, and sigma are the scoring parameters
 * s and t are the strings to be aligned
 *
 * return a string with the alignment score and the reconstructed alignment
 * be sure to follow the problem's output format
 */
string align(const int& match_score, const int& mismatch_mu, const int& indel_sigma, const string &s, const string &t)
{
    int n,m;
    n = s.length();
    m = t.length();

    int longest_path[n+1][m+1];
    std::string backtrack[n+1][m+1];

    longest_path[0][0] = 0;

    for(int i=1; i<n+1; i++) {
        longest_path[i][0] = 0;
        backtrack[i][0] = "down"; //*************
    }

    for(int j=1; j<m+1; j++){
        longest_path[0][j] = 0;
        backtrack[0][j] = "right";//*************
    }

    for(int i=1; i<n+1; i++) {
        for(int j=1; j<m+1; j++) {
            int path0 = 0;
            int path1 = longest_path[i-1][j] - indel_sigma;
            int path2 = longest_path[i][j-1] - indel_sigma;
            int path3 = longest_path[i-1][j-1] + ((s[i-1]==t[j-1]) ? match_score : -mismatch_mu);

            if (path0 >= path1 && path0 >= path2 && path0 >= path3)
            {
                longest_path[i][j] = path0;
                backtrack[i][j] = "free-ride";
            }
            else if (path1 >= path0 && path1 >= path2 && path1 >= path3)
            {
                longest_path[i][j] = path1;
                backtrack[i][j] = "down";
            }
            else if (path2 >= path0 && path2 >= path1 && path2 >= path3)
            {
                longest_path[i][j] = path2;
                backtrack[i][j] = "right";
            }
            else
            {
                longest_path[i][j] = path3;
                backtrack[i][j] = "diagonal";
            }
        }
    }

    /* get the index to longest path i,j */
    int max = -1*INF;
    int max_i, max_j;
    for(int i=1; i<n+1; i++) {
        for(int j=1; j<m+1; j++) {
            if(longest_path[i][j] > max){
                max = longest_path[i][j];
                max_i = i;
                max_j = j;
            }
        }
    }

    /* down == gap in 1st string s */
    /* right == gap in 2nd strin t */
    int i=max_i;
    int j=max_j;
    std::string s_annotated, t_annotated;
    while ((backtrack[i][j]!="free-ride") && (i>0 && j>0)) {
        if(backtrack[i][j] == "down") {
            s_annotated += s[i-1];
            t_annotated += "-";
            i--;
        } else if (backtrack[i][j] == "right") {
            s_annotated += "-";
            t_annotated += t[j-1];
            j--;
        } else if (backtrack[i][j] == "diagonal") {
            s_annotated += s[i-1];
            t_annotated += t[j-1];
            i--;
            j--;
        }
    }

    std::reverse(s_annotated.begin(), s_annotated.end());
    std::reverse(t_annotated.begin(), t_annotated.end());
    std::stringstream ss;
    ss << max;
    std::string str;
    ss >> str;

    return str+"\n"+s_annotated+"\n"+t_annotated;

}

int main()
{
    int m, mu, sigma;
    cin >> m >> mu >> sigma;
    cin.ignore();
    string s, t;
    getline(cin, s);
    getline(cin, t);

    cout << align(m, mu, sigma, s, t) << endl;
    return 0;
}
