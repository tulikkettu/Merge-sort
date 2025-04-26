#include <iostream>
using namespace std;

struct Node {
    double data;
    Node *parent;
    Node *left;
    Node *right;
    int color;
};
typedef Node *NodePtr;

class RedBlackTree {
private:
    NodePtr root;
    NodePtr TNULL;

    void insertFix(NodePtr k) {
        NodePtr u;//дядя
        while (k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {//если родитель— правый потомок
                u = k->parent->parent->left;//тогда дядя левый потомок
                if (u->color == 1) {//перекрашиваем дядю и отца, если они красные
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {//если дядя черный, делаем поворот
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            } else {//то же самое, если родитель левый
                u = k->parent->parent->right;

                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {//дошли до корня
                break;
            }
        }
        root->color = 0;
    }
    void printHelper(NodePtr root, string indent, bool last) {
        if (root != TNULL) {
            cout << indent;
            if (last) {//правый потомок
                cout << "R____";
                indent += "   ";
            } else {//левый потомок
                cout << "L____";
                indent += "|  ";
            }
			// Выбор цвета
            if (root->color) {
                cout << "\033[31m" << root->data << "\033[0m\n";  // Красный
            } else {
                cout << root->data << "\n";  // Чёрный (или обычный)
            }
            //вывод левого и правого поддерева
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }
public:
    RedBlackTree() {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }
    void leftRotate(NodePtr x) {
        NodePtr y = x->right;
        x->right = y->left;//левое поддерево y теперь правое поддерево x
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) { // если x был корнем
            this->root = y;
        } else if (x == x->parent->left) {// если x был левым потомком
            x->parent->left = y;
        } else {// если x был правым потомком
            x->parent->right = y;
        }
        y->left = x; // x — левый потомок y
        x->parent = y; // y — родитель x
    }
    void rightRotate(NodePtr x) {
        NodePtr y = x->left;
        x->left = y->right; //правое поддерево y теперь леввое поддерево x
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {// если x был корнем
            this->root = y;
        } else if (x == x->parent->right) {// если x был правым потомком
            x->parent->right = y;
        } else {// если x был левым потомком
            x->parent->left = y;
        }
        y->right = x;// x — левый потомок y
        x->parent = y;// y — родитель x
    }
    void insert(double key) {
        NodePtr node = new Node;
        node->parent = nullptr;
        node->data = key;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;

        NodePtr y = nullptr; //вспомогательный узел
        NodePtr x = this->root;//указатель для поиска места
		//поиск места
        while (x != TNULL) {
            y = x;
            if (node->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }
        node->parent = y;//устанавливаем родителя
        if (y == nullptr) {//если дерево было пустым
            root = node;
        } else if (node->data < y->data) {//вставляем влево или право
            y->left = node;
        } else {
            y->right = node;
        }
        if (node->parent == nullptr) {//ничего больше нет
            node->color = 0;
            return;
        }
        if (node->parent->parent == nullptr) {//нет деда
            return;
        }
		//восстанавливаем свойства rbt
        insertFix(node);
    }
    void printTree() {
        if (root) {
            printHelper(this->root, "", true);
        }
    }
};