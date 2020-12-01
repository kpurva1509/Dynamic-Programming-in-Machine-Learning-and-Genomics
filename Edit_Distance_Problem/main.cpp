#include "headers.hpp"

/* Algorithm to transform str1 into str2 with least # of
 * transformations 
 *                           | edit_distance[i-1][j-1] + 1 (otherwise)
 *                           | edit_distance[i-1][j-1] + 0 (if str1[i-1]==str2[j-1])
 * edit_distance[i][j] = min | edit_distance[i][j-1]+1
 *                           | edit_distance[i-1][j]+1
 */
int edit_distance_func(const string &str1, const string &str2)
{
    int n = str1.length();
    int m = str2.length();
    int result = 0;

    int **edit_distance = new int *[n + 1];
    for (int i = 0; i < n + 1; i++)
    {
        edit_distance[i] = new int[m + 1];
        memset(edit_distance[i], INF, sizeof(int) * (m + 1));
    }
    // memset(edit_distance,INF,sizeof(edit_distance[0][0]*(n+1)*(m+1)));
    edit_distance[0][0] = 0;

    for (int i = 1; i < n + 1; i++)
    {
        edit_distance[i][0] = edit_distance[i - 1][0] + 1;
    }

    for (int j = 1; j < m + 1; j++)
    {
        edit_distance[0][j] = edit_distance[0][j - 1] + 1;
    }

    for (int i = 1; i < n + 1; i++)
    {
        for (int j = 1; j < m + 1; j++)
        {
            /* least cost transition */
            int path1 = edit_distance[i][j - 1] + 1;
            int path2 = edit_distance[i - 1][j] + 1;
            int path3;
            if (str1[i - 1] == str2[j - 1])
            {
                path3 = edit_distance[i - 1][j - 1];
            }
            else
            {
                path3 = edit_distance[i - 1][j - 1] + 1;
            }
            if (path1 <= path2 && path1 <= path3)
            {
                edit_distance[i][j] = path1;
            }
            else if (path2 <= path1 && path2 <= path3)
            {
                edit_distance[i][j] = path2;
            }
            else
            {
                edit_distance[i][j] = path3;
            }
        }
    }

    result = edit_distance[n][m];

    for (int i = 0; i <= n; i++)
    {
        delete[] edit_distance[i];
    }
    delete[] edit_distance;

    return result;
}

int main()
{
    string str1;
    string str2;
    std::cin >> str1 >> str2;
    std::cout << edit_distance_func(str1, str2) << std::endl;
    return 0;
}

/* Edx compiling cmd -
 * g++ -o edit_distance edit_distance.cpp */
