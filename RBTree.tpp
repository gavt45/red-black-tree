#include "RBTree.h"

/**
 * Default constructor
 * @tparam keyT
 * @tparam valueT
 */
template<typename keyT, typename valueT>
RBTree<keyT, valueT>::RBTree() {
    leaf.key = NULL;
}

/**
 * Lower comparator for keyT's
 * @tparam keyT
 * @tparam valueT
 * @param a
 * @param b
 * @return true if a < b
 */
template<typename keyT, typename valueT>
bool RBTree<keyT,valueT>::compL(keyT a, keyT b) {
    return a < b;
}

/**
 * Equality comparator
 * @tparam keyT
 * @tparam valueT
 * @param a
 * @param b
 * @return true if a == b
 */
template<typename keyT, typename valueT>
bool RBTree<keyT,valueT>::compEQ(keyT a, keyT b) {
    return a == b;
}

/**
 * Internal func.
 * Rotates tree left around node x
 * @tparam keyT
 * @tparam valueT
 * @param x
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::rotLeft(rbnode *x) {
    rbnode *y = x->right;

    x->right = y->left;
    if (y->left != NIL)
        y->left->parent = x;

    if (y != NIL) y->parent = x->parent;
    if (x->parent) {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        root = y;
    }

    if (x != NIL) x->parent = y;
    y->left = x;
}

/**
 * Internal func.
 * Rotates tree right around node x
 * @tparam keyT
 * @tparam valueT
 * @param x
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::rotRight(rbnode *x) {
    rbnode *y = x->left;

    x->left = y->right;
    if (y->right != NIL)
        y->right->parent = x;

    if (y != NIL) y->parent = x->parent; /* при этом, возможно, y становится корнем дерева */
    if (x->parent) {
        if (x->parent->left == x)
            x->parent->left = y;
        else
            x->parent->right = y;
    } else {
        root = y;
    }

    if (x != NIL) x->parent = y;
    y->right = x;
}

