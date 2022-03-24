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
    cout << table.insert(426, 851) << endl;
    cout << table.insert(682, 589)<< endl;
    cout << "gonna insert 22" << endl <<table.insert(22, 100) << endl;
    cout << "finished insert 22" << endl;
    printTable(table);
    cout << "main::contain(682) " << table.contains(682)<< endl << endl;
    cout << "main::erase(682) "<<table.erase(682)<< endl << endl;
    printTable(table);
    cout << "main::contain(682) "<<table.contains(682)<< endl;
}