// Copyright 2022 NNTU-CS
#include "tree.h"

#include <iostream>
#include <iomanip>
#include <chrono>
#include <random>
#include <string>
#include <vector>


//  Преобразование вектора символов в строку (для удобства)
static std::string vec2str(const std::vector<char>& v) {
    return std::string(v.begin(), v.end());
}


//  Демонстрация работы с перестановками
void demo() {
    std::vector<char> symbols = {'1', '2', '3'};
    PMTree tree(symbols);

    std::cout << "=== Все перестановки для {1,2,3} ===\n";
    auto all = getAllPerms(tree);
    for (const auto& perm : all)
        std::cout << vec2str(perm) << ' ';
    std::cout << "\n\n";

    std::cout << "Сравнение getPerm1 и getPerm2:\n";
    for (int num = 1; num <= static_cast<int>(all.size()); ++num) {
        auto p1 = getPerm1(tree, num);
        auto p2 = getPerm2(tree, num);
        std::cout << num << ": " << vec2str(p1)
                  << " (getPerm1) | "
                  << vec2str(p2)
                  << " (getPerm2)\n";
    }
    std::cout << "\n";
}


//  Вычислительный эксперимент
void runExperiment() {
    std::cout << "Вычислительный эксперимент (зависимость времени от n)\n";
    std::cout << std::setw(4) << "n"
              << std::setw(12) << "t_all(ms)"
              << std::setw(12) << "t1(ms)"
              << std::setw(12) << "t2(us)\n";

    std::mt19937 rng(42);  // фиксированный seed

    for (int n = 2; n <= 10; ++n) {
        std::vector<char> input;
        for (int i = 0; i < n; ++i)
            input.push_back(static_cast<char>('a' + i));

        PMTree tree(input);
        int totalPerms = tree.getRoot()->subtreeSize;

        // 1. Время getAllPerms
        auto t0 = std::chrono::steady_clock::now();
        volatile auto all = getAllPerms(tree);
        auto t1 = std::chrono::steady_clock::now();
        double tAll =
            std::chrono::duration<double, std::milli>(t1 - t0).count();

        // 2. Время getPerm1 
        std::uniform_int_distribution<int> dist(1, totalPerms);
        int num = dist(rng);
        t0 = std::chrono::steady_clock::now();
        volatile auto perm1 = getPerm1(tree, num);
        t1 = std::chrono::steady_clock::now();
        double t1time =
            std::chrono::duration<double, std::milli>(t1 - t0).count();

        // 3. Время getPerm2 
        const int repeats = 5000;
        t0 = std::chrono::steady_clock::now();
        for (int r = 0; r < repeats; ++r) {
            num = dist(rng);
            volatile auto perm2 = getPerm2(tree, num);
        }
        t1 = std::chrono::steady_clock::now();
        double t2avg =
            std::chrono::duration<double, std::micro>(t1 - t0).count()
            / repeats;

        std::cout << std::setw(4) << n
                  << std::setw(12) << std::fixed
                  << std::setprecision(2) << tAll
                  << std::setw(12) << std::fixed
                  << std::setprecision(2) << t1time
                  << std::setw(12) << std::fixed
                  << std::setprecision(2) << t2avg
                  << '\n';
    }
}

int main() {
    demo();
    runExperiment();
    return 0;
}
