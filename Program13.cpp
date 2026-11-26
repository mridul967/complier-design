#include <iostream>
#include <vector>
#include <list>
#include <string>
using namespace std;
struct Symbol {
    string name;
    string dataType;
};

class SymbolTable {
    static const int TABLE_SIZE = 10;
    vector<list<Symbol>> table;
    int hashFunction(const string& name) const {
        unsigned long sum = 0;
        for (unsigned char ch : name) sum += ch;
        return static_cast<int>(sum % TABLE_SIZE);
    }

public:
    SymbolTable() : table(TABLE_SIZE) {}
    void insert(const string& name, const string& dataType) {
        int index = hashFunction(name);
        for (const auto& entry : table[index]) {
            if (entry.name == name) {
                cout << "Identifier already exists.\n";
                return;
            }
        }
        table[index].push_back({name, dataType});
        cout << "Identifier inserted successfully at bucket " << index << ".\n";
    }

    void lookup(const string& name) const {
        int index = hashFunction(name);
        for (const auto& entry : table[index]) {
            if (entry.name == name) {
                cout << "Identifier found: " << entry.name
                     << ", Data type: " << entry.dataType << '\n';
                return;
            }
        }
        cout << "Identifier not found.\n";
    }

    void remove(const string& name) {
        int index = hashFunction(name);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->name == name) {
                table[index].erase(it);
                cout << "Identifier deleted successfully.\n";
                return;
            }
        }
        cout << "Identifier not found.\n";
    }

    void display() const {
        cout << "\nHASH TABLE CONTENTS:\n";
        for (int i = 0; i < TABLE_SIZE; ++i) {
            cout << "Bucket " << i << ": ";
            if (table[i].empty()) {
                cout << "Empty";
            } else {
                for (const auto& entry : table[i])
                    cout << "[" << entry.name << ", " << entry.dataType << "] ";
            }
            cout << '\n';
        }
    }
};

int main() {
    SymbolTable symbols;
    int choice;
    string name, dataType;
    do {
        cout << "\n--- SYMBOL TABLE ---\n";
        cout << "1. Insert\n2. Lookup\n3. Delete\n4. Display\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter identifier name: ";
                cin >> name;
                cout << "Enter data type: ";
                cin >> dataType;
                symbols.insert(name, dataType);
                break;
            case 2:
                cout << "Enter identifier to search: ";
                cin >> name;
                symbols.lookup(name);
                break;
            case 3:
                cout << "Enter identifier to delete: ";
                cin >> name;
                symbols.remove(name);
                break;
            case 4:
                symbols.display();
                break;
            case 5:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 5);

    return 0;
}
