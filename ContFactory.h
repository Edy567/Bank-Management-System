#ifndef CONT_FACTORY_H
#define CONT_FACTORY_H

#include "Cont.h"
#include "Cont_Silver.h"
#include "Cont_Gold.h"
#include "Cont_premium.h"
#include "ContStudent.h"
#include <string>
#include <vector>

class ContFactory {
public:
    static Cont* creareCont(const std::string& tipCont,
                            const std::vector<Card>& carduri,
                            const std::string& iban,
                            const std::vector<Tranzactie>& tranzactii) {
        if (tipCont == "SILVER") {
            return new ContSilver(carduri, iban, tranzactii);
        } else if (tipCont == "GOLD") {
            return new ContGold(carduri, iban, tranzactii);
        } else if (tipCont == "PREMIUM") {
            return new ContPremium(carduri, iban, tranzactii);
        } else if (tipCont == "STUDENT") {
            return new ContStudent(carduri, iban, tranzactii);
        }
        return nullptr;
    }
};

#endif