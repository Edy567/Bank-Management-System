//
// Created by User on 1/20/2026.
//

#ifndef OOP_CLIENTBUILDER_H
#define OOP_CLIENTBUILDER_H

#ifndef CLIENT_BUILDER_H
#define CLIENT_BUILDER_H

#include "Client.h"
#include <string>

class ClientBuilder {
    std::string nume;
    std::string prenume;
    std::string CNP;
    std::string parola;
    double venit;
    int scorCredit;

    std::vector<Cont *> conturiDeAtasat;

public:
    ClientBuilder();

    void reset();

    ClientBuilder &setNume(const std::string &n);

    ClientBuilder &setPrenume(const std::string &p);

    ClientBuilder &setCNP(const std::string &c);

    ClientBuilder &setParola(const std::string &pass);

    ClientBuilder &setVenit(double v);

    ClientBuilder &setScorCredit(int s);

    [[maybe_unused]] ClientBuilder &makeVIP();

    [[maybe_unused]] ClientBuilder &makeStudent();

    [[maybe_unused]] ClientBuilder &makeRiskyClient();

    Client build();
};

#endif


#endif //OOP_CLIENTBUILDER_H
