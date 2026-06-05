// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <vector>

// Дерево вариантов для генерации перестановок символов
class PMTree {
 public:
    struct Node {
        char data;                      // символ в узле
        int  subtreeSize;               // число перестановок, порождаемых поддеревом
        std::vector<Node*> children;    // потомки текущего узла

        Node(char d, int sz) : data(d), subtreeSize(sz) {}
    };

    // Построение дерева по вектору символов (сортируются на месте)
    explicit PMTree(const std::vector<char>& input);

    ~PMTree();

    // Запрет копирования
    PMTree(const PMTree&) = delete;
    PMTree& operator=(const PMTree&) = delete;

    // Доступ к корню
    Node* getRoot() const { return root_; }

 private:
    Node* root_;

    // Рекурсивное удаление
    void clear(Node* node);

    // Построение поддерева для заданного набора оставшихся символов
    void buildSubtree(Node* parent, const std::vector<char>& symbolsLeft);

    // Вычисление факториала
    static int factorial(int n);
};

// Основные операции
std::vector<std::vector<char>> getAllPerms(const PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(const PMTree& tree, int num);

#endif  // INCLUDE_TREE_H_
