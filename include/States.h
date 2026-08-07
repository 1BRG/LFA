//
// Created by Gabriel on 4/16/2025.
//

#ifndef STATES_H
#define STATES_H

#include "Input.h"

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

class States {
private:
    static const int kNodeLimit = 2000;
    static const int kStateNameLength = 50;
    bool ok = true;
    std::map<std::string, int> stateMap;
    int nodeCount = 0;

protected:
    int start = 0;
    bool final[kNodeLimit] = {false};

public:
    // --- Constructors, Destructor, and Assignment ---
    States() = default;
    explicit States(char value);
    States(int start, bool finalStates[]);
    explicit States(const Input &input);

    States(const States &other);
    States &operator=(const States &other);
    ~States();

    // --- Core API / Getters ---
    bool validStates() const;
    int size();
    int translate(const std::string &node) const;
    int startNode() const;
    bool isFinalState(int state) const;
    std::vector<int> initialStates() const;
    std::vector<int> finalStates() const;

    // --- Modifiers ---
    void changeInitialState(int state);
    void changeFinalStates(const std::vector<int> &states);
    void updateNodeCount(int count);
    void increaseN(int count);

    // --- Operator Overloads ---
    friend std::ostream &operator<<(std::ostream &output, const States &state);
};

#endif //STATES_H