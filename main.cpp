//
// Created by Gav on 27/04/2020.
//
#include "RBTree.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto *tree = new RBTree<int,int>();
    int nval = 31337;
//    map<int, int> another_tree;

    // Speed test
    for (int key = 0; key < 10; key++) {
        int k = 1 + rand() / ((RAND_MAX + 1u) / 123456);
//        another_tree.insert({k, val});
        tree->insert(key, &key);
        tree->printConnections();
    }
    for (int i = 10; i > 5; i--){
        cout << "DELETE: " << i << endl;
        tree->remove(i);
    }
    tree->printConnections();
    cout << "Tree: " << endl;
    tree->printStructure();
    return 0;
}

