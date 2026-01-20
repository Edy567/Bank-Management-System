#ifndef OOP_STATS_H
#define OOP_STATS_H

#include <vector>
#include <functional>
#include <cmath>
#include <algorithm>


template<typename T>
class Stats {
    const std::vector<T> &date;

public:
    explicit Stats(const std::vector<T> &d) : date(d) {
    }


    double calculeazaMedia(std::function<double(const T &)> extractor) const {
        if (date.empty()) return 0.0;
        double suma = 0.0;
        for (const auto &elem: date) {
            suma += extractor(elem);
        }
        return suma / date.size();
    }

    double standardDeviation(std::function<double(const T &)> extractor) const {
        if (date.size() < 2) return 0.0;

        double medie = calculeazaMedia(extractor);
        double sumaPatrate = 0.0;

        for (const auto &elem: date) {
            const double val = extractor(elem);
            double diff = val - medie;
            sumaPatrate += diff * diff;
        }

        return std::sqrt(sumaPatrate / date.size());
    }


    int numaraDaca(std::function<bool(const T &)> predicat) const {
        int count = 0;
        for (const auto &elem: date) {
            if (predicat(elem)) {
                count++;
            }
        }
        return count;
    }


    double CalculMediana(std::function<double(const T &)> extractor) const {
        if (date.empty()) return 0.0;


        std::vector<double> valori;
        valori.reserve(date.size());
        for (const auto &elem: date) {
            valori.push_back(extractor(elem));
        }


        std::ranges::sort(valori);


        if (size_t n = valori.size(); n % 2 == 0) {
            return (valori[n / 2 - 1] + valori[n / 2]) / 2.0;
        } else {
            return valori[n / 2];
        }
    }

    //
    double calculeazaSkewness(std::function<double(const T &)> extractor) const {
        size_t n = date.size();
        if (n < 2) return 0.0;

        double medie = calculeazaMedia(extractor);
        double deviatie = standardDeviation(extractor);

        if (deviatie == 0) return 0.0; // Toate valorile sunt egale

        double sumaCuburi = 0.0;
        for (const auto &elem: date) {
            double val = extractor(elem);

            double diff = val - medie;
            sumaCuburi += diff * diff * diff;
        }


        return sumaCuburi / ((n - 1) * deviatie * deviatie * deviatie);
    }
};

#endif //OOP_STATS_H
