//
// Created by sejapoe on 10/7/23.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshadow"

#include <algorithm>

#ifndef SEM2_5_H
#define SEM2_5_H


class AvlTree {
    constexpr static double e = 1e-6;

    struct Node {
        double value;
        Node *left;
        Node *right;
        Node *parent;
        int height = 0;

        Node(double value, Node *left, Node *right, Node *parent) : value(value), left(left), right(right),
                                                                    parent(parent) {}

        void recalculateHeight() {
            this->height = std::max((left != nullptr ? left->height : 0), (right != nullptr ? right->height : 0)) + 1;
        }

        [[nodiscard]] int bf() const { // NOLINT(*-no-recursion)
            return (left != nullptr ? left->height : 0) - (right != nullptr ? right->height : 0);
        }

        [[nodiscard]] Node *insert(double value) { // NOLINT(*-no-recursion)
            if (std::abs(value - this->value) < e) nullptr;
            if (value < this->value) {
                if (left != nullptr) {
                    return left->insert(value);
                } else {
                    left = new Node{value, nullptr, nullptr, this};
                    return left;
                }
            } else {
                if (right != nullptr) {
                    return right->insert(value);
                } else {
                    right = new Node{value, nullptr, nullptr, this};
                    return right;
                }
            }
        }

        void balance() { // NOLINT(*-no-recursion)
            recalculateHeight();
            int bf = this->bf();
            if (std::abs(bf) == 2) {

            }
            if (parent != nullptr) parent->balance();
        }

        virtual ~Node() {
            delete left;
            delete right;
        }
    };

    Node *root;

    bool insert(double value) {
        if (root == nullptr) {
            root = new Node{value, nullptr, nullptr, nullptr};
            return true;
        }

        auto t = root->insert(value);
        if (t == nullptr) return false;

        t->balance();
    }
};

#endif //SEM2_5_H

#pragma clang diagnostic pop