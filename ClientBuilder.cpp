#include "ClientBuilder.h"
#include "ContFactory.h"
#include "Card.h"


ClientBuilder::ClientBuilder() {
    reset();
}

void ClientBuilder::reset() {
    nume = "";
    prenume = "";
    CNP = "";
    parola = "";
    venit = 0.0;
    scorCredit = 0;
    conturiDeAtasat.clear();
}

ClientBuilder &ClientBuilder::setNume(const std::string &n) {
    nume = n;
    return *this;
}

ClientBuilder &ClientBuilder::setPrenume(const std::string &p) {
    prenume = p;
    return *this;
}

ClientBuilder &ClientBuilder::setCNP(const std::string &c) {
    CNP = c;
    return *this;
}

ClientBuilder &ClientBuilder::setParola(const std::string &p) {
    parola = p;
    return *this;
}

ClientBuilder &ClientBuilder::setVenit(double v) {
    venit = v;
    return *this;
}

ClientBuilder &ClientBuilder::setScorCredit(int s) {
    scorCredit = s;
    return *this;
}

ClientBuilder &ClientBuilder::makeVIP() {
    venit = 15000.0;
    scorCredit = 850;
    if (nume.empty()) nume = "VIP_User";


    const std::vector<Card> cRon = {Card(50000.0, "Titular VIP", "12/30", "1111222233334444", Moneda("RON", "Leu", 1))};
    if (Cont *p = ContFactory::creareCont("PREMIUM", cRon, "RO01VIP_RON", {}))
        conturiDeAtasat.push_back(p);


    const std::vector<Card> cEur = {
        Card(10000.0, "Titular VIP", "12/30", "5555666677778888", Moneda("EUR", "Euro", 4.97))
    };
    if (Cont *p = ContFactory::creareCont("PREMIUM", cEur, "RO02VIP_EUR", {}))
        conturiDeAtasat.push_back(p);


    const std::vector<Card> cGbp = {
        Card(5000.0, "Titular VIP", "12/30", "9999000011112222", Moneda("GBP", "Lira", 5.80))
    };
    if (Cont *p = ContFactory::creareCont("PREMIUM", cGbp, "RO03VIP_GBP", {}))
        conturiDeAtasat.push_back(p);

    return *this;
}

ClientBuilder &ClientBuilder::makeStudent() {
    venit = 800.0;
    scorCredit = 600;


    const std::vector<Card> cStud = {
        Card(500.50, "Student Bursier", "09/28", "1234123412341234", Moneda("RON", "Leu", 1))
    };
    if (Cont *p = ContFactory::creareCont("STUDENT", cStud, "RO99STUDENT", {}))
        conturiDeAtasat.push_back(p);

    return *this;
}

ClientBuilder &ClientBuilder::makeRiskyClient() {
    venit = 2000.0;
    scorCredit = 300;


    const std::vector<Card> cRisk = {Card(30, "Riscant", "01/26", "0000000000000000", Moneda("RON", "Leu", 1))};
    if (Cont *p = ContFactory::creareCont("SILVER", cRisk, "RO00RISKY_RON", {}))
        conturiDeAtasat.push_back(p);

    return *this;
}

Client ClientBuilder::build() {
    if (nume.empty()) nume = "Necunoscut";
    if (prenume.empty()) prenume = "";
    if (venit < 0) venit = 0;

    Client c(nume, prenume, CNP, parola, venit, scorCredit);


    for (auto *cont: conturiDeAtasat) {
        c.adaugaCont(cont);
    }
    

    conturiDeAtasat.clear(); 

    return c;
}