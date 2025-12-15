

#ifndef OOP_CONT_PREMIUM_H
#define OOP_CONT_PREMIUM_H

#endif //OOP_CONT_PREMIUM_H

#include "Cont.h"

class ContPremium : public Cont {

    public:
    explicit ContPremium(const std::vector<Card> &carduri, std::string IBAN, const std::vector<Tranzactie> &tranzactii);
    [[nodiscard]] ContPremium* clone() const override;
    [[nodiscard]] double calculeazaComision(double suma) const override;
protected:
    void afisareDetaliata(std::ostream &os) const override;
};