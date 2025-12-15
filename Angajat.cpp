#include "Angajat.h"
#include <utility>

Angajat::Angajat(std::string nume, const int &id, const int &salariu)
    : nume{std::move(nume)}, id{id}, salariu{salariu} {
    std::cout << "(Angajat) Constructor parametrizat\n";
}

Angajat::Angajat(const Angajat &other)
    : nume{other.nume}, id{other.id}, salariu{other.salariu} {
    std::cout << "(Angajat) Constructor de copiere\n";
}

Angajat &Angajat::operator=(const Angajat &other) {
    if (this != &other) {
        nume = other.nume;
        id = other.id;
        salariu = other.salariu;
        std::cout << "(Angajat) Operator= apelat\n";
    }
    return *this;
}

Angajat::~Angajat() {
    std::cout << "(Angajat) Destructor\n";
}

std::ostream &operator<<(std::ostream &os, const Angajat &ang) {
    os << ang.nume << " " << ang.id << " " << ang.salariu;
    return os;
}
