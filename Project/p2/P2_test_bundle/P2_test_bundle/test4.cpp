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
    cout <<"contains 27 now? "<< table.contains(27) << endl;
    cout << table[27] << endl;
    table[27] = 64;
    cout <<"contains 27 now? "<< table.contains(27) << endl;
    table[824] = 426;
    /*printTable(table);
    table[824] = 100;
    printTable(table);*/
    cout <<"contains 824 now? "<< table.contains(824) << endl;
}