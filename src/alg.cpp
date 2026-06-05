// Copyright 2022 NNTU-CS
#include <algorithm>
#include <string>
#include <vector>

#include "tree.h"


//  Вспомогательные методы класса PMTree
PMTree::PMTree(const std::vector<char>& input) {
    std::vector<char> sorted = input;
    std::sort(sorted.begin(), sorted.end());
    root_ = new Node('\0', factorial(static_cast<int>(sorted.size())));
    buildSubtree(root_, sorted);
}

PMTree::~PMTree() {
    clear(root_);
}

void PMTree::clear(Node* node) {
    if (node == nullptr) return;
    for (Node* kid : node->children)
        clear(kid);
    delete node;
}

int PMTree::factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

void PMTree::buildSubtree(Node* parent,
                         const std::vector<char>& symbolsLeft) {
    if (symbolsLeft.empty()) return;

    int childPermCount =
        factorial(static_cast<int>(symbolsLeft.size()) - 1);

    for (char ch : symbolsLeft) {
        Node* kid = new Node(ch, childPermCount);
        parent->children.push_back(kid);

        std::vector<char> nextSymbols;
        for (char c : symbolsLeft)
            if (c != ch) nextSymbols.push_back(c);

        buildSubtree(kid, nextSymbols);
    }
}

//  Рекурсивный сбор всех перестановок из дерева
namespace {
void gatherPerms(PMTree::Node* node,
                 std::vector<char>& prefix,
                 std::vector<std::vector<char>>& out) {
    if (node->children.empty()) {
        out.push_back(prefix);
        return;
    }
    for (PMTree::Node* kid : node->children) {
        prefix.push_back(kid->data);
        gatherPerms(kid, prefix, out);
        prefix.pop_back();
    }
}
}  // namespace


//  getAllPerms – все перестановки в лексикографическом порядке
std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> allPermutations;
    PMTree::Node* root = tree.getRoot();
    if (root == nullptr) return allPermutations;

    for (PMTree::Node* firstChild : root->children) {
        std::vector<char> curPath;
        curPath.push_back(firstChild->data);
        gatherPerms(firstChild, curPath, allPermutations);
    }
    return allPermutations;
}

//  getPerm1 – медленный способ: генерируем все и выбираем
std::vector<char> getPerm1(PMTree& tree, int num) {
    auto all = getAllPerms(tree);
    if (num < 1 || num > static_cast<int>(all.size()))
        return {};
    return all[num - 1];
}

//  getPerm2 – быстрый способ: навигация по дереву
std::vector<char> getPerm2(const PMTree& tree, int num) {
    PMTree::Node* root = tree.getRoot();
    if (root == nullptr || num < 1 || num > root->subtreeSize)
        return {};

    std::vector<char> result;
    PMTree::Node* current = root;

    while (!current->children.empty()) {
        for (PMTree::Node* kid : current->children) {
            if (num <= kid->subtreeSize) {
                result.push_back(kid->data);
                current = kid;
                break;
            } else {
                num -= kid->subtreeSize;
            }
        }
    }
    return result;
}
