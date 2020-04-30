//
// Created by Gav on 27/04/2020.
//

#ifndef RED_BLACK_TREE_RBTREE_H
#define RED_BLACK_TREE_RBTREE_H
#include <iostream>
#include <stack>
#include <map>

#define NIL &leaf

using namespace std;

template<typename keyT, typename valueT>
class RBTree {

    typedef enum {
        RED, BLACK
    } rbNodeColor;

    typedef struct rbnode_ {
        keyT key;
        rbNodeColor color;
        valueT *value;
        struct rbnode_ *parent;
        struct rbnode_ *left;
        struct rbnode_ *right;
    } rbnode;

    rbnode leaf = {NULL, BLACK, nullptr, nullptr, NIL, NIL};
    rbnode *root = NIL;

public:
    // Constructor
    RBTree();

    // Comparators
    bool compL(keyT a, keyT b);
    bool compEQ(keyT a, keyT b);

    // Main tree operations
    void remove(keyT key);
    void insert(keyT key, valueT *value);
    valueT get(keyT key);

    // Debug function. Just prints all connections to stdout.
    void printConnections();
    void printStructure();

private:
    // Internal functions

    // Tree rotation
    void rotLeft(rbnode *x);
    void rotRight(rbnode *x);

    // 'Fixupers'
    void insert_fixup(rbnode *z);
    void delete_fixup(rbnode *x);

    // Main tree manipulations
    void deleteNode(rbnode *z);
    rbnode * find(keyT key);

    // Debug funcs
    void printRBT(const std::string& prefix, const rbnode * node, bool isLeft);

};

#include "RBTree.tpp"

#endif RED_BLACK_TREE_RBTREE_H