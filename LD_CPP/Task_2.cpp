#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct LA_Result {
    int cost;
    vector<string> operations;
};

int minimum_cost(int a, int b, int c) {
    return min(a, min(b, c));
} 

// DP Matrix Method with different operational costs - Return only the cost
// Time Complexity: O(M * N)    - traversing the matrix
// Space Complexity: O(M * N)   - 2D matrix
int levenshtein_Weighted_Cost_Only(const string& str1, const string& str2, int insert_Cost, int delete_Cost, int replace_Cost) {

    int m = str1.length();
    int n = str2.length();

    vector<vector<int>> dp_Matrix(m + 1, vector<int>(n + 1));   // Space Complexity: O(M * N)

    for (int i = 0; i <= m; i++) {
        dp_Matrix[i][0] = i * delete_Cost;
    }

    for (int j = 0; j <= n; j++) {
        dp_Matrix[0][j] = j * insert_Cost;
    }

    // Fill the dp matrix
    for (int i = 1; i <= m; i++) {              // Time Complexity: O(M)
        for (int j = 1; j <= n; j++) {              // Time Complexity: O(M*N)    
            if (str1[i - 1] == str2[j - 1]) {
                dp_Matrix[i][j] = dp_Matrix[i - 1][j - 1];
            } else {
                int ins = dp_Matrix[i][j - 1] + insert_Cost;
                int del = dp_Matrix[i - 1][j] + delete_Cost;
                int rep = dp_Matrix[i - 1][j - 1] + replace_Cost;

                dp_Matrix[i][j] = minimum_cost(ins, del, rep);
            }
        }
    }

    return dp_Matrix[m][n];
}


// DP Matrix Method with different operational costs - Return the cost and operations
// Time Complexity: O(M * N)            Traversing the matrix
// Space Complexity: O(2(M * N))       2D matrix + operations vector 
LA_Result levenshtein_Weighted_Cost_And_Operation(const string& str1, const string& str2, int insert_Cost, int delete_Cost, int replace_Cost) {
    int m = str1.length();
    int n = str2.length();

    vector<vector<LA_Result>> dp_Matrix(m + 1, vector<LA_Result>(n + 1));

    for (int i = 0; i <= m; i++) {
        dp_Matrix[i][0].cost = i * delete_Cost;
        for (int k = 0; k < i; k++)
            dp_Matrix[i][0].operations.push_back("DELETE '" + string(1, str1[k]) + "'");
    }

    for (int j = 0; j <= n; j++) {
        dp_Matrix[0][j].cost = j * insert_Cost;
        for (int k = 0; k < j; k++)
            dp_Matrix[0][j].operations.push_back("INSERT '" + string(1, str2[k]) + "'");
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp_Matrix[i][j] = dp_Matrix[i - 1][j - 1];
                dp_Matrix[i][j].cost = dp_Matrix[i - 1][j - 1].cost;
                dp_Matrix[i][j].operations.push_back("MATCH '" + string(1, str1[i - 1]) + "'");
            } else {
                int ins = dp_Matrix[i][j - 1].cost + insert_Cost;
                int del = dp_Matrix[i - 1][j].cost + delete_Cost;
                int rep = dp_Matrix[i - 1][j - 1].cost + replace_Cost;

                int minVal = minimum_cost(ins, del, rep);
                dp_Matrix[i][j].cost = minVal;

                if (minVal == rep) {
                    dp_Matrix[i][j].operations = dp_Matrix[i - 1][j - 1].operations;
                    dp_Matrix[i][j].operations.push_back("REPLACE '" + string(1, str1[i - 1]) + "' with '" + string(1, str2[j - 1]) + "'");
                }
                else if (minVal == del) {
                    dp_Matrix[i][j].operations = dp_Matrix[i - 1][j].operations;
                    dp_Matrix[i][j].operations.push_back("DELETE '" + string(1, str1[i - 1]) + "'");
                }
                else {
                    dp_Matrix[i][j].operations = dp_Matrix[i][j - 1].operations;
                    dp_Matrix[i][j].operations.push_back("INSERT '" + string(1, str2[j - 1]) + "'");
                }
            }
        }
    }

    return dp_Matrix[m][n];
}


int EditDistance(string s1, string s2, int Ci, int Cd, int Cs) {

    int flag = 0;
    cout << "Do you want detailed operations? (yes=1 / no=1)" << endl;
    cout << "Enter your choice (1/0): ";
    cin >> flag;

    if (flag == 1) {
        LA_Result result = levenshtein_Weighted_Cost_And_Operation(s1, s2, Ci, Cd, Cs);
        cout << "Operations to convert '" << s1 << "' to '" << s2 << "':\n";
        for (auto op : result.operations)
            cout << " - " << op << "\n";
        cout << "\n";
        return result.cost;
    }

    return levenshtein_Weighted_Cost_Only(s1, s2, Ci, Cd, Cs);
}


int main() {
    cout << "This is a C++ program to calculate the Weighted Levenshtein distance.\n";
    cout << "All operations (insert, delete, replace) have different costs.\n\n";

    int numInputs;
    cout << "Enter the number of string pairs you want to process: ";
    cin >> numInputs;
    cout << "\n";

    for (int i = 1; i <= numInputs; i++) {
        string str1, str2;
        int insert_Cost, delete_Cost, replace_Cost;

        cout << " ***** ***** ***** ***** Pair " << i << " ***** ***** ***** ***** \n" << endl;
        cout << "Enter the first string (The original string): ";
        cin >> str1;
        cout << "Enter the second string (String to convert to): ";
        cin >> str2;
        cout << "Enter the cost for insertion: ";
        cin >> insert_Cost; 
        cout << "Enter the cost for deletion: ";
        cin >> delete_Cost;
        cout << "Enter the cost for replacement: ";
        cin >> replace_Cost;
        cout << "\nResults for { Pair " << i << " }\n";
        int res = EditDistance(str1, str2, insert_Cost, delete_Cost, replace_Cost);
        cout << "Levenshtein Distance for Pair " << i << ": " << res << endl;
        cout << "\n\n\n\n";
    }

    return 0;
}