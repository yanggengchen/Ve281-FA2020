#include "hashtable_debug.hpp"
//#include "hashtable.hpp"
#include <iostream>
using namespace std;

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
    printTable(table);
    table.insert(727,379);
    table.insert(100,200);
    printTable(table);
}