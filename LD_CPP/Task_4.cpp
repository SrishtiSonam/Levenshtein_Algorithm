#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

class Levenshtein_Distance {

    int minimum_Cost(int a, int b) {
        return (a <= b) ? a : b;
    }

    int minimum_Cost(int a, int b, int c) {
        return min(min(a, b), c);
    }
    
    void print_Vector(const vector<int>& vec, const string& label) {
        cout << label << ": [";
        for (int i = 0; i < vec.size(); i++) {
            cout << setw(3) << vec[i];
            if (i < vec.size() - 1) cout << ",";
        }
        cout << " ]" << endl;
    }
    

public:

    // DP Vector Method with different operational costs - Return only the cost
    // Time Complexity: O(M * N)    - traversing over str1 for each letter in str2

    // Space Complexity: O(2(min(M,N)))   - 2 vectors 
    
    // {As in the task_2 (DP Matrix), only last 2 rows were needed, previous one and current one to perform the calculations}
    // Time Comp can't be reduced further, as we need to traverse both the strings and compare characters of one with other.
    // 2 loops are needed, for such traversal.

    int levenshtein_Space_Optimized_Cost(const string& str1, const string& str2, int insert_Cost = 1, int delete_Cost = 1, int substitute_Cost = 1) {
        
        int m = str1.length();
        int n = str2.length();
        
        if (m == 0) return n * insert_Cost;
        if (n == 0) return m * delete_Cost;

        cout << "String 1: \"" << str1 << "\" (length: " << m << ")" << endl;
        cout << "String 2: \"" << str2 << "\" (length: " << n << ")" << endl;
        cout << "Costs - Insert: " << insert_Cost << ", Delete: " << delete_Cost << ", Substitute: " << substitute_Cost << endl;
        
        // str1 should be the shorter string for space optimization
        if (m > n) {
            cout << "\nSwapping strings for space optimization..." << endl;
            return levenshtein_Space_Optimized_Cost(str2, str1, delete_Cost, insert_Cost, substitute_Cost);
        }
        
        vector<int> prev(m + 1);
        vector<int> curr(m + 1, 0);
    
        cout << "Initializing base cases for deletions:" << endl;
        for (int i = 0; i <= m; i++) {
            prev[i] = i * delete_Cost;
        }
        
        cout << "prev[i] (cost of deleting 'i' characters)" << endl;
        print_Vector(prev, "Initial prev vector");
        
        for (int j = 1; j <= n; j++) {
            cout << "\n**** Processing column " << j << " (character '" << str2[j-1] << "') ****" << endl;
            
            curr[0] = j * insert_Cost;
            cout << "curr[0] = " << curr[0] << " (cost of inserting " << j << " characters)" << endl;
            
            for (int i = 1; i <= m; i++) {
                cout << "Comparing str1[" << (i-1) << "]='" << str1[i-1] << "' with str2[" << (j-1) << "]='" << str2[j-1] << "':";
                
                if (str1[i-1] == str2[j-1]) {
                    curr[i] = prev[i-1];
                    cout << "  MATCH! No cost added. curr[" << i << "] = prev[" << (i-1) << "] = " << curr[i] << endl;
                } else {
                    int substitute_OP = prev[i-1] + substitute_Cost;
                    int insert_OP = curr[i-1] + insert_Cost;
                    int del_OP = prev[i] + delete_Cost;
                    
                    cout << "  NO MATCH.";
                    
                    curr[i] = minimum_Cost(substitute_OP, insert_OP, del_OP);
                    cout << "    Minimum cost: " << curr[i];
                    
                    if (curr[i] == substitute_OP) cout << " (substitute_OP)";
                    else if (curr[i] == insert_OP) cout << " (insert_OP)";
                    else cout << " (delete)";
                    cout << endl;
                }
            }
            
            print_Vector(curr, "Current curr vector");
            
            prev = curr;
        }
        
        return prev[m];
    }

};

int main() {

    cout << "This is a C++ program to calculate the Weighted Levenshtein distance, with space optimisation and detailed logging.\n";
    Levenshtein_Distance temp;

    int num_Inputs;
    cout << "Enter the number of string pairs you want to process: ";
    cin >> num_Inputs;
    cout << "\n";

    for (int i = 1; i <= num_Inputs; i++) {
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
        
        int res = temp.levenshtein_Space_Optimized_Cost(str1, str2, insert_Cost, delete_Cost, replace_Cost);
        
        cout << "\n\nMinimum edit distance: " << res << endl;
        cout << "\n\n\n\n";
    }

    return 0;
}