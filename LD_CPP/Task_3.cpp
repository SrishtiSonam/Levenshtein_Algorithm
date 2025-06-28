#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <limits>

using namespace std;

class Security_Checker {
private:
    vector<string> trusted_Emails = {
        "hdfcbank.com", "onlinesbi.com", "icicibank.com", "axisbank.com",
        "gmail.com", "yahoo.com", "outlook.com", "protonmail.com",
        "amazon.com", "flipkart.com", "paytm.com", "google.com",
        "gov.in", "uidai.gov.in", "epfindia.gov.in", "rbi.org.in",
        "edu.in", "ac.in", "iitb.ac.in", "du.ac.in"
    };
    
    vector<string> previous_Passwords = {
        "@Bank1234", "Secure123!", "Password123!", "Love@Life99", "Work@Home1",
        "Login#2024", "Jan2023@Pass", "Complex@Pass123", "qwerty@123", "admin@pass1"
    };

    string to_Lower_Case(const string& str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }

    int levenshtein_Distance(const string& str1, const string& str2, int max_Distance = -1) {
        int m = str1.length();
        int n = str2.length();
        
        if (max_Distance == -1) {
            max_Distance = max(m, n) / 3;
        }
        
        if (abs(m - n) > max_Distance) {
            return max_Distance + 1;
        }
        
        if (m > n) {
            return levenshtein_Distance(str2, str1, max_Distance);
        }
        
        vector<int> prev(m + 1);
        vector<int> curr(m + 1);
        
        for (int i = 0; i <= m; i++) {
            prev[i] = i;
        }
        
        for (int j = 1; j <= n; j++) {
            curr[0] = j;
            int row_Min = curr[0];
            
            for (int i = 1; i <= m; i++) {
                if (str1[i-1] == str2[j-1]) {
                    curr[i] = prev[i-1];
                } else {
                    int substitute_OP = prev[i-1] + 1;
                    int insert_OP = curr[i-1] + 1;
                    int delete_Op = prev[i] + 1;
                    curr[i] = min({substitute_OP, insert_OP, delete_Op});
                }
                row_Min = min(row_Min, curr[i]);
            }
            
            if (row_Min > max_Distance) {
                return max_Distance + 1;
            }
            prev = curr;
        }
        return prev[m];
    }

    double calculate_Similarity(const string& str1, const string& str2) {
        int distance = levenshtein_Distance(str1, str2);
        int maxLen = max(str1.length(), str2.length());
        if (maxLen == 0) return 100.0;
        return (1.0 - ((double)distance / maxLen)) * 100.0;
    }

    void display_Result(const string& title, const string& message, const string& type) {
        
        if (type == "safe") {
            cout << "[SAFE] " << title << endl;
        } else if (type == "warning") {
            cout << "[WARNING] " << title << endl;
        } else {
            cout << "[UNVERIFIED] " << title << endl;
        }
        cout << endl;
        cout << message << endl;
        cout << endl;
    }

public:
    void check_Phishing() {
        string input;
        cout << "\n ***** ***** PHISHING DETECTION ***** ***** " << endl;
        cout << "Enter URL or Email to check: ";
        cin.ignore(); // Clear any leftover newline
        getline(cin, input);
        
        if (input.empty()) {
            display_Result("INPUT ERROR", "Please enter a URL or email address.", "unverified");
            return;
        }
        
        string input_Lower = to_Lower_Case(input);
        string best_Match;
        double best_Similarity = 0.0;
        
        for (auto trusted : trusted_Emails) {
            string trusted_Lower = to_Lower_Case(trusted);
            double similarity = calculate_Similarity(input_Lower, trusted_Lower);
            
            if (similarity > best_Similarity) {
                best_Similarity = similarity;
                best_Match = trusted;
            }
            
            if (similarity == 100.0) {
                display_Result("DOMAIN VERIFIED", "Exact match with trusted domain: " + trusted, "safe");
                return;
            }
            
            if (similarity >= 65.0) {
                string message = "Input: " + input + "\n" + 
                               "Similar to: " + trusted + " (" + to_string((int)(similarity * 10) / 10.0) + "%)\n" +
                               "This may be a PHISHING attempt!\n" +
                               "Verify the URL carefully before proceeding!";
                display_Result("POTENTIAL PHISHING DETECTED", message, "warning");
                return;
            }
        }
        
        string message = "Domain not found in trusted list.\n Closest match: " + best_Match + " (" + to_string((int)(best_Similarity * 10) / 10.0) + "%)\n" +
                        "Please verify this domain independently before using it.";
        display_Result("UNVERIFIED DOMAIN", message, "unverified");
    }

    void check_Password() {
        string new_Password;
        cout << "\nPASSWORD SIMILARITY CHECK" << endl;
        cout << "Enter new password: ";
        cin.ignore(); // Clear any leftover newline
        getline(cin, new_Password);
        
        if (new_Password.empty()) {
            display_Result("INPUT ERROR", "Please enter a password.", "unverified");
            return;
        }
        
        bool is_Similar = false;
        double highest_Similarity = 0.0;
        string most_Similar_Password;
        
        for (const auto& oldPwd : previous_Passwords) {
            double similarity = calculate_Similarity(new_Password, oldPwd);
            if (similarity > highest_Similarity) {
                highest_Similarity = similarity;
                most_Similar_Password = oldPwd;
            }
            if (similarity >= 70.0) {
                is_Similar = true;
            }
        }
        
        if (is_Similar) {
            string message = "This password is too similar to one of your previous passwords.\n\n Highest similarity detected: " + to_string((int)(highest_Similarity * 10) / 10.0) + "%\n\n" +
                           "RECOMMENDATION: Create a more unique password\n Different passwords for different accounts are suggested for safety.";
            display_Result("PASSWORD NOT RECOMMENDED", message, "warning");
        } else {
            string message = "This password is sufficiently different from all previous passwords.\n\n Highest similarity: " + to_string((int)(highest_Similarity * 10) / 10.0) + "%\n\n" +
                           "Good job! Different passwords for different accounts enhance security.";
            display_Result("PASSWORD APPROVED", message, "safe");
        }
    }

    void display_Menu() {
        cout << "\nENHANCED SECURITY CHECKER" << endl;
        cout << "Protect yourself from phishing and weak passwords" << endl;
        cout << "1. Check for Phishing (URL/Email)" << endl;
        cout << "2. Check Password Similarity" << endl;
        cout << "3. Exit" << endl;
        cout << "\nEnter your choice (1-3): ";
    }

    void run() {
        int choice;
        
        while (true) {
            display_Menu();
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "\nInvalid input! Please enter a number between 1-3." << endl;
                continue;
            }
            
            switch (choice) {
                case 1:
                    check_Phishing();
                    break;
                case 2:
                    check_Password();
                    break;
                case 3:
                    cout << "\nThank you for using Enhanced Security Checker!" << endl;
                    cout << "Stay safe online!" << endl;
                    return;
                default:
                    cout << "\nInvalid choice! Please select 1, 2, or 3." << endl;
            }
            
            cout << "\nPress Double Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }
};

int main() {
    Security_Checker checker;
    checker.run();
    return 0;
}