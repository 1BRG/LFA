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
    static const int kNodeLimit = 2000;
    static const int kStateNameLength = 50;
    bool ok = true;
    std::map<std::string, int> stateMap;
    int nodeCount = 0;
protected:
    int start = 0;
    bool final[kNodeLimit] = {false};
public:
    States &operator=(const States &other);

    std::vector<int> initialStates() const;

    void changeFinalStates(const std::vector<int> &states);

    void changeInitialState(int state);
    explicit States(char value);
    States(const States &other);
    States() = default;
    States(int start, bool finalStates[]);
    explicit States(const Input &input);
    bool validStates() const;
    int translate(const std::string &node) const;
    bool isFinalState(int state) const;
    int startNode() const;
    friend std::ostream &operator<<(std::ostream &output, const States &state);
    void updateNodeCount(int count);
    int size();
    std::vector<int> finalStates() const;
    void increaseN(int count);
    ~States();
};



#endif //STATES_H
