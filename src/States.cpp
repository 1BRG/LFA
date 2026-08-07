//
// Created by Gabriel on 4/16/2025.
//

#include "../include/States.h"

#include <cstring>

States &States::operator=(const States &other) {
    for (int index = 0; index < kNodeLimit; ++index) {
        this->final[index] = other.final[index];
    }
    this->start = other.start;
    this->ok = other.ok;
    this->stateMap = other.stateMap;
    this->nodeCount = other.nodeCount;
    return *this;
}

States::States(char value) : nodeCount(2), start(1) {
    final[2] = true;
    stateMap[std::to_string(1)] = 1;
    stateMap[std::to_string(2)] = 2;
}

States::States(const States &other) {
    for (int index = 0; index < kNodeLimit; ++index) {
        this->final[index] = other.final[index];
    }
    this->start = other.start;
    this->ok = other.ok;
    this->stateMap = other.stateMap;
    this->nodeCount = other.nodeCount;
}

States::States(int initialState, bool initialFinalStates[]) {
    this->start = initialState;
    std::memcpy(this->final, initialFinalStates, sizeof(final));
    for (int index = 0; index < kNodeLimit; ++index) {
        stateMap[std::to_string(index)] = index;
    }
}

States::States(const Input &input) {
    for (bool & index : final) {
        index = false;
    }

    std::string matrix[kNodeLimit];
    input.matrix(matrix);
    int stateId = 0;
    bool hasStartState = false;

    for (int index = input.findState() + 1; true; ++index) {
        char stateLine[kStateNameLength];
        if (matrix[index] == "End") {
            break;
        }
        if (matrix[index][0] == '#') {
            continue;
        }

        std::strcpy(stateLine, matrix[index].c_str());
        char *token = std::strtok(stateLine, ", ");
        int currentNode = 0;
        int stateIndex = 0;

        while (token) {
            char word[kStateNameLength];
            std::strcpy(word, token);

            if (word[0] == 'S' && stateIndex != 0) {
                if (start != 0) {
                    ok = false;
                } else {
                    start = currentNode;
                    hasStartState = true;
                }
            } else if (word[0] == 'F' && stateIndex != 0) {
                final[currentNode] = true;
            } else {
                if (stateMap[word] != 0) {
                    currentNode = stateMap[word];
                } else {
                    currentNode = ++stateId;
                    stateMap[word] = currentNode;
                }
            }

            ++stateIndex;
            token = std::strtok(nullptr, ", ");
        }
    }

    if (!hasStartState) {
        ok = false;
    }
}

bool States::validStates() const {
    return ok;
}

int States::translate(const std::string &node) const {
    auto it = stateMap.find(node);
    if (it != stateMap.end()) {
        return it->second;
    }
    return -1;
}

bool States::isFinalState(int state) const {
    return final[state];
}

int States::startNode() const {
    return this->start;
}

std::ostream &operator<<(std::ostream &output, const States &state) {
    output << "About states:\n";
    if (state.ok == false || state.start == 0) {
        output << "States invalid\n";
        return output;
    }

    output << "Start node: " << state.start << "\n";
    output << "Final nodes: ";
    int count = 0;
    for (int index = 0; index < States::kNodeLimit; ++index) {
        if (state.final[index]) {
            output << index << " ";
            ++count;
        }
    }

    if (!count) {
        output << "The automaton has no final states";
    }
    output << "\n";
    return output;
}

void States::updateNodeCount(int count) {
    if (nodeCount == 0) {
        nodeCount = stateMap.size();
    }
    nodeCount += count;
}

void States::changeInitialState(int state) {
    start = state;
}

int States::size() {
    updateNodeCount(0);
    return nodeCount;
}

void States::increaseN(int count) {
    for (int index = kNodeLimit - 1; index >= 0; --index) {
        if (final[index]) {
            final[index + count] = true;
        }
        final[index] = false;
    }
    start += count;
    updateNodeCount(count);
}

std::vector<int> States::initialStates() const {
    std::vector<int> states;
    states.push_back(start);
    return states;
}

void States::changeFinalStates(const std::vector<int> &states) {
    for (bool & index : final) {
        index = false;
    }
    for (auto node : states) {
        final[node] = true;
    }
}

std::vector<int> States::finalStates() const {
    std::vector<int> states;
    for (int index = 0; index < kNodeLimit; ++index) {
        if (final[index]) {
            states.push_back(index);
        }
    }
    return states;
}

States::~States() = default;
