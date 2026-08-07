//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Sigma.h"

Sigma &Sigma::operator=(const Sigma &other) {
    // std::cout << "Operator egal\n";
    for (int i = 0; i < 257; i++)
        this->alfabet[i] = other.alfabet[i];
    this->ok = other.ok;
    return *this;
}

Sigma::Sigma(const Sigma &other) {
    //std::cout << "Constructor de Copiere\n";
    for (int i = 0; i < 257; i++)
        this->alfabet[i] = other.alfabet[i];
    this->ok = other.ok;
}

Sigma::Sigma(char value) {
    alfabet[value] = true;
}

bool Sigma::validSigma() const {
    return ok;
}

Sigma::Sigma(const Input &citire) {
    for (int i = 0; i < 257; i++)
        alfabet[i] = 0;
    string mat[n];
    char s;
    citire.Matrice(mat);
    for (int i = citire.findSigma() + 1; true; i++) {
        if (mat[i] == "End")
            break;
        if (mat[i][0] == '#')
            continue;
        s = mat[i][0];
        alfabet[int(s)] = 1;
    }
}

bool Sigma::apartineAlfabet(char ch) const {
    return alfabet[int(ch)];
}

ostream &operator<<(ostream &os, const Sigma &a) {
    os << "Alfabet: ";
    for (int i = 0; i < 255; i++)
        if (a.alfabet[i])
            os << char(i) << " ";
    os << "\n";
    return os;
}

Sigma::~Sigma() {
    //cout << "Destructor";
}
