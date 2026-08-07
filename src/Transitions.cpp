//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Transitions.h"

#include <cstring>

Transitions &Transitions::operator=(const Transitions &other) {
    this->ok = other.ok;
    this->dfa = other.dfa;
    this->count = other.count;
    for (int index = 0; index < kNodeLimit; ++index) {
        this->transitions[index] = other.transitions[index];
        this->transitionMap[index] = other.transitionMap[index];
        this->word[index] = other.word[index];
    }
    return *this;
}

Transitions::Transitions(char value) {
    transitions[1].push_back({2, value});
    transitionMap[1][value].insert(2);
}

Transitions::Transitions(const Transitions &other) {
    this->ok = other.ok;
    this->dfa = other.dfa;
    this->count = other.count;
    for (int index = 0; index < kNodeLimit; ++index) {
        this->transitions[index] = other.transitions[index];
        this->transitionMap[index] = other.transitionMap[index];
        this->word[index] = other.word[index];
    }
}

Transitions::Transitions(const std::vector<TransitionNode> transitions[kNodeLimit]) {
    for (int index = 0; index < kNodeLimit; ++index) {
        for (int transitionIndex = 0; transitionIndex < transitions[index].size(); ++transitionIndex) {
            this->transitions[index].push_back(transitions[index][transitionIndex]);
            transitionMap[index][transitions[index][transitionIndex].symbol].insert(transitions[index][transitionIndex].node);
        }
    }
}

Transitions::Transitions(const Input &input, const States &state, const Sigma &sigma) {
    std::string matrix[kNodeLimit];
    input.matrix(matrix);

    for (int index = input.findTrans() + 1; ok; ++index) {
        char stateLine[kStringLength];
        if (matrix[index] == "End") {
            break;
        }
        if (matrix[index][0] == '#') {
            continue;
        }

        std::strcpy(stateLine, matrix[index].c_str());
        char *token = std::strtok(stateLine, ", ");
        int node = 0;
        char symbol = '\0';
        int tokenCount = 0;

        while (token && ok) {
            char buffer[kStringLength];
            ++tokenCount;
            std::strcpy(buffer, token);
            if (tokenCount == 1 || tokenCount == 3) {
                if (state.translate(buffer) == -1) {
                    ok = false;
                    break;
                }
                if (tokenCount == 1) {
                    node = state.translate(buffer);
                } else {
                    int destination = state.translate(buffer);
                    transitions[node].push_back({destination, symbol});
                    transitionMap[node][symbol].insert(destination);
                    if (transitionMap[node][symbol].size() > 1) {
                        this->dfa = false;
                    }
                }
            } else if (std::strlen(buffer) > 1 || !sigma.belongsToAlphabet(buffer[0])) {
                ok = false;
            } else {
                symbol = buffer[0];
            }
            token = std::strtok(nullptr, ", ");
        }
    }
}

void Transitions::getTransition(std::map<char, std::set<int>> transitionMap[kNodeLimit]) const {
    for (int index = 0; index < 255; ++index) {
        if (!this->transitionMap[index].empty()) {
            transitionMap[index] = this->transitionMap[index];
        }
    }
}

bool Transitions::validTransitions() const {
    return ok;
}

char Transitions::character(int state, int index) const {
    return transitions[state][index].symbol;
}

int Transitions::size(int state) const {
    return transitions[state].size();
}

int Transitions::node(int state, int index) const {
    return transitions[state][index].node;
}

bool Transitions::isDFA() const {
    return dfa;
}

bool Transitions::isNFA() const {
    return !dfa;
}

std::ostream &operator<<(std::ostream &output, const Transitions &transition) {
    output << "About transitions:\n";

    if (transition.isDFA()) {
        std::cout << "DFA\n";
    } else {
        std::cout << "NFA\n";
    }

    for (int index = 0; index < Transitions::kNodeLimit; ++index) {
        if (!transition.transitions[index].empty()) {
            output << index << ": ";
            for (auto transitionIndex : transition.transitions[index]) {
                output << "[" << transitionIndex.symbol << ","
                       << transitionIndex.node << "] ";
            }
            output << "\n";
        }
    }

    std::cout << "For https://csacademy.com/app/graph_editor/" << std::endl;
    for (int index = 0; index < Transitions::kNodeLimit; ++index) {
        if (!transition.transitions[index].empty()) {
            for (int transitionIndex = 0; transitionIndex < transition.transitions[index].size(); ++transitionIndex) {
                output << index << " " << transition.transitions[index][transitionIndex].node << " "
                       << transition.transitions[index][transitionIndex].symbol << "\n";
            }
        }
    }
    return output;
}

void Transitions::modifyTransitions(std::vector<TransitionNode> transitions[kNodeLimit]) {
    for (int index = 0; index < kNodeLimit; ++index) {
        this->transitions[index] = transitions[index];
    }
    dfa = true;
}

void Transitions::increaseN(int count) {
    for (int index = kNodeLimit - 1; index >= 0; --index) {
        if (!transitions[index].empty()) {
            for (auto transition : transitions[index]) {
                transitions[index + count].push_back({transition.node + count, transition.symbol});
            }
            transitions[index].clear();
        }
    }
}

void Transitions::addTransitions(const std::vector<int> &states, const std::vector<int>& targets) {
    for (auto node : states) {
        for (auto destination : targets) {
            this->transitions[node].push_back({destination, '$'});
        }
    }
}

void Transitions::addTransitions(std::vector<TransitionNode> transitions[kNodeLimit]) {
    for (int index = 0; index < kNodeLimit; ++index) {
        if (!transitions[index].empty()) {
            if (!this->transitions[index].empty()) {
                std::cout << "invalid transition update";
            }
            this->transitions[index] = transitions[index];
        }
    }
}

std::vector<TransitionNode> *Transitions::getTransitions() {
    return transitions;
}

Transitions::~Transitions() = default;
