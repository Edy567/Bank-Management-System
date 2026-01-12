#ifndef OOP_CONT_STUDENT_H
#define OOP_CONT_STUDENT_H

#include "Cont.h"

class ContStudent : public Cont {
public:
    ContStudent(const std::vector<Card> &carduri, std::string IBAN,
                const std::vector<Tranzactie> &tranzactii)
        : Cont(carduri, std::move(IBAN), tranzactii, 2) {
    }

    [[nodiscard]] ContStudent *clone() const override { return new ContStudent(*this); }
    [[nodiscard]] double calculeazaComision(double suma) const override { return suma * 0.0; }
    [[nodiscard]] std::string getTip() const override { return "STUDENT"; }

protected:
    void afisareDetaliata(std::ostream &os) const override {
        os << "Student: Max 2 carduri, fara comision.";
    }
};

#endif
