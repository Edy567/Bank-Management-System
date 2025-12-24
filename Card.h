#ifndef OOP_CARD_H
#define OOP_CARD_H
#pragma once
#include <string>
#include <iostream>
#include "Moneda.h"

class Card {
    double suma;
    std::string titular;
    std::string numarExp;
    std::string nrCard;
    Moneda moneda;

public:
    Card(const double &suma, std::string titular, std::string numarExp,
         std::string nrCard, const Moneda &moneda);

    Card(const Card &other);

    Card &operator=(const Card &other);

    ~Card();

    friend std::ostream &operator<<(std::ostream &os, const Card &card);

    [[nodiscard]] double getSuma() const;

    [[nodiscard]] const Moneda &getMoneda() const;

    [[nodiscard]] const std::string &getTitular() const;

    [[nodiscard]] const std::string& getNrCard() const;

    void scadeSuma(double valoare);

    void adaugaSuma(double valoare);
};
#endif
