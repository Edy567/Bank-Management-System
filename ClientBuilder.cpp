//
// Created by User on 1/20/2026.
//

#include "ClientBuilder.h"

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

ClientBuilder &ClientBuilder::setParola(const std::string &pass) {
    parola = pass;
    return *this;
}

ClientBuilder &ClientBuilder::setVenit(double v) {
    venit = v;
    return *this;
}

ClientBuilder &ClientBuilder::setScorCredit(const int s) {
    scorCredit = s;
    return *this;
}

Client ClientBuilder::build() {
    if (nume.empty()) nume = "Necunoscut";
    if (prenume.empty()) prenume = "";
    if (venit < 0) venit = 0;

    return Client(nume, prenume, CNP, parola, venit, scorCredit);
}
