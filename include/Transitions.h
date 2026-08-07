//
// Created by Gabriel on 4/16/2025.
//

#ifndef TRANSITIONS_H
#define TRANSITIONS_H

#include <map>
#include <set>
#include <vector>

#include "Sigma.h"
#include "States.h"

struct TransitionNode {
    int node;
    char symbol;
};

class Transitions {
    static const int kNodeLimit = 2000;
    static const int kStringLength = 50;
    char word[kNodeLimit]{};
    bool ok = true;
    bool dfa = true;
    int count = 0;

protected:
    std::vector<TransitionNode> transitions[kNodeLimit];
    std::map<char, std::set<int>> transitionMap[kNodeLimit];

public:
    Transitions &operator=(const Transitions &other);
    Transitions(const Transitions &other);
    Transitions() = default;
    explicit Transitions(const std::vector<TransitionNode> transitions[kNodeLimit]);
    Transitions(const Input &input, const States &state, const Sigma &sigma);
    explicit Transitions(char value);
    void getTransition(std::map<char, std::set<int>> transitionMap[kNodeLimit]) const;
    bool validTransitions() const;
    char character(int state, int index) const;
    int size(int state) const;
    int node(int state, int index) const;
    bool isDFA() const;
    bool isNFA() const;
    friend std::ostream &operator<<(std::ostream &output, const Transitions &transition);
    void modifyTransitions(std::vector<TransitionNode> transitions[kNodeLimit]);
    void increaseN(int count);
    void addTransitions(const std::vector<int> &states, const std::vector<int>& targets);
    void addTransitions(std::vector<TransitionNode> transitions[kNodeLimit]);
    std::vector<TransitionNode> *getTransitions();
    ~Transitions();
};



#endif //TRANSITIONS_H
