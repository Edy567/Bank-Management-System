#ifndef OOP_MONEDA_H
#define OOP_MONEDA_H
#pragma once
#include <string>
#include <iostream>

class Moneda {
    std::string cod;
    std::string nume;
    double cursValutar;

public:
    Moneda(std::string cod, std::string nume, const double &cursValutar);

    Moneda(const Moneda &other);

    Moneda &operator=(const Moneda &other);

    ~Moneda();

    [[nodiscard]] std::string getCod() const { return cod; }

    friend std::ostream &operator<<(std::ostream &os, const Moneda &moneda);
};
#endif
