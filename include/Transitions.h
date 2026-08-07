//
// Created by Gabriel on 4/16/2025.
//

#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include <vector>
#include "States.h"
#include "Sigma.h"

struct TransitionNode
{
    int node;
    char symbol;
};
class Transitions
{

    static const int n = 2000, m = 50;
    char word[n]{};
    bool ok = true;
    bool dfa = true;
    int count = 0;

protected:

    vector<TransitionNode> transitions[n];
    //dfa -> size() = 1 V nod,  nfa -> Ex. size() > 1
    map<char, set<int>> w[n];

public:
    Transitions& operator=(const Transitions& other);
    Transitions(const Transitions& other);
    Transitions() = default;
    explicit Transitions(const vector<TransitionNode> transitions[n]);
    Transitions(const Input &input, const States &state, const Sigma &sigma);
    explicit Transitions(char value);
    void getTransition(map<char, set<int>> w[n]) const;
    bool validTransitions() const;
    char character(int state, int index) const;
    int size(int state) const;
    int node(int state, int index) const;
    bool isDFA() const;
    bool isNFA()const;
    friend ostream& operator<<(ostream& os, const Transitions &a);
    void modifyTransitions(vector<TransitionNode> transitions[n]);
    void increaseN(int n);
    void addTransitions(const vector<int>& states, vector<int> targets);
    void addTransitions(vector<TransitionNode> transitions[n]);
    vector<TransitionNode> *getTransitions();
    ~Transitions();

};



#endif //TRANSITIONS_H
