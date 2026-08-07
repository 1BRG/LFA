//
// Created by Gabriel on 4/16/2025.
//

#ifndef SIGMA_H
#define SIGMA_H

#include "States.h"
#include <ostream>

class Sigma {
private:
    static const int kNodeLimit = 2000;
    static const int kStringLength = 50;
    bool ok = true;

protected:
    bool alphabet[257];

public:
    // --- Constructors, Destructor, and Assignment ---
    Sigma() = default;
    explicit Sigma(char value);
    explicit Sigma(const Input &input);

    Sigma(const Sigma &other);
    Sigma &operator=(const Sigma &other);
    ~Sigma();

    // --- Core API / Getters ---
    bool validSigma() const;
    bool belongsToAlphabet(char ch) const;

    // --- Operator Overloads ---
    friend std::ostream &operator<<(std::ostream &output, const Sigma &sigma);
};

#endif //SIGMA_H