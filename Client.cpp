#include "Client.h"
#include <utility>

Client::Client(std::string nume, std::string prenume, std::string CNP, std::string parola,
               const double &venit, const int &scorCredit)
    : nume{std::move(nume)}, prenume{std::move(prenume)}, CNP{std::move(CNP)}, parola{std::move(parola)},
      venit{venit}, scorCredit{scorCredit} {
    std::cout << "(Client) Constructor parametrizat\n";
}

Client::Client(const Client &other)
    : nume{other.nume}, prenume{other.prenume}, CNP{other.CNP}, parola{other.parola},
      venit{other.venit}, scorCredit{other.scorCredit} {
    for (const auto *cont: other.conturi) {
        conturi.push_back(cont->clone());
    }
    std::cout << "(Client) Constructor de copiere\n";
}

void swap(Client &first, Client &second) noexcept {
    using std::swap;
    swap(first.nume, second.nume);
    swap(first.conturi, second.conturi);
    swap(first.prenume, second.prenume);
    swap(first.CNP, second.CNP);
    swap(first.parola, second.parola);
    swap(first.venit, second.venit);
    swap(first.scorCredit, second.scorCredit);
}

Client &Client::operator=(const Client &other) {
    if (this != &other) {
        Client temp(other);
        swap(*this, temp);
    }
    std::cout << "(Client) Operator= apelat\n";
    return *this;
}

Client::~Client() {
    for (auto *cont: conturi) {
        delete cont;
    }
    conturi.clear();
    std::cout << "[Client] Destructor\n";
}

std::ostream &operator<<(std::ostream &os, const Client &c) {
    os << c.nume << " " << c.prenume << " " << c.CNP << " " << c.venit << " " << c.scorCredit;
    os << "Conturi:\n";
    for (const auto *cont: c.conturi) {
        os << cont << "\n";
    }
    return os;
}

std::vector<Cont *> &Client::getConturi() { return conturi; }

void Client::adaugaCont(Cont *c) {
    conturi.push_back(c);
}

std::string Client::credit(const double suma, int luni) const {
    const double rata = (suma / luni) * 1.05;
    double raport = rata / venit;

    if (scorCredit >= 700 && raport < 0.3)
        return "Cerere aprobata.";
    else if (scorCredit >= 600 && raport < 0.4)
        return "Cererea trebuie analizata.";
    else
        return "Cerere respinsa";
}

const std::string &Client::getCNP() const { return CNP; }
const std::string &Client::getNume() const { return nume; }
const std::string &Client::getPrenume() const { return prenume; }
bool Client::verificaParola(const std::string &p) const { return parola == p; }