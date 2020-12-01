#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include <functional>
#include <string>
#include <algorithm>

using std::cout;
using std::endl;

#define INF 32767

using namespace std;

string align(const int match_score, const int mismatch_mu, const int indel_sigma, const string s, const string t)
{

    int n, m;
    n = s.length();
    m = t.length();

    /* (n+1)x(m+1) tall-thin matrix, ideally */
    int longest_path[n + 1][m + 1];
    char backtrack[n + 1][m + 1];

    longest_path[0][0] = 0;

    /* initialize the first column to 0 */
    for (int i = 1; i < n + 1; i++)
    {
        longest_path[i][0] = 0;
        backtrack[i][0] = 'd';
    }

    /* initialize first row to 0 */
    for (int j = 1; j < m + 1; j++)
    {
        longest_path[0][j] = 0;
        backtrack[0][j] = 'r';
    }

    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 1; j < m + 1; j++)
        {
            int path1 = longest_path[i - 1][j] - indel_sigma;
            int path2 = longest_path[i][j - 1] - indel_sigma;
            int path3 = longest_path[i - 1][j - 1] + ((s[i - 1] == t[j - 1]) ? match_score : -mismatch_mu);
            if (path1 >= path2 && path1 >= path3)
            {
                longest_path[i][j] = path1;
                backtrack[i][j] = 'd';
            }
            else if (path2 >= path1 && path2 >= path3)
            {
                longest_path[i][j] = path2;
                backtrack[i][j] = 'r';
            }
            else
            {
                longest_path[i][j] = path3;
                backtrack[i][j] = 'm';
            }

        }
    }

    /* we need till the end of suffix string s, which is the LAST ROW */
    /* find column with max value in last row */

    int max_score = -INF;
    int i, j, max_i, max_j;
    max_i = n;

    for(int j=0; j < m+1; j++){
        if(longest_path[max_i][j] >= max_score){
            max_score = longest_path[max_i][j];
            max_j = j;
        }
    }

    i = max_i;
    j = max_j;

    /* down == gap in 1st string s */
    /* right == gap in 2nd string t */
    std::string s_annotated, t_annotated;
    while (i > 0 && j > 0)
    {
        if (backtrack[i][j] == 'd')
        {
            s_annotated += s[i - 1];
            t_annotated += "-";
            i--;
        }
        else if (backtrack[i][j] == 'r')
        {
            s_annotated += "-";
            t_annotated += t[j - 1];
            j--;
        }
        else if (backtrack[i][j] == 'm')
        {
            s_annotated += s[i - 1];
            t_annotated += t[j - 1];
            i--;
            j--;
        }
    }

    std::reverse(s_annotated.begin(), s_annotated.end());
    std::reverse(t_annotated.begin(), t_annotated.end());
    std::stringstream ss;
    ss << max_score;
    std::string str;
    ss >> str;

    return str + "\n" + s_annotated + "\n" + t_annotated;
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
