#include "headers.hpp"

/* Middle - diagonal movements */
/* Upper - horizontal movements(gap in t) */
/* Lower - vertical movements(gap in s) */

string align(int match_score, int mismatch_mu, int sigma, int eps, const string &s, const string &t)
{
    int n = t.length(); // nt
    int m = s.length(); // ns

    int lower[n+1][m+1];
    int upper[n+1][m+1];
    int middle[n+1][m+1];

    std::string backtrack_lower[n+1][m+1];
    std::string backtrack_upper[n+1][m+1];
    std::string backtrack_middle[n+1][m+1];

    middle[0][0] = 0;
    lower[0][0] = -INF;
    upper[0][0] = -INF;

    /* upper matrix is for horizontal movements */
    /* therefore, mark vertical edges -INF */
    for(int i=1; i<n+1; i++) {
        upper[i][0] = -INF;
        backtrack_upper[i][0] = "midtoup";
    }

    /* lower matrix is for vertical movements */
    /* therefore, mark horizontal edges -INF */
    for(int j=1; j<m+1; j++) {
        lower[0][j] = -INF;
        backtrack_lower[0][j] = "midtolow";
    }

    /* vertical movements of the lower matrix */
    for(int i=1; i<n+1; i++) {
        int path1 = lower[i-1][0] - eps;
        int path2 = middle[i-1][0] - sigma;
        if(path1 >= path2) {
            lower[i][0] = path1;
            backtrack_lower[i][0] = "lowtolow";
        } else {
            lower[i][0] = path2;
            backtrack_lower[i][0] = "midtolow";
        }
        middle[i][0] = lower[i][0];
        backtrack_middle[i][0] = "lowtomid";
    }

    /* horizontal movements of the upper matrix */
    for(int j=1; j<m+1; j++) {
        int path1 = upper[0][j-1] - eps;
        int path2 = upper[0][j-1] - sigma;
        if(path1>=path2) {
            upper[0][j] = path1;
            backtrack_upper[0][j] = "uptoup";
        } else {
            upper[0][j] = path1;
            backtrack_upper[0][j] = "midtoup";
        }
        middle[0][j] = upper[0][j];
        backtrack_upper[0][j] = "uptomid";
    }

    /* diagonal movements of the middle matrix */
    for(int i=1; i<n+1; i++) {
        for(int j=1; j<m+1; j++) {

            /* vertical movement */
            int path1 = lower[i-1][j] - eps;
            int path2 = middle[i-1][j] - sigma;
            if(path1>=path2) {
                lower[i][j] = path1;
                backtrack_lower[i][j] = "lowtolow";
            } else {
                lower[i][j] = path2;
                backtrack_lower[i][j] = "midtolow";
            }

            /* horizontal movement */
            path1 = upper[i][j-1] - eps;
            path2 = middle[i][j-1] - sigma;
            if (path1 >= path2) {
                upper[i][j] = path1;
                backtrack_upper[i][j] = "uptoup";
            } else {
                upper[i][j] = path2;
                backtrack_upper[i][j] = "midtoup";
            }

            /* diagonal movement */
            path1 = lower[i][j];
            if(t[i-1] == s[j-1]){
                path2 = middle[i-1][j-1] + match_score;
            } else {
                path2 = middle[i-1][j-1] - mismatch_mu;
            }
            int path3 = upper[i][j];
            if(path1>=path2 && path1>=path3) {
                middle[i][j] = path1;
                backtrack_middle[i][j] = "lowtomid";
            } else if (path3>=path2) {
                middle[i][j] = path3;
                backtrack_middle[i][j] = "uptomid";
            } else {
                middle[i][j] = path2;
                backtrack_middle[i][j] = "midtomid";
            }
        }
    }

    std::string s_annotated;
    std::string t_annotated;

    int i = n;
    int j = m;
    std::string floor = "middle";
    while(i>0 || j>0) {
        if(floor == "middle") {
            if(backtrack_middle[i][j] == "lowtomid") {
                floor = "lower";
            } else if(backtrack_middle[i][j] == "midtomid") {
                s_annotated += s[j-1];
                t_annotated += t[i-1];
                i--; j--;
            } else {
                floor = "upper";
            }
        } else if(floor == "lower") {
            if(backtrack_lower[i][j] == "lowtolow") {
                s_annotated += "-";
                t_annotated += t[i-1];
                i--;
            } else {
                s_annotated += "-";
                t_annotated += t[i - 1];
                i--;
                floor = "middle";
            }
        } else {
            if (backtrack_upper[i][j] == "uptoup") {
                s_annotated += s[j-1];
                t_annotated += "-";
                j--;
            } else {
                s_annotated += s[j - 1];
                t_annotated += "-";
                j--;
                floor = "middle";
            }
        }
    }

    std::reverse(s_annotated.begin(), s_annotated.end());
    std::reverse(t_annotated.begin(), t_annotated.end());

    std::string result = std::to_string(middle[n][m])+"\n"+s_annotated+"\n"+t_annotated;

    return result;
}

int main()
{
    int m, mu, sigma, eps;
    cin >> m >> mu >> sigma >> eps;
    cin.ignore();
    string s, t;
    getline(cin, s);
    getline(cin, t);

    cout << align(m, mu, sigma, eps, s, t) << endl;
    return 0;
}

/* TEST CASES 5 AND 6 ***

Purvas-MacBook-Pro% ./main (PASSED)
1 3 2 1
GA
GTTA
-1
G--A
GTTA

Purvas-MacBook-Pro% ./main (FAILED, 2ND STRING -TT)
1 5 3 1
TTT
TT
-1
TTT
TT-

Purvas-MacBook-Pro% ./main (PASSED)
1 5 2 1
CCAT
GAT
-3
-CCAT
G--AT

Purvas-MacBook-Pro% ./main (PASSED)
1 2 3 2
CAGGT
TAC
-8
CAGGT
TAC--

Purvas-MacBook-Pro% ./main (FAILED, 1ST STRING --GT--TCCAGGTA 2ND STRING CAGTAGTC---GT-)
2 3 3 2
GTTCCAGGTA
CAGTAGTCGT
-8
--GTTCCAG--GTA
CAGT---AGTCGT-

Purvas-MacBook-Pro% ./main (FAILED ALTOGETHER)
1 3 1 1
AGCTAGCCTAG
GT
-10
-AGCTAGCCTAG
G---T-------

Purvas-MacBook-Pro% ./main (FAILED, 1ST STRING -------A--A)
2 1 2 1
AA
CAGTGTCAGTA
-7
-A--------A
CAGTGTCAGTA

Purvas-MacBook-Pro% ./main (PASSED)
5 2 15 5
ACGTA
ACT
-12
ACGTA
ACT--

*/