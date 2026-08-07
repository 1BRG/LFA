//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Sigma.h"

// --- Constructors, Destructor, and Assignment ---

Sigma::Sigma(char value) {
    for (bool & index : alphabet) {
        index = false;
    }
    alphabet[value] = true;
}

Sigma::Sigma(const Input &input) {
    for (bool & index : alphabet) {
        index = false;
    }

    std::string matrix[kNodeLimit];
    input.matrix(matrix);

    for (int index = input.findSigma() + 1; true; ++index) {
        if (matrix[index] == "End") {
            break;
        }
        if (matrix[index][0] == '#') {
            continue;
        }
        char symbol = matrix[index][0];
        alphabet[int(symbol)] = true;
    }
}

Sigma::Sigma(const Sigma &other) {
    for (int index = 0; index < 257; ++index) {
        this->alphabet[index] = other.alphabet[index];
    }
    this->ok = other.ok;
}

Sigma &Sigma::operator=(const Sigma &other) {
    if (this != &other) {
        for (int index = 0; index < 257; ++index) {
            this->alphabet[index] = other.alphabet[index];
        }
        this->ok = other.ok;
    }
    return *this;
}

Sigma::~Sigma() = default;

// --- Core API / Getters ---

bool Sigma::validSigma() const {
    return ok;
}

bool Sigma::belongsToAlphabet(char ch) const {
    return alphabet[int(ch)];
}

// --- Operator Overloads ---

std::ostream &operator<<(std::ostream &output, const Sigma &sigma) {
    output << "Alphabet: ";
    for (int index = 0; index < 255; ++index) {
        if (sigma.alphabet[index]) {
            output << static_cast<char>(index) << " ";
        }
    }
    output << "\n";
    return output;
}