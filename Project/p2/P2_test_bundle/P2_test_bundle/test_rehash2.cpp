//#include "hashtable_debug.hpp"
#include "hashtable.hpp"
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
    //table.contains(676);
    table.insert(426, 109);
    table[480] = 216;
    //table.contains(115);
    //table.contains(886);
    //table.contains(480);
    printTable(table);
    cout << "-------------------------------------" << endl;
    table.insert(374, 997); // first rehash
    cout << "-----------------first rehash done--------------------" << endl;
    table.insert(490,750);
    table[253]=228;
    printTable(table);
    cout << "-------------------gonna second rehash------------------" << endl;
    table[270]=258; // second rehash
    //table.insert(449, 100);
    printTable(table);
}