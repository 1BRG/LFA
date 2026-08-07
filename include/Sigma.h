//
// Created by Gabriel on 4/16/2025.
//

#ifndef SIGMA_H
#define SIGMA_H
#include "States.h"

#include <ostream>

class Sigma {
    static const int kNodeLimit = 2000;
    static const int kStringLength = 50;
    bool ok = true;
protected:
    bool alphabet[257];
public:
    Sigma &operator=(const Sigma &other);
    Sigma(const Sigma &other);
    Sigma() = default;
    explicit Sigma(char value);
    bool validSigma() const;
    explicit Sigma(const Input &input);
    bool belongsToAlphabet(char ch) const;
    friend std::ostream &operator<<(std::ostream &output, const Sigma &sigma);
    ~Sigma();
};



#endif //SIGMA_H
