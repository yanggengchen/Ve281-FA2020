#include "hashtable_debug.hpp"
//#include "hashtable.hpp"
#include <iostream>
using namespace std;
//using namespace HashTable;

void printTable(HashTable<int, int> &table) {
    cout << "print the hashTable" << endl;
    for (auto it = table.begin(); it != table.end(); it++) {
        cout << " "<<it->first << ", " << it->second << endl;
    }
    cout << "finish printing the hashTable" << endl << endl;
}

int main () {
    HashTable<int, int> table;
    table.insert(850,694);
    table.insert(855,379);
    printTable(table);
    cout << "--------gonna delete all items" << endl;
    auto it = table.begin();
    while (it != table.end()) {
        cout << "erase one item" << endl;
        it = table.erase(it);
    }
    //table.erase(it);
    cout << "expect nth. printed" << endl;
    printTable(table);
    
    table.insert(100,200);
    printTable(table);

    HashTable<int, int> table2;
    auto it2 = table2.begin();
    while (it2 != table2.end()) {
        it2 = table2.erase(it2);
    }
    printTable(table2);
}