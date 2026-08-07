//
// Created by Gabriel on 4/16/2025.
//

#ifndef STATES_H
#define STATES_H

#include "Input.h"
#include <iostream>
#include <map>
#include <set>
#include<string.h>
#include <vector>

class States
{
    static const int n = 2000, m = 50;
    bool ok = true;
    map<string, int> q;
    int nrNoduri = 0;
protected:
    int start = 0;
    bool final[n] = {false};
public:
    States& operator=(const States& other);

    vector<int> stareInitiala();

    void changeStariFinale(const vector<int> & vector);

    void changeStareInitiala(int i);
    explicit States(char value);
    States(const States& other);
    States() = default;
    States(int start, bool final1[]);
    States(const Input& citire);
    bool validStates() const;
    int translate(const string& nod) const;
    bool stareFinala(int stare) const;
    int nodStart() const;
    friend ostream& operator<<(ostream& os, const States &a);
    void nNoduri(int nr);
    int size();
    vector <int> stariFinale();
    void increaseN(int n);
    ~States();
};



#endif //STATES_H
