#include <iostream>
#include <stack>

using namespace std;

//const bool RED = true;
//const bool BLACK = false;
template<typename T, typename T1>
class RBTree {
public:
    RBTree() = default;
private:
    typedef enum {
        RED, BLACK
    } nodeColor;

    typedef struct rbnode_ {
        T key;
        nodeColor color;
        T1 *value;
        struct rbnode_ *parent;
        struct rbnode_ *left;
        struct rbnode_ *right;
    } rbnode;

#define NIL &leaf
    rbnode leaf = {NULL, BLACK, nullptr, nullptr, NIL, NIL};
    rbnode *root = NIL;

    void rotLeft(rbnode *x) {
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

    void rotRight(rbnode *x) {
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

    void insert_fixup(rbnode *z) {
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

    void delete_fixup(rbnode *x) {
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

    void deleteNode(rbnode *z) {
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


public:
    void printConnections(rbnode *n) {
        stack<rbnode> s;
        s.push(*n);
        //cout << "---DUMP-----------------------------\n";
        while (!s.empty()) {
            rbnode x = s.top();
            //cout << x.key << (x.color == RED ? "r" : "b") << " -right-> " << x.right->key
                 //<< (x.right->color == RED ? "r" : "b") << endl;
            //cout << x.key << (x.color == RED ? "r" : "b") << " -left--> " << x.left->key
                 //<< (x.left->color == RED ? "r" : "b") << endl;
            s.pop();
            if (x.left != NIL)
                s.push(*x.left);
            if (x.right != NIL)
                s.push(*x.right);
        }
        //cout << "------------------------------------\n";
    }

    void remove(T key){
        rbnode *n = find(key);
        deleteNode(n);
    }

    void insert(T key, T1 *value) {
        rbnode *x = root, *y = nullptr;//, *x;
        //cout << "Root: " << x->key << endl;
        while (x != NIL) {
            //if (x->key == key) return x;
            y = x;
            x = key < x->key ? x->left : x->right;
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
            if (z->key < y->key) {
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

    rbnode *find(T key) {
        rbnode *x = root;
        while (x != NIL) {
            if (x->key == key) return x;
            x = key < x->key ? x->left : x->right;
        }
        return x;
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
//    rbnode * test = NIL;
//    cout << test->key << test->color << test->parent << " " << test->left->color << endl;
    int val = 123;
    auto *tree = new RBTree<int, int>();
    for (int key = 0; key < 10; key++) {
        if (key == 4){
            int nval = 31337;
            tree->insert(key, &nval);
            continue;
        }
        tree->insert(key, &val);
        //printConnections(root);
    }
    for (int i = 10; i > 5; i--){
        cout << "DELETE: " << i << endl;
        tree->remove(i);
    }
//    printConnections(root);
    cout << *(tree->find(4)->value) << endl;
//    cout << root->key << endl;
    return 0;
}
