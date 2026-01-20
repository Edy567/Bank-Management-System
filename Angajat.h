//
// Created by User on 11/27/2025.
//

#ifndef OOP_ANGAJAT_H
#define OOP_ANGAJAT_H

#endif //OOP_ANGAJAT_H
#pragma once
#include <string>
#include <iostream>

class Angajat {
    std::string nume;
    int id;
    int salariu;

public:
    Angajat(std::string nume, const int &id, const int &salariu);

    Angajat(const Angajat &other);

    Angajat &operator=(const Angajat &other);

    ~Angajat();

    friend std::ostream &operator<<(std::ostream &os, const Angajat &ang);

    [[nodiscard]] int getSalariu() const { return salariu; }
};
