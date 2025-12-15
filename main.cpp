
#include <vector>
#include "Banca.h"     // [cite: 85]
#include "InterfataBanca.h"  // Header-ul interfetei grafice

int main() {

    std::vector<Client> clienti;
    std::vector<Angajat> angajati;



    Banca myBank("Banking App", clienti, angajati);


    UI_Banca app(myBank);

    app.run("banca_si_meniu.txt");

    return 0;
}