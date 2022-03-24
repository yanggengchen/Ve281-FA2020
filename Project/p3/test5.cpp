//#include "kdtree.hpp"
#include "kdtree_debug.hpp"
#include <tuple>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <iostream>
using namespace std;

typedef std::tuple<int, int, int> Key;
typedef KDTree<Key, int> Tree;
typedef std::pair<Key, int> t_data;

void insert_data(Tree &tree, int key0, int key1, int key2, int value) {
    Key tempKey(key0,key1,key2);
    tree.insert(tempKey, value);
}

void drawTree(Tree &tree) {
    cout << ">>>drawTree::tree size is " << tree.size() << "==========="<<endl;
    for (auto &item : tree) {
        cout << item.second << endl;
    }
    cout << "drawTree::finish" << endl;
    cout << endl;
}

void find_min_max(Tree &tree) {
    cout << "==========test findMin & findMax" << endl;
    auto min0 = tree.findMin(0);
    cout << "min0 = " << min0.node->value() << endl;
    auto min1 = tree.findMin(1);
    cout << "min1 = " << min1.node->value() << endl;
    auto max0 = tree.findMax(0);
    cout << "max0 = " << max0.node->value() << endl;
    auto max1 = tree.findMax(1);
    cout <<  "max1 = " << max1.node->value() << endl;
}

void insert_tree(Tree &tree) {
    cout << "==========initialization" << endl;
    insert_data(tree,0, 0, 0, 1);
    insert_data(tree,-1, 2, -4, 2);
    insert_data(tree,1, -1, -10, 3);
    insert_data(tree,3, -2, 3, 4);
    insert_data(tree,4, -3, 2, 5);
    insert_data(tree,-2, 4, 9, 6);
    insert_data(tree,-3, 1, 1, 7);
    insert_data(tree,-6, -10, -7, 8);
    insert_data(tree,1, -1, -10, 100);
    insert_data(tree,16, 23, -23, 50);
    insert_data(tree,-23, 32, 34, 70);
    insert_data(tree,-11, 12, 31, 18);
    insert_data(tree,49, -11, -24, 20);
    insert_data(tree,33, 44, 70, 80);
    insert_data(tree,-100, -100, -100, -200);
    drawTree(tree);
}

void test_find(Tree &tree, Key &key) {
    cout << "==========test find" << endl;
    auto it3 = tree.find(key);
    cout << it3.node->value() << endl;
}



void test_copy_constructor(Tree &tree) {
    cout << "==========test copy constructor" << endl;
    Tree tree2(tree);
    drawTree(tree2);
    
}

void test_operator_equal(Tree &tree) {
    cout << "==========test operator=" << endl;
    Tree tree3 = tree;
    drawTree(tree3);
    cout << "==========special case: A = A" << endl;
    tree = tree;
    drawTree(tree);
}

/*void test_vector_constructor() {
    Key a1(0,0), a2(0,0), a3(-1, 2), a4(1, -1);
    t_data d1(a1, 1), d2(a2, 2), d3(a3, 3), d4(a4, 4);
    vector<t_data> v_data;
    v_data.push_back(d1);
    v_data.push_back(d2);
    v_data.push_back(d3);
    v_data.push_back(d4);
    cout << "==========vector initialization" << endl;
    Tree tree_v(v_data);
    drawTree(tree_v);
}*/

void test_erase(Tree &tree, Key &key) {
    cout << "==========test erase ==========" << endl;
    cout << "tree size before erase is " << tree.size() << endl;
    //cout << "erase take place? "<< << endl;
    tree.erase(key);
    find_min_max(tree);
    drawTree(tree);
    //cout << "tree size after erase is " << tree.size() << endl;
    
    cout << "==========end  of test erase ==========" << endl;
}

/*void test_general(Tree &tree, Key &key) {
    return;
}*/

void test_iterator(Tree &tree) {
    cout << "==========test iterator ==========" << endl;
    Tree::Iterator it = tree.begin();
    cout << it->second << endl;
    it++;
    cout << it->second << endl;
    tree.erase(it);
    drawTree(tree);


    cout << "==========end test iterator ==========" << endl;
}

int main() {
    Key a1(0,0,0), a2(0,0,0), a3(-1, 2,0), a4(1, -1,0);
    t_data d1(a1, 1), d2(a2, 2), d3(a3, 3), d4(a4, 4);
    vector<t_data> v_data;
    v_data.push_back(d1);
    v_data.push_back(d2);
    v_data.push_back(d3);
    v_data.push_back(d4);
    cout << "vector size is " << v_data.size() << endl;
    cout << "==========vector initialization" << endl;
    Tree tree_v(v_data);
    drawTree(tree_v);

    insert_tree(tree_v);
}
