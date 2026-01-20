//
// Created by User on 1/20/2026.
//

#ifndef OOP_TEMPLATES_H
#define OOP_TEMPLATES_H

#include <vector>
#include <algorithm>

template <typename T, typename Func>
T* gasesteElement(std::vector<T>& v, Func criteriu) {
    auto it = std::find_if(v.begin(), v.end(), criteriu);
    if (it != v.end()) {
        return &(*it);
    }
    return nullptr;
}

template <typename T, typename Func>
std::vector<T*> filtreazaElemente(std::vector<T>& container, Func criteriu) {
    std::vector<T*> rezultate;
    for (auto& elem : container) {
        if (criteriu(elem)) {
            rezultate.push_back(&elem);
        }
    }
    return rezultate;
}

#endif //OOP_TEMPLATES_H