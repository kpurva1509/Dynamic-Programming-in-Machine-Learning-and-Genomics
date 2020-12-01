#include "headers.cpp"

string LCS(const string& s, const string& t) {
    
    /* Query the length of the strings and create
     * a 2D matrix of the size of the strings
     */
    unsigned n = s.size();
    unsigned m = t.size();

    int** longest_path = new int*[n+1];
    string** backtrack = new string*[n+1];
    for(int i=0; i<=n; i++) {
        longest_path[i] = new int[m+1];
        backtrack[i] = new string[m+1];
        /* memset(longest_path[i],0,(m+1)*sizeof(int)); */
    }

    /* Initialize the first row and the first column
     * of the matrix to 0 
     */

    longest_path[0][0] = 0;

    for(int i=1; i<=n; i++) {
        longest_path[i][0] = longest_path[i-1][0];
        backtrack[i][0] = "down";
    }

    for(int j=1; j<=m; j++) {
        longest_path[0][j] = longest_path[0][j-1];
        backtrack[0][j] = "right";
    }

    for(int i=1; i<=n; i++) {
        for(int j=1; j<=m; j++) {
            int path1 = longest_path[i-1][j];
            int path2 = longest_path[i][j-1];
            int path3;
            if (s[i-1] == t[j-1]) {
                /* +1 increase in score since found a match between
                 * the 2 input strings 
                 */
                path3 = longest_path[i-1][j-1]+1;
            } else {
                path3 = longest_path[i-1][j-1];
            }
            if(path1>=path2 && path1>=path3) {
                longest_path[i][j] = path1;
                backtrack[i][j] = "down";
            } else if(path2>=path1 && path2>=path3) {
                longest_path[i][j] = path2;
                backtrack[i][j] = "right";
            } else {
                longest_path[i][j] = path3;
                backtrack[i][j] = "diagonal";
            }
        }
    }

    int i = n;
    int j = m;
    string LCS;
    while(i>0 && j>0) {
        if(backtrack[i][j] == "down") {
            i--;
        } else if(backtrack[i][j] == "right") {
            j--;
        } else { /* backtrack[i][j] == "diagonal" */
            if(s[i-1] == t[j-1]) {
                // LCS.append(s[i-1]);
                LCS += s[i-1];
            }
            /* Have to backtrack diagonally irrespective of
             * match being found or not 
             * else will get stuck in an infinite loop
             */
            i--;
            j--;
        }
    }

    std::reverse(LCS.begin(), LCS.end());

    /* Delete dynamically allocated arrays longest_path and backtrack */
    for(int j=0; j<=m; j++) {
        delete [] longest_path[j];
        delete [] backtrack[j];
    }

    delete [] longest_path;
    delete [] backtrack;

    return LCS;
}

int main() {
    string s,t;
    //cin >> s >> t;
    s = "ACCGTCTTAGCGATCAACACATTTAACAACGCGCCGCACCCCCCGTCAAACGAGCTTTTGGGCTCTTGTCCTTTTACAAGCTTCACGACGCATACAGCCTTGATCAACGGTTTGATCTGTCTCCCTTCAGCTGGCTTTAAAGGACATACATATGAAGGCCTTAATAAGGTCCGGGAACTCCACATATTCGGTACTGGGCAAACCCCATGAACCACCTCAACATGAAGAGTCCGAGGACTCTCACGATCCACCAATGCAGATCGGAACTGTGCGATCGCGTAATGAGCCGAGTACTTGGTTTGTGTTTAGGTTATGGGGGCCGGGAGCCGGTTCAATATAAGGAAGTAGTTGCAGATTAGTTGTTGCGAACGGTCATAAATTTGATGGGTAAACGTGAACTTAACAAACCGTGATAGCTAATCCTATGCATCCCTTACGTGGATCGACTCGAGTACCCAGGTGAACCGACTACTTGATAACCGGAAATCGCGGTATAAAAGCGCTCACGGTCAGGAGATATACCTCCAAGCAGTAGTCTTTCTGAGCCTAGAGTAGTAAATTACAGGGACGATGTCTTTTACCGAGGCAACATTTTATTGAGAATCACATGAGGCACAGGTAAAGGCGACATCACGATCGAGATCAACCCCTACTTGTTCAAAACATTGAGAACCAGCTCTGTTTTGGAACCTAGAAAGATAACGCATCCGCTTGATATTCCACGGCTTGTCCCTCTTGTGCGGTCCATCTATCGGAGTTTCCTCCGATACGACCCGCAATGTTTCCAGGCGTACGGTACTTTATGAATACACTCGCGCTGTAACCTGTTATGTGAAACACACACGACAGAGCTTCGCGTGGGCCCAGCGACCCGGTAATACTACATCACCGCACACGACCTCGAGCAGTCTTTGCCGGCGTCCGTAAGTAGTCTAAAGTTGTGTTGATGCTTGGGGTTAAAGCTAAATCGTCCGCAGAATACGACTCTCATCCCAAT";
    t = "ACCCGCACGCGCTTTGGTCTAGATTCTAGCTCCAACTTGCCTGCTAGATACTCTGTTAAAAGATGGTTTTACAACCCCCTCCTCTGTCCCTGGGGTATTATATAATACGTCGGATAGTCAGGTACAAATACAAGTGGGTGGGAATACTTTTCCTCGGATCCTAGACCACGGATTACTGCGTGGTTGACAAGAGTCGGCCCGGAGGGAAACGTGAAGGTTAGTGCAATTAAAGTCTCTAATGTGAAGCCTCCGCGAAGCGAGGAGTTTCTGAGATCGAGTACTATTTAGAGTTCGAAATCACGGCTTAACCTCACTGCCACGCATAACTTGCCGGCAATCCAGTTTTGCAACGATACTTAATTTGTGCAGCTCATCTTTGCTGTCCAGAAATAGAGCTAGTCGATCTCATCTTGCGGGTAGCCAGAAGTCCTACCGTCTCCTCCATGTAGCTTAAAAATTTCGGTGAGGATCAAAAATGATAAACGTGACAGGTAAGCTCCTACGTCTATCCTATGACCCCCGCGGCAGAATAGGTTGGTAGTGTTAGTGCGTGAGCTGGTAGAATAGAGCACACTTAGGGAAACGGGAACCGTTATGTAGGGCTGCGACACACAAAAAAGTGTTCGTTGGTAAGCTGCCTCTCCACTAAACAGGATTTCTCTGGATGATCCCATCGAAGCAAGTTACGCACCACGCCGAGGCGGACCCTGGTACTAGCTGCCCCCCCCTTTATGGGGCGCTCGTACATCAAGATGATCGCGGACTCAACCTGATTACGAGTTGTCCAAGTAGTCCAGGGTAAGAGAAACTGGAGAGA";
    cout << LCS(s,t) << endl;
}

