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
    int nodeCount = 0;
protected:
    int start = 0;
    bool final[n] = {false};
public:
    States& operator=(const States& other);

    vector<int> initialStates();

    void changeFinalStates(const vector<int> &states);

    void changeInitialState(int state);
    explicit States(char value);
    States(const States& other);
    States() = default;
    States(int start, bool final1[]);
    States(const Input& input);
    bool validStates() const;
    int translate(const string& node) const;
    bool isFinalState(int state) const;
    int startNode() const;
    friend ostream& operator<<(ostream& os, const States &a);
    void updateNodeCount(int count);
    int size();
    vector<int> finalStates();
    void increaseN(int n);
    ~States();
};



#endif //STATES_H
