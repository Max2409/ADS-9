// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>

class PMTree {
public:
    struct Node {
        char data;
        int  subtreeSize;
        std::vector<Node*> children;
        Node(char d, int sz) : data(d), subtreeSize(sz) {}
    };

    explicit PMTree(const std::vector<char>& input);
    ~PMTree();

    PMTree(const PMTree&) = delete;
    PMTree& operator=(const PMTree&) = delete;

    Node* getRoot() const { return root_; }

private:
    Node* root_;

    void clear(Node* node);
    void buildSubtree(Node* parent, const std::vector<char>& symbolsLeft);
    static int factorial(int n);
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);

#endif

