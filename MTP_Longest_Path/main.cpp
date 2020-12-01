#include <iostream>
#include <algorithm>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

typedef vector<vector<int> > matrix;

int longest_path(int n, int m, matrix down, matrix right) {
    
    /* Dynamically initializing a 2D Matrix for longest path */
    /* Initialize all elements of longest_path to 0 */
    int **longest_path;
    longest_path = new int*[n+1];
    for(int i=0; i<n+1; i++){
        longest_path[i] = new int[m+1];
        memset(longest_path[i], 0, (m+1)*sizeof(int));
    }

    /* 
    std::shared_ptr<std::shared_ptr<int[]> [] > longest_path_2(new std::shared_ptr<int[]>[n+1]);
    std::shared_ptr<int[]> smart_pointer_1d;
    smart_pointer_2d = std::make_shared<std::make_shared<int[]>[]>(n+1);
    for(int i=0; i<=n;i++) {
        // smart_pointer_1d = std::make_shared<int[]>(m+1);
        // smart_pointer_2d[i] = smart_pointer_1d[i];
        longest_path_2[i] = new std::shared_ptr<int[]>[m+1];
    }
    */

    for(int i =1; i<=n; i++){
        longest_path[i][0] = longest_path[i-1][0] + down[i-1][0];
    }

    for(int j=1; j<=m; j++){
        longest_path[0][j] = longest_path[0][j-1] + right[0][j-1];
    }

    for(int i=1; i<=n; i++){
        for(int j=1; j<=m; j++){
            longest_path[i][j] = std::max(longest_path[i-1][j]+down[i-1][j-1],
                                          longest_path[i][j-1]+right[i-1][j-1]);
        }
    }
    return longest_path[n][m];
}

int main() {
    int n,m;
    cout << "Enter the dimensions of the grid: ";
    cin >> n;
    cin >> m;

    /* nx(m+1) matrix - weight of vertical edge from (i-1, j) to (i,j) */
    /* (n+1)x(m+1) with the 1st row 0 is more consistent */
    cout << "Enter the elements of the down matrix :";
    matrix down;
    for (int i = 0; i < n; i++) {
        vector<int> curr(m+1);
        for (int j = 0; j < m+1; j++) {
            cin >> curr[j];
        }
        down.push_back(curr);
    }

    cin.ignore(256,'-');

    /* (n+1)xm matrix - weight of horizontal edge from (i, j-1) to (i,j) */
    /* (n+1)x(m+1) with the 1st column 0 is more consistent */
    cout << "Enter the elements of the right matrix :";
    matrix right;
    for (int i = 0; i < n+1; i++) {
        vector<int> curr(m);
        for (int j = 0; j < m; j++) {
            cin >> curr[j];
        }
        right.push_back(curr);
    }

    cout << longest_path(n,m,down,right) << endl;
    return 0;
}

