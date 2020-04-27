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
    for (long long key = 0; key < 15; key++) {
        int k = 1 + rand() / ((RAND_MAX + 1u) / 123456);
//        another_tree.insert({k, val});
        tree->insert(k, &nval);
        tree->printConnections();
    }
    for (int i = 10; i > 5; i--){
        cout << "DELETE: " << i << endl;
        tree->remove(i);
    }
    tree->printConnections();
    return 0;
}

