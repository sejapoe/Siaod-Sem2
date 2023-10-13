//
// Created by sejapoe on 10/7/23.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"

#include <algorithm>
#include <iostream>
#include <functional>

#ifndef SEM2_5_H
#define SEM2_5_H


class AvlTree {
    constexpr static double e = 1e-6;

    struct Node {
        double value;
        Node *left = nullptr;
        Node *right = nullptr;
        int height = 0;
    };

    Node *root;


    Node *insert(double value, Node *node) {
        if (node == nullptr) {
            node = new Node{value};
            return node;
        }

        if (value < node->value) {
            node->left = insert(value, node->left);
            if (height(node->left) - height(node->right) == 2) {
                if (value < node->left->value) {
                    node = singleRightRotate(node);
                } else {
                    node = doubleRightRotate(node);
                }
            }
        } else if (value > node->value) {
            node->right = insert(value, node->right);
            if (height(node->right) - height(node->left) == 2) {
                if (value > node->right->value) {
                    node = singleLeftRotate(node);
                } else {
                    node = doubleLeftRotate(node);
                }
            }
        }
        node->height = std::max(height(node->left), height(node->right)) + 1;
        return node;
    }


    Node *remove(double value, Node *node) {
        Node *temp;

        if (node == nullptr) return nullptr;

        if (std::abs(value - node->value) < e) {
            if (node->left && node->right) {
                temp = findMin(node->right);
                node->value = temp->value;
                node->right = remove(node->value, node->right);
            } else {
                temp = node;
                if (node->left == nullptr) node = node->right;
                else if (node->right == nullptr) node = node->left;
                delete temp;
            }
        } else {
            if (value < node->value) node->left = remove(value, node->left);
            else if (value > node->value) node->right = remove(value, node->right);
        }

        if (node == nullptr) return nullptr;

        node->height = std::max(height(node->left), height(node->right)) + 1;

        if (height(node->left) - height(node->right) == 2) {
            if (height(node->left->left) - height(node->left->right) == 1)
                return singleLeftRotate(node);
            else
                return doubleLeftRotate(node);
        } else if (height(node->right) - height(node->left) == 2) {
            if (height(node->right->right) - height(node->right->left) == 1)
                return singleRightRotate(node);
            else
                return doubleRightRotate(node);
        }

        return node;
    }


    void clear(Node *node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }


    Node *singleRightRotate(Node *t) {
        Node *u = t->left;
        t->left = u->right;
        u->right = t;
        t->height = std::max(height(t->left), height(t->right)) + 1;
        u->height = std::max(height(u->left), t->height) + 1;
        return u;
    }

    Node *singleLeftRotate(Node *&t) {
        Node *u = t->right;
        t->right = u->left;
        u->left = t;
        t->height = std::max(height(t->left), height(t->right)) + 1;
        u->height = std::max(height(t->right), t->height) + 1;
        return u;
    }

    Node *doubleLeftRotate(Node *&t) {
        t->right = singleRightRotate(t->right);
        return singleLeftRotate(t);
    }

    Node *doubleRightRotate(Node *&t) {
        t->left = singleLeftRotate(t->left);
        return singleRightRotate(t);
    }

    Node *findMin(Node *t) {
        if (t == nullptr)
            return nullptr;
        else if (t->left == nullptr)
            return t;
        else
            return findMin(t->left);
    }

    Node *findMax(Node *t) {
        if (t == nullptr)
            return nullptr;
        else if (t->right == nullptr)
            return t;
        else
            return findMax(t->right);
    }

    int height(Node *t) {
        return (t == nullptr ? -1 : t->height);
    }

    void inorder(Node *t, const std::function<void (double)>& lambda) {
        if (t == nullptr)
            return;
        inorder(t->left, lambda);
        lambda(t->value);
        inorder(t->right, lambda);
    }

    void postOrder(Node *t, const std::function<void (double)>& lambda) {
        if (t == nullptr)
            return;
        inorder(t->left, lambda);
        inorder(t->right, lambda);
        lambda(t->value);
    }
public:
    AvlTree() : root(nullptr) {}

    // вставка элементов
    void insert(double value) {
        Node *pNode = insert(value, root);
        root = pNode;
    }

    void remove(double value) {
        root = remove(value, root);
    }

    void clear() {
        clear(root);
    }

    void display() {
        inorder([](double x){
            std::cout << x << " ";
        });
    }

    // сумма значений
    double sum() {
        double sum = 0;
        inorder([&sum](double x) { sum += x; });
        return sum;
    }

    // среднее арифметическое всех узлов
    double avg() {
        double count = 0;
        inorder([&count](double x) { count++; });
        return sum() / count;
    }

    // симметричный обход
    void inorder(const std::function<void (double)>& lambda) {
        inorder(root, lambda);
    }

    // обратный обход
    void postOrder(const std::function<void (double)>& lambda) {
        postOrder(root, lambda);
    }

};

void task5() {
    AvlTree t;
    t.insert(20);
    t.insert(25);
    t.insert(15);
    t.insert(10);
    t.insert(30);
    t.insert(5);
    t.insert(35);
    t.insert(67);
    t.insert(43);
    t.insert(21);
    t.insert(10);
    t.insert(89);
    t.insert(38);
    t.insert(69);
    t.display();
    std::cout << "\n";
    t.remove(5);
    t.remove(35);
    t.remove(65);
    t.remove(89);
    t.remove(43);
    t.remove(88);
    t.remove(20);
    t.remove(38);
    t.display();
    std::cout << "\n";
    std::cout << t.sum() << " " << t.avg() << "\n";
}

#endif //SEM2_5_H

#pragma clang diagnostic pop