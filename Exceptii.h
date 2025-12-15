//
// Created by User on 12/2/2025.
//

#ifndef OOP_EXCEPTII_H
#define OOP_EXCEPTII_H

#endif //OOP_EXCEPTII_H

#pragma once
#include <exception>
#include <string>

class Eroare : public std::exception {
protected:
    std::string mesaj;

public:
    explicit Eroare(const std::string &msg) : mesaj("Eroare : " + msg) {
    }

    [[nodiscard]] const char *what() const noexcept override { return mesaj.c_str(); }
};

class FonduriInsuficiente : public Eroare {
public:
    explicit FonduriInsuficiente() : Eroare("Fonduri insuficiente pentru aceasta operatiune!") {
    }
};

class LimitaCarduriDepasita : public Eroare {
public:
    LimitaCarduriDepasita() : Eroare("S-a atins limita maxima de carduri admisa pentru acest tip de cont!") {
    }
};
