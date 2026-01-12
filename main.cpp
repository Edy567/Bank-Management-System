
#include <vector>
#include "Banca.h"
#include "InterfataBanca.h"

int main() {
    std::vector<Client> clienti;
    std::vector<Angajat> angajati;


    Banca myBank("Banking App", clienti, angajati);


    UI_Banca app(myBank);

    app.run("banca_si_meniu.txt");

    return 0;
}