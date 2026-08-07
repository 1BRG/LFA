//
// Created by Gabriel on 4/16/2025.
//

#ifndef SIGMA_H
#define SIGMA_H
#include "States.h"
using namespace std;


class Sigma
{
    static const int n = 2000, m = 50;
    bool ok = true;
protected:
    bool alfabet[257];
public:
    Sigma& operator=(const Sigma& other);
    Sigma(const Sigma& other);
    Sigma () {}
    Sigma(char value);
    bool validSigma() const;
    Sigma(const Input &citire);
    bool apartineAlfabet(char ch) const;
    friend ostream& operator<<(ostream& os, const Sigma &a);
    ~Sigma();

};



#endif //SIGMA_H
