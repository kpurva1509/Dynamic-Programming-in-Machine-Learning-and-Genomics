#include "headers.hpp"

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

    int** longest_path = new int*[n+1];
    std::string** backtrack = new std::string*[n+1];
    for(int i=0; i<n+1; i++){
        longest_path[i] = new int[m+1];
        backtrack[i] = new std::string[m+1];
    }

    longest_path[0][0] = 0;

    for(int i=1; i<n+1; i++) {
        longest_path[i][0] = longest_path[i-1][0] - indel_sigma;
        backtrack[i][0] = "down";
    }

    for(int j=1; j<m+1; j++){
        longest_path[0][j] = longest_path[0][j-1] - indel_sigma;
        backtrack[0][j] = "right";
    }

    for(int i=1; i<n+1; i++) {
        for(int j=1; j<m+1; j++) {
            int path1 = longest_path[i-1][j] - indel_sigma;
            int path2 = longest_path[i][j-1] - indel_sigma;
            int path3;
            if(s[i-1]==t[j-1]){
                path3 = longest_path[i-1][j-1] + match_score;
            } else {
                path3 = longest_path[i-1][j-1] - mismatch_mu;
            }
            if(path1>=path2 && path1>=path3){
                longest_path[i][j] = path1;
                backtrack[i][j] = "down";
            } else if(path2>=path1 && path2>=path3){
                longest_path[i][j] = path2;
                backtrack[i][j] = "right";
            }else{
                longest_path[i][j] = path3;
                backtrack[i][j] = "diagonal";
            }
        }
    }

    /* down == gap in 1st string s */
    /* right == gap in 2nd strin t */
    int i=n;
    int j=m;
    std::string s_annotated, t_annotated;
    while (i>0 || j>0) {
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
    ss << longest_path[n][m];
    std::string str;
    ss >> str;

    /* Clear memory assigned to dynamically created 2D matrices */
    for (int i=0; i <= n; i++)
    {
        delete [] longest_path[i];
        delete [] backtrack[i];
    }

    delete [] longest_path;
    delete [] backtrack;

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
