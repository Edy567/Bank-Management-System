//
// Created by User on 11/27/2025.
//

#ifndef OOP_TRANZACTIE_H
#define OOP_TRANZACTIE_H

#endif //OOP_TRANZACTIE_H
#pragma once
#include <string>
#include <iostream>

class Tranzactie {
    std::string descriere;
    int suma;
    static int nrTranzactii;

public:
    Tranzactie(const int &sum, std::string descriere);

    Tranzactie(const Tranzactie &tr);

    Tranzactie &operator=(const Tranzactie &tr);

    ~Tranzactie();

    friend std::ostream &operator<<(std::ostream &os, const Tranzactie &tr);

    static int getTotal() { return nrTranzactii; }
};