/**
 * Fixup tree after insertion.
 * Checks rb-tree properties
 * @tparam keyT
 * @tparam valueT
 * @param z node which was been inserted
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::insert_fixup(rbnode *z) {
    if (z == root) {
        z->color = BLACK;
        return;
    } else if (z->parent == root) {
        root->color = BLACK;
        return;
    }
    while (z != root && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            auto *y = z->parent->parent->right;
            if (y->color == RED) {
                // Case #1
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    rotLeft(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotRight(z->parent->parent);
            }
        } else {
            auto *y = z->parent->parent->left;
            if (y->color == RED) {
                // Case #1
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rotRight(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotLeft(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

/**
 * Fixup tree after deletion.
 * Checks rb-tree properties
 * @tparam keyT
 * @tparam valueT
 * @param z node which was been deleted
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::delete_fixup(rbnode *x) {
    while (x != root && x->color == BLACK) {
        if (x == x->parent->left) {
            rbnode *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotLeft(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotRight(w);
                    w = x->parent->right;
                }
                x->color = x->parent->color;
                x->parent->color = BLACK;
                x->right->color = BLACK;
                rotLeft(x->parent);
                x = root;
            }
        } else {
            rbnode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rotRight(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotLeft(w);
                    w = x->parent->left;
                }
                x->color = x->parent->color;
                x->parent->color = BLACK;
                x->left->color = BLACK;
                rotRight(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

/**
 * Deletes node z from tree
 * @tparam keyT
 * @tparam valueT
 * @param z Node to delete
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::deleteNode(rbnode *z) {
    rbnode *x, *y;

    if (!z || z == NIL) return;

    if (z->left == NIL || z->right == NIL) {
        y = z;
    } else {
        y = z->right;
        while (y->left != NIL) y = y->left;
    }

    if (y->left != NIL)
        x = y->left;
    else
        x = y->right;

    x->parent = y->parent;
    if (y->parent) {
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    } else {
        root = x;
    }
    if (y != z) z->value = y->value;

    if (y->color == BLACK)
        delete_fixup(x);
    free(y);
}

/**
 * Debug function.
 * Just prints tree state.
 * @tparam keyT
 * @tparam valueT
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::printConnections() {
    rbnode *n = root;
    stack<rbnode> s;
    s.push(*n);
    cout << "---DUMP-----------------------------\n";
    while (!s.empty()) {
        rbnode x = s.top();
        cout << x.key << (x.color == RED ? "r" : "b") << " -right-> " << x.right->key
             << (x.right->color == RED ? "r" : "b") << endl;
        cout << x.key << (x.color == RED ? "r" : "b") << " -left--> " << x.left->key
             << (x.left->color == RED ? "r" : "b") << endl;
        s.pop();
        if (x.left != NIL)
            s.push(*x.left);
        if (x.right != NIL)
            s.push(*x.right);
    }
    cout << "------------------------------------\n";
}

/**
 * Removes node with key `key` from tree.
 * @tparam keyT
 * @tparam valueT
 * @param key Key of the node to be deleted
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::remove(keyT key) {
    rbnode *n = find(key);
    deleteNode(n);
}

/**
 * Inserts node with key `key` and value `value` to the tree
 * @tparam keyT
 * @tparam valueT
 * @param key
 * @param value
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::insert(keyT key, valueT *value) {
    rbnode *x = root, *y = nullptr;//, *x;
    //cout << "Root: " << x->key << endl;
    while (x != NIL) {
        //if (x->key == key) return x;
        y = x;
        x = compL(key, x->key) ? x->left : x->right;
    }

    // Create new node
    auto *z = new rbnode;
    z->key = key;
    z->color = RED;
    z->value = value;
    z->parent = y;
    z->left = NIL;
    z->right = NIL;

    //cout << "Y in insert: " << ((y != nullptr) ? y->key : -1) << endl;
    //cout << "Z in insert: " << z->key << endl;

    if (y) {
        if (compL(z->key, y->key)) {
            //cout << "Insert to left" << endl;
            y->left = z;
        } else {
            //cout << "Insert to right" << endl;
            y->right = z;
        }
    } else {
        root = z;
        z->parent = nullptr;
        //cout << "Root set to " << z->key << endl;
        return;
    }
    //cout << "Dump before fixup: " << endl;
    //printConnections(root);
    insert_fixup(z);
    //cout << "Dump after: " << endl;
}

/**
 * Find node in tree by key
 * @tparam keyT
 * @tparam valueT
 * @param key
 * @return node
 */
template<typename keyT, typename valueT>
typename RBTree<keyT,valueT>::rbnode * RBTree<keyT,valueT>::find(keyT key) {
    rbnode *x = root;
    while (x != NIL) {
        if (compEQ(x->key, key)) return x;
        x = compL(key, x->key) ? x->left : x->right;
    }
    return x;
}

/**
 * Get value by key.
 */
template<typename keyT, typename valueT>
valueT RBTree<keyT,valueT>::get(keyT key) {
    return find(key)->data;
}

/**
 * Internal method for printStructure
 * @tparam keyT
 * @tparam valueT
 * @param prefix
 * @param node
 * @param isLeft
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::printRBT(const std::string& prefix, const RBTree<keyT,valueT>::rbnode * node, bool isLeft)
{
    if( node != NIL )
    {
        cout << prefix;

        cout << (isLeft ? "├──" : "└──" );

        // print the value of the node
        if (node->color == RED)
            cout << "\x1B[31m" << node->key << "\033[0m" << endl;
        else
            cout << node->key << endl;
        // enter the next tree level - left and right branch
        printRBT( prefix + (isLeft ? "│   " : "    "), node->left, true);
        printRBT( prefix + (isLeft ? "│   " : "    "), node->right, false);
    }
}

/**
 * Prints tree structure like `tree` command
 * @tparam keyT
 * @tparam valueT
 */
template<typename keyT, typename valueT>
void RBTree<keyT,valueT>::printStructure() {
    const RBTree<keyT,valueT>::rbnode* node = root;
    printRBT("", node, false);
}
