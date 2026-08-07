//
// Created by Gabriel on 4/16/2025.
//

#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include <vector>
#include "States.h"
#include "Sigma.h"

struct nu
    {
        int nod;
        char a;
    };
class Transitions
{

    static const int n = 2000, m = 50;
    char cuv[n]{};
    bool ok = true;
    bool dfa = true;
    int ct = 0;

protected:

    vector<nu> v[n];
    //dfa -> size() = 1 V nod,  nfa -> Ex. size() > 1
    map<char, set<int>> w[n];

public:
    Transitions& operator=(const Transitions& other);
    Transitions(const Transitions& other);
    Transitions() = default;
    explicit Transitions(const vector<nu> v[n]);
    Transitions(const Input &citire, const States &state, const Sigma &sigma);
    explicit Transitions(char value);
    void getTransition(map<char, set<int>> w[n]) const;
    bool validTransitions() const;
    char caracter(int stare, int i) const;
    int Size(int stare)const;
    int nod(int stare, int i) const;
    bool isDFA() const;
    bool isNFA()const;
    friend ostream& operator<<(ostream& os, const Transitions &a);
    void modificareTrans(vector <nu> v[n]);
    void increaseN(int n);
    void addTransitions(const vector<int>& stari, vector <int> v);
    void addTransitions(vector <nu> v[n]);
    vector<nu> *get_transitions();
    ~Transitions();

};



#endif //TRANSITIONS_H
