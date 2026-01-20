#include "CardValidator.h"
#include <iostream>
#include <ctime>
#include <sstream>

bool CardValidator::verificaLuhn(const std::string &numarCard) {
    if (numarCard.length() != 16) return false;

    int suma = 0;
    bool isSecond = false;


    for (int i = numarCard.length() - 1; i >= 0; i--) {
        int d = numarCard[i] - '0';

        if (d < 0 || d > 9) return false;

        if (isSecond) {
            d = d * 2;
        }


        suma += (d / 10) + (d % 10);

        isSecond = !isSecond;
    }

    return (suma % 10 == 0);
}

bool CardValidator::verificaExpirare(const std::string &dataExp) {
    if (dataExp.length() != 5 || dataExp[2] != '/') return false;

    try {
        int luna = std::stoi(dataExp.substr(0, 2));
        int an = std::stoi(dataExp.substr(3, 2)) + 2000;

        if (luna < 1 || luna > 12) return false;


        const std::time_t t = std::time(nullptr);
        const std::tm *now = std::localtime(&t);
        int anCurent = now->tm_year + 1900;
        int lunaCurenta = now->tm_mon + 1;

        if (an > anCurent) return true;
        if (an == anCurent && luna >= lunaCurenta) return true;

        return false;
    } catch (...) {
        return false;
    }
}
