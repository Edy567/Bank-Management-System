//
// Created by User on 11/28/2025.
//

#ifndef OOP_CONT_GOLD_H
#define OOP_CONT_GOLD_H

#include "Cont.h"
#endif //OOP_CONT_GOLD_H
class ContGold : public Cont {

    public:
    ContGold(const std::vector<Card> &carduri, std::string IBAN, const std::vector<Tranzactie> &tranzactii);
    [[nodiscard]] ContGold* clone() const override;
    [[nodiscard]] double calculeazaComision(double suma) const override;
protected:
    void afisareDetaliata(std::ostream &os) const override;
};

