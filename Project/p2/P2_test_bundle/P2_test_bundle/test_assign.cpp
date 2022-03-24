//#include "hashtable_debug.hpp"
#include "hashtable.hpp"
#include <iostream>
using namespace std;

void printTable(HashTable<int, int> &table) {
    cout << "print the hashTable" << endl;
    for (auto it = table.begin(); it != table.end(); it++) {
        cout << "  "<<it->first << ", " << it->second << endl;
    }
    cout << "finish printing the hashTable" << endl << endl;
}

int main () {
    HashTable<int, int> table, table2;
    table.insert(4,414);
    table.insert(3, 123);
    //table.insert(10, 144);
    printTable(table);
    cout << ">>table1 size is "<< table.size()<<endl;
    cout << ">>table1 bucketSize is " << table.bucketSize() << endl;
    cout << endl;

    table2 = table;
    //table2.insert(9,379);   
    //table2[9] = 20;
    printTable(table2);
    cout << ">>table2 size is "<< table2.size()<<endl;
    cout << ">>table2 bucketSize is " << table2.bucketSize() << endl;
    cout << endl;
    cout << "-----------------------------------table 2----------------------------" << endl;
    table2.insert(8,20); // gonna rehash()
    
    //printTable(table);
    printTable(table2);
    cout << ">> table2 size is "<< table2.size()<<endl;
    cout << ">> table2 bucketSize is " << table2.bucketSize() << endl;
    cout << "-------------table3 initialize-----------" << endl;
    HashTable<int, int> table3(table2);
    printTable(table3);
    cout << ">>table3 size is "<< table3.size()<<endl;
    cout << ">>table3 bucketSize is " << table3.bucketSize() << endl;
    cout << endl;

    cout << "-------------table2 = table2 again-----------" << endl;
    table2 = table2;
    printTable(table2);
    cout << ">>table2 size is "<< table2.size()<<endl;
    cout << ">>table2 bucketSize is " << table2.bucketSize() << endl;
    cout << endl;
    cout << "-------------table2 = table3 again-----------" << endl;
    table2 = table3;
    printTable(table2);
    cout << ">>table2 size is "<< table2.size()<<endl;
    cout << ">>table2 bucketSize is " << table2.bucketSize() << endl;
    cout << endl;
}