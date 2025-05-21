#include <iostream>
#include <cstdlib>

using namespace std;

// Node for Expense Data (Singly Linked List)
struct DataNode {
    string fieldName;
    string value;
    DataNode* next;

    DataNode(string fName, string val) {
        fieldName = fName;
        value = val;
        next = NULL;
    }
};

// Node for Main Expense (Doubly Linked List)
struct ExpenseNode {
    string id;
    DataNode* dataHead;
    ExpenseNode* prev;
    ExpenseNode* next;

    ExpenseNode(string i) {
        id = i;
        dataHead = NULL;
        prev = next = NULL;
    }
};

class ExpenseTracker {
private:
    ExpenseNode* head;
    ExpenseNode* tail;
    int expenseCounter;

    // Manual string to double conversion
    double stringToDouble(const string& s) {
        double result = 0.0;
        double fraction = 1.0;
        bool decimalFound = false;
        
        for (int i = 0; s[i] != '\0'; i++) {
            if (s[i] == '.') {
                decimalFound = true;
                continue;
            }
            
            if (s[i] >= '0' && s[i] <= '9') {
                if (decimalFound) {
                    fraction *= 0.1;
                    result += (s[i] - '0') * fraction;
                } else {
                    result = result * 10 + (s[i] - '0');
                }
            }
        }
        return result;
    }

    // Manual number to string conversion
    string intToString(int num) {
        if (num == 0) return "0";
        
        char buffer[20];
        int i = 0;
        bool isNegative = false;
        
        if (num < 0) {
            isNegative = true;
            num = -num;
        }
        
        while (num > 0) {
            buffer[i++] = '0' + (num % 10);
            num /= 10;
        }
        
        string result;
        if (isNegative) {
            result += '-';
        }
        
        while (i > 0) {
            result += buffer[--i];
        }
        
        return result;
    }

public:
    ExpenseTracker() {
        head = tail = NULL;
        expenseCounter = 1;
    }

    ~ExpenseTracker() {
        ExpenseNode* current = head;
        while (current != NULL) {
            ExpenseNode* next = current->next;
            DataNode* dataCurrent = current->dataHead;
            while (dataCurrent != NULL) {
                DataNode* dataNext = dataCurrent->next;
                delete dataCurrent;
                dataCurrent = dataNext;
            }
            delete current;
            current = next;
        }
    }

    void addExpense() {
        string id = "EXP-" + intToString(expenseCounter++);
        ExpenseNode* newExpense = new ExpenseNode(id);
        
        string name, category, price, date;

        cout << "Enter Item Name: ";
        getline(cin, name);
        cout << "Enter Category: ";
        getline(cin, category);
        cout << "Enter Price: ";
        getline(cin, price);
        cout << "Enter Date (DD-MM-YYYY): ";
        getline(cin, date);

        // Create and link data nodes
        newExpense->dataHead = new DataNode("ID", id);
        newExpense->dataHead->next = new DataNode("Name", name);
        newExpense->dataHead->next->next = new DataNode("Category", category);
        newExpense->dataHead->next->next->next = new DataNode("Price", price);
        newExpense->dataHead->next->next->next->next = new DataNode("Date", date);

        // Add to main list
        if (head == NULL) {
            head = tail = newExpense;
        } else {
            tail->next = newExpense;
            newExpense->prev = tail;
            tail = newExpense;
        }

        cout << "\nExpense Added Successfully! ID: " << id << endl;
    }

    void editExpense() {
        string id;
        cout << "Enter Expense ID to Edit: ";
        getline(cin, id);

        ExpenseNode* expense = NULL;
        ExpenseNode* current = head;
        while (current != NULL) {
            if (current->id == id) {
                expense = current;
                break;
            }
            current = current->next;
        }

        if (expense == NULL) {
            cout << "Expense not found with ID: " << id << endl;
            return;
        }

        // Free existing data nodes (except ID)
        DataNode* dataCurrent = expense->dataHead->next;
        while (dataCurrent != NULL) {
            DataNode* temp = dataCurrent;
            dataCurrent = dataCurrent->next;
            delete temp;
        }

        string name, category, price, date;
        cout << "Enter New Item Name: ";
        getline(cin, name);
        cout << "Enter New Category: ";
        getline(cin, category);
        cout << "Enter New Price: ";
        getline(cin, price);
        cout << "Enter New Date (DD-MM-YYYY): ";
        getline(cin, date);

        // Recreate data nodes
        expense->dataHead->next = new DataNode("Name", name);
        expense->dataHead->next->next = new DataNode("Category", category);
        expense->dataHead->next->next->next = new DataNode("Price", price);
        expense->dataHead->next->next->next->next = new DataNode("Date", date);

        cout << "\nExpense Updated Successfully!\n";
    }

    void deleteExpense() {
        string id;
        cout << "Enter Expense ID to Delete: ";
        getline(cin, id);

        ExpenseNode* expense = NULL;
        ExpenseNode* current = head;
        while (current != NULL) {
            if (current->id == id) {
                expense = current;
                break;
            }
            current = current->next;
        }

        if (expense == NULL) {
            cout << "Expense not found with ID: " << id << endl;
            return;
        }

        // Update links
        if (expense->prev != NULL) {
            expense->prev->next = expense->next;
        } else {
            head = expense->next;
        }

        if (expense->next != NULL) {
            expense->next->prev = expense->prev;
        } else {
            tail = expense->prev;
        }

        // Free memory
        DataNode* dataCurrent = expense->dataHead;
        while (dataCurrent != NULL) {
            DataNode* temp = dataCurrent;
            dataCurrent = dataCurrent->next;
            delete temp;
        }
        delete expense;

        cout << "\nExpense Deleted Successfully!\n";
    }

    void viewAllExpenses() {
        if (head == NULL) {
            cout << "\nNo Expenses Found!\n";
            return;
        }

        cout << "\n====== ALL EXPENSES ======\n";
        ExpenseNode* current = head;
        while (current != NULL) {
            cout << "\n---------------------------------\n";
            DataNode* dataCurrent = current->dataHead;
            while (dataCurrent != NULL) {
                cout << dataCurrent->fieldName << ": " << dataCurrent->value << endl;
                dataCurrent = dataCurrent->next;
            }
            cout << "---------------------------------\n";
            current = current->next;
        }
    }

    void generateReport() {
        if (head == NULL) {
            cout << "\nNo Expenses Found to Generate Report!\n";
            return;
        }

        // Count expenses
        int count = 0;
        ExpenseNode* current = head;
        while (current != NULL) {
            count++;
            current = current->next;
        }

        // Create array for sorting
        ExpenseNode** expenses = new ExpenseNode*[count];
        current = head;
        for (int i = 0; i < count; i++) {
            expenses[i] = current;
            current = current->next;
        }

        // Bubble sort by price (descending)
        for (int i = 0; i < count-1; i++) {
            for (int j = 0; j < count-i-1; j++) {
                // Get prices
                string price1, price2;
                DataNode* data = expenses[j]->dataHead;
                while (data != NULL) {
                    if (data->fieldName == "Price") {
                        price1 = data->value;
                        break;
                    }
                    data = data->next;
                }
                
                data = expenses[j+1]->dataHead;
                while (data != NULL) {
                    if (data->fieldName == "Price") {
                        price2 = data->value;
                        break;
                    }
                    data = data->next;
                }
                
                // Compare
                if (stringToDouble(price1) < stringToDouble(price2)) {
                    ExpenseNode* temp = expenses[j];
                    expenses[j] = expenses[j+1];
                    expenses[j+1] = temp;
                }
            }
        }

        // Display report
        cout << "\n====== EXPENSE REPORT ======\n";
        cout << "Sorted by Price (Highest to Lowest):\n";
        for (int i = 0; i < count; i++) {
            cout << "\n---------------------------------\n";
            DataNode* dataCurrent = expenses[i]->dataHead;
            while (dataCurrent != NULL) {
                cout << dataCurrent->fieldName << ": " << dataCurrent->value << endl;
                dataCurrent = dataCurrent->next;
            }
            cout << "---------------------------------\n";
        }

        // Show most expensive
        cout << "\n>>> MOST EXPENSIVE ITEM <<<\n";
        DataNode* dataCurrent = expenses[0]->dataHead;
        while (dataCurrent != NULL) {
            cout << dataCurrent->fieldName << ": " << dataCurrent->value << endl;
            dataCurrent = dataCurrent->next;
        }

        delete[] expenses;
    }
};

void displayMainMenu() {
    cout << "\n===== EXPENSE TRACKER =====\n";
    cout << "1. Add New Expense\n";
    cout << "2. View All Expenses\n";
    cout << "3. Generate Expense Report\n";
    cout << "4. Exit\n";
    cout << "Enter your choice (1-4): ";
}

void displayExpenseMenu() {
    cout << "\n===== EXPENSE MANAGEMENT =====\n";
    cout << "1. Add New Expense\n";
    cout << "2. Edit Existing Expense\n";
    cout << "3. Delete Specific Expense\n";
    cout << "4. Back to Main Menu\n";
    cout << "Enter your choice (1-4): ";
}

int main() {
    ExpenseTracker tracker;
    int choice;

    do {
        displayMainMenu();
        cin >> choice;
        cin.ignore(); // Clear input buffer

        switch (choice) {
            case 1: {
                int subChoice;
                do {
                    displayExpenseMenu();
                    cin >> subChoice;
                    cin.ignore();

                    switch (subChoice) {
                        case 1:
                            tracker.addExpense();
                            break;
                        case 2:
                            tracker.editExpense();
                            break;
                        case 3:
                            tracker.deleteExpense();
                            break;
                        case 4:
                            break;
                        default:
                            cout << "Invalid choice! Try again.\n";
                    }
                } while (subChoice != 4);
                break;
            }
            case 2:
                tracker.viewAllExpenses();
                break;
            case 3:
                tracker.generateReport();
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}