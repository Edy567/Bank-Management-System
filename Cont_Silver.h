

#ifndef OOP_CONT_SILVER_H
#define OOP_CONT_SILVER_H
#include "Cont.h"

#endif //OOP_CONT_SILVER_H


class ContSilver : public Cont {
public:
        explicit ContSilver(const std::vector<Card> &carduri, std::string IBAN,
                            const std::vector<Tranzactie> &tranzactii);

        [[nodiscard]] ContSilver *clone() const override;

        [[nodiscard]] double calculeazaComision(double suma) const override;

protected:
        void afisareDetaliata(std::ostream &os) const override;
};