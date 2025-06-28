#include<iostream>
#include<vector>
#include<string>
using namespace std;

struct LA_Result {
    int cost;
    vector<string> operations;
    LA_Result() : cost(0), operations({}) {}
    LA_Result(int c, vector<string> ops) : cost(c), operations(ops) {}
};

int minimum_cost(int a, int b, int c) {
    return min(a, min(b, c));
}

// Recursion Method - Return only the cost
// Time Complexity: O(3^N)
// Space Complexity: O(N) {stack}
int levenshtein_Cost_Only(const string &str1, const string &str2, int i, int j) {
    
    if (i == str1.length())         return str2.length() - j;           // O(1)
    if (j == str2.length())         return str1.length() - i;           // O(1)

    if (str1[i] == str2[j]) {              // O(1)
        return levenshtein_Cost_Only(str1, str2, i + 1, j + 1);
    }

    int insertCost = 1 + levenshtein_Cost_Only(str1, str2, i, j + 1);               // O(3^N)
    int deleteCost = 1 + levenshtein_Cost_Only(str1, str2, i + 1, j);               // O(3^N)
    int replaceCost = 1 + levenshtein_Cost_Only(str1, str2, i + 1, j + 1);          // O(3^N)

    return minimum_cost(insertCost, deleteCost, replaceCost);
}

// Recursion Method - Return cost + operations
// Time Complexity: O(3^N)
// Space Complexity: O(N^2) {stack + operations}
LA_Result levenshtein_Cost_And_Operation(string str1, string str2, int i, int j) {

    if (i == str1.length()) {
        vector<string> ops;
        for (int k = j; k < str2.length(); k++) {
            ops.push_back("Insert '" + string(1, str2[k]) + "'");
        }
        return LA_Result(str2.length() - j, ops);
    }

    if (j == str2.length()) {
        vector<string> ops;
        for (int k = i; k < str1.length(); k++) {
            ops.push_back("Delete '" + string(1, str1[k]) + "'");
        }
        return LA_Result(str1.length() - i, ops);
    }

    if (str1[i] == str2[j]) {
        LA_Result res = levenshtein_Cost_And_Operation(str1, str2, i + 1, j + 1);   
        res.operations.insert(res.operations.begin(), "Match '" + string(1, str1[i]) + "'");
        return res;
    }

    LA_Result insert_Op = levenshtein_Cost_And_Operation(str1, str2, i, j + 1);
    insert_Op.cost += 1;
    insert_Op.operations.insert(insert_Op.operations.begin(), "Insert '" + string(1, str2[j]) + "'");

    LA_Result delete_Op = levenshtein_Cost_And_Operation(str1, str2, i + 1, j);
    delete_Op.cost += 1;
    delete_Op.operations.insert(delete_Op.operations.begin(), "Delete '" + string(1, str1[i]) + "'");

    LA_Result replace_Op = levenshtein_Cost_And_Operation(str1, str2, i + 1, j + 1);
    replace_Op.cost += 1;
    replace_Op.operations.insert(replace_Op.operations.begin(), "Replace '" + string(1, str1[i]) + "' with '" + string(1, str2[j]) + "'");

    if (insert_Op.cost <= delete_Op.cost && insert_Op.cost <= replace_Op.cost)
        return insert_Op;
    else if (delete_Op.cost <= insert_Op.cost && delete_Op.cost <= replace_Op.cost)
        return delete_Op;
    else
        return replace_Op;
}

int EditDistance (string s1, string s2) {

    int flag = 0;
    cout << "Do you want detailed operations? (yes=1 / no=1)" << endl;
    cout << "Enter your choice (1/0): ";
    cin >> flag;

    if (flag == 1) {
        LA_Result result = levenshtein_Cost_And_Operation(s1, s2, 0, 0);
        cout << "Operations to convert '" << s1 << "' to '" << s2 << "':\n";
        for (auto op : result.operations)
            cout << " - " << op << "\n";
        cout << "\n";
        return result.cost;
    }

    return levenshtein_Cost_Only(s1, s2, 0, 0);      
}

int main() {
    cout << "This is a C++ program to calculate the Levenshtein distance and show operations.\n";
    cout << "All operations (insert, delete, replace) have the same cost of 1.\n\n";

    int numInputs;
    cout << "Enter the number of string pairs you want to process: ";
    cin >> numInputs;
    cout << "\n";

    for (int i = 1; i <= numInputs; i++) {
        string str1, str2;
        
        cout << " ***** ***** ***** ***** Pair " << i << " ***** ***** ***** ***** \n" << endl;
        cout << "Enter the first string (The original string): ";
        cin >> str1;
        cout << "Enter the second string (String to convert to): ";
        cin >> str2;
        cout << "\nResults for { Pair " << i << " }\n";
        int res = EditDistance(str1, str2);
        cout << "Levenshtein Distance for Pair " << i << ": " << res << endl;
        cout << "\n\n\n\n";
    }

    return 0;
}