#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


struct Donor {
    string name;
    double donationAmount;
    string category;
};

void registerDonor() {
    Donor donor;
    cout << "Enter your name: ";
    cin.ignore(); 
    getline(cin, donor.name);
    cout << "Enter your donation amount: ";
    cin >> donor.donationAmount;
    
    cout << "Select Donation Category:\n";
    cout << "1. Education\n2. Health\n3. Food\n4. Shelter\n";
    int choice;
    cout << "Enter category number: ";
    cin >> choice;

    switch (choice) {
        case 1: donor.category = "Education"; break;
        case 2: donor.category = "Health"; break;
        case 3: donor.category = "Food"; break;
        case 4: donor.category = "Shelter"; break;
        default: 
            cout << "Invalid choice, defaulting to 'General'\n";
            donor.category = "General"; 
    }

    ofstream file("donations.txt", ios::app); 
    if (file.is_open()) {
        file << donor.name << " " << donor.donationAmount << " " << donor.category << endl;
        file.close();
        cout << "Thank you for your donation to " << donor.category << "!\n";
    } else {
        cout << "Error saving donation record.\n";
    }
}


void viewDonations() {
    ifstream file("donations.txt");
    if (!file) {
        cout << "No donation records found.\n";
        return;
    }

    string name, category;
    double amount;
    cout << "\nDonation History:\n";
    cout << "-----------------------------------\n";
    while (file >> name >> amount >> category) {
        cout << "Donor: " << name << " | Amount: " << amount << " | Category: " << category << endl;
    }
    file.close();
}


double getTotalBalance(string categoryFilter = "") {
    ifstream file("donations.txt");
    double total = 0.0, amount;
    string name, category;

    while (file >> name >> amount >> category) {
        if (categoryFilter.empty() || category == categoryFilter) {
            total += amount;
        }
    }
    file.close();
    return total;
}


void withdrawFunds() {
    cout << "Select Category to Withdraw From:\n";
    cout << "1. Education\n2. Health\n3. Food\n4. Shelter\n";
    int choice;
    cout << "Enter category number: ";
    cin >> choice;

    string category;
    switch (choice) {
        case 1: category = "Education"; break;
        case 2: category = "Health"; break;
        case 3: category = "Food"; break;
        case 4: category = "Shelter"; break;
        default:
            cout << "Invalid choice!\n";
            return;
    }

    double withdrawAmount;
    cout << "Enter amount to withdraw for " << category << ": ";
    cin >> withdrawAmount;

    double totalBalance = getTotalBalance(category);
    if (withdrawAmount > totalBalance) {
        cout << "Insufficient funds! Current balance in " << category << ": " << totalBalance << endl;
        return;
    }

   
    vector<Donor> donors;
    ifstream file("donations.txt");
    Donor donor;

    while (file >> donor.name >> donor.donationAmount >> donor.category) {
        donors.push_back(donor);
    }
    file.close();

    
    ofstream outFile("donations.txt");
    double remainingAmount = withdrawAmount;

    for (auto& d : donors) {
        if (remainingAmount > 0 && d.category == category) {
            if (d.donationAmount <= remainingAmount) {
                remainingAmount -= d.donationAmount;
                d.donationAmount = 0; 
            } else {
                d.donationAmount -= remainingAmount;
                remainingAmount = 0;
            }
        }
        outFile << d.name << " " << d.donationAmount << " " << d.category << endl;
    }

    outFile.close();
    cout << "Withdrawal successful! New balance for " << category << ": " << getTotalBalance(category) << endl;
}


int main() {
    int choice;
    do {
        cout << "\n----- Charity Donation Tracker -----\n";
        cout << "1. Register as Donor & Donate\n";
        cout << "2. View Donation History\n";
        cout << "3. Check Total Balance by Category\n";
        cout << "4. Withdraw Funds (Admin Only)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: registerDonor(); break;
            case 2: viewDonations(); break;
            case 3: {
                cout << "Select Category to View Balance:\n";
                cout << "1. Education\n2. Health\n3. Food\n4. Shelter\n";
                int catChoice;
                cout << "Enter category number: ";
                cin >> catChoice;

                string category;
                switch (catChoice) {
                    case 1: category = "Education"; break;
                    case 2: category = "Health"; break;
                    case 3: category = "Food"; break;
                    case 4: category = "Shelter"; break;
                    default:
                        cout << "Invalid choice!\n";
                        continue;
                }

                cout << "Total Balance for " << category << ": " << getTotalBalance(category) << endl;
                break;
            }
            case 4: withdrawFunds(); break;
            case 5: cout << "Exiting program. Thank you!\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}