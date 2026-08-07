//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Sigma.h"

Sigma &Sigma::operator=(const Sigma &other) {
    for (int i = 0; i < 257; i++)
        this->alphabet[i] = other.alphabet[i];
    this->ok = other.ok;
    return *this;
}

Sigma::Sigma(const Sigma &other) {
    for (int i = 0; i < 257; i++)
        this->alphabet[i] = other.alphabet[i];
    this->ok = other.ok;
}

Sigma::Sigma(char value) {
    alphabet[value] = true;
}

bool Sigma::validSigma() const {
    return ok;
}

Sigma::Sigma(const Input &input) {
    for (int i = 0; i < 257; i++)
        alphabet[i] = 0;
    string matrix[n];
    char symbol;
    input.matrix(matrix);
    for (int i = input.findSigma() + 1; true; i++) {
        if (matrix[i] == "End")
            break;
        if (matrix[i][0] == '#')
            continue;
        symbol = matrix[i][0];
        alphabet[int(symbol)] = 1;
    }
}

bool Sigma::belongsToAlphabet(char ch) const {
    return alphabet[int(ch)];
}

ostream &operator<<(ostream &os, const Sigma &a) {
    os << "Alphabet: ";
    for (int i = 0; i < 255; i++)
        if (a.alphabet[i])
            os << char(i) << " ";
    os << "\n";
    return os;
}

Sigma::~Sigma() {
}
