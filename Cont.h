#ifndef OOP_CONT_H
#define OOP_CONT_H
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Card.h"
#include "Tranzactie.h"

class Cont {
    std::string IBAN;
    std::vector<Card> carduri;
    std::vector<Tranzactie> tranzactii;
    int limitaCarduri;

    virtual void afisareDetaliata(std::ostream &os) const = 0;

public:
    explicit Cont(const std::vector<Card> &carduri, std::string IBAN,
                  const std::vector<Tranzactie> &tranzactii, int limitaCarduri);

    Cont(const Cont &other);

    Cont &operator=(const Cont &other);

    virtual ~Cont();

    [[nodiscard]] virtual Cont *clone() const = 0;

    [[nodiscard]] virtual double calculeazaComision(double suma) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Cont &cont);

    [[nodiscard]] const std::string &getIBAN() const;

    [[nodiscard]] const std::vector<Card> &getCarduri() const;

    bool retrageSuma(double suma);

    void adaugaSuma(double suma);

    [[nodiscard]] double getSoldTotal() const;

    [[nodiscard]] const std::vector<Tranzactie> &getTranzactii() const;

    void adaugaTranzactie(const Tranzactie &t);

    bool adaugaCard(const Card &card);
};
#endif
