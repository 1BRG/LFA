//
// Created by Gabriel on 4/16/2025.
//

#ifndef AUTOMAT_H
#define AUTOMAT_H
#include "Transitions.h"
#include <deque>


class Automat
{

    static const int n = 2000, m = 50;
    States S;
    Transitions T;
    Sigma A;

    bool ok = true;
    void dfs(bool &valid, char cuv[], int stare, int poz, int len) const;
    void returneaza_nfa(int nod, bool ok[], vector<nu> &aux, vector<nu> v[], bool &esteFinal);
public:
    Automat& operator=(const Automat& other);
    Automat(const Automat& other);

    static Automat toAutomat(deque<string> p);

    Automat() = default;
    explicit Automat(const Input &citire);
    Automat(const States &state, const Sigma &sigma, const Transitions &trans);

    explicit Automat(char value);

    static void parsareRegex( string & regex);

    static deque<string> postfixat(string &s);

    explicit Automat(string &regex);

    bool apartine_automat(string &cuv);

    bool cuvant(char cuv[]);
    bool isValid();
    bool isDFA() const;
    bool isNFA()const;
    void toDFA();
    friend ostream& operator<<(ostream& os, const Automat &a);
    void concatenare(Automat &other);
    void alternare(Automat &other);
    void stelat();
    void plus();
    void misterios();
    void increaseN(int n);
    void toNFA();
    vector<nu> *get_transitions();
    vector<int> stareInitiala();
    vector <int> stariFinale();
    States s() const {
        return S;
    }

    Transitions t() const {
        return T;
    }

    Sigma a() const {
        return A;
    }

    ~Automat();
};



#endif //AUTOMAT_H
