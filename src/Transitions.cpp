//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Transitions.h"

Transitions &Transitions::operator=(const Transitions &other) {
    this->ok = other.ok;
    this->dfa = other.dfa;
    this->count = other.count;
    for (int i = 0; i < n; i++)
        this->transitions[i] = other.transitions[i], this->w[i] = other.w[i], this->word[i] = other.word[i];
    return *this;
}

Transitions::Transitions(char value) {
    transitions[1].push_back({2, value});
    w[1][value].insert(2);
}

Transitions::Transitions(const Transitions &other) {
    this->ok = other.ok;
    this->dfa = other.dfa;
    this->count = other.count;
    for (int i = 0; i < n; i++)
        this->transitions[i] = other.transitions[i], this->w[i] = other.w[i], this->word[i] = other.word[i];
}

Transitions::Transitions(const vector<TransitionNode> transitions[n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < transitions[i].size(); j++) {
            this->transitions[i].push_back(transitions[i][j]);
            w[i][transitions[i][j].symbol].insert(transitions[i][j].node);
        }
    }
}

Transitions::Transitions(const Input &input, const States &state, const Sigma &sigma) {
    char stateLine[m];
    string matrix[n];
    input.matrix(matrix);
    for (int i = input.findTrans() + 1; ok; i++) {
        if (matrix[i] == "End")
            break;
        if (matrix[i][0] == '#')
            continue;
        strcpy(stateLine, matrix[i].c_str());
        char *p = strtok(stateLine, ", "), token[m];
        int node;
        char symbol;
        int count = 0;
        while (p && ok) {
            count++;
            strcpy(token, p);
            if (count == 1 || count == 3) {
                if (state.translate(token) == -1) {
                    ok = false;
                    break;
                }
                if (count == 1)
                    node = state.translate(token);
                else {
                    int destination = state.translate(token);
                    transitions[node].push_back({destination, symbol});
                    w[node][symbol].insert(destination);
                    if (w[node][symbol].size() > 1)
                        this->dfa = false;
                }
            } else if (strlen(token) > 1 || !sigma.belongsToAlphabet(token[0]))
                ok = false;
            else symbol = token[0];
            p = strtok(NULL, ", ");
        }
    }
}

void Transitions::getTransition(map<char, set<int>> w[n]) const {
    for (int i = 0; i < 255; i++) {
        if (this->w[i].size() > 0)
            w[i] = this->w[i];
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

ostream &operator<<(ostream &os, const Transitions &a) {
    os << "About transitions:\n";

    if (a.isDFA())
        cout << "DFA\n";
    else cout << "NFA\n";
    for (int i = 0; i < Transitions::n; i++)
        if (a.transitions[i].size() != 0) {
            os << i << ": ";
            for (int j = 0; j < a.transitions[i].size(); j++)
                os << "[" << a.transitions[i][j].symbol << "," << a.transitions[i][j].node << "] ";
            os << "\n";
        }

    cout << "For https://csacademy.com/app/graph_editor/" << endl;
    for (int i = 0; i < Transitions::n; i++)
        if (a.transitions[i].size() != 0) {
            for (int j = 0; j < a.transitions[i].size(); j++)
                os << i << " " << a.transitions[i][j].node << " " << a.transitions[i][j].symbol << "\n";
        }
    return os;
}

void Transitions::modifyTransitions(vector<TransitionNode> transitions[n]) {
    for (int i = 0; i < n; i++) {
        this->transitions[i] = transitions[i];
    }
    dfa = true;
}

void Transitions::increaseN(int count) {
    for (int i = this->n - 1; i >= 0; i--)
        if (transitions[i].size() != 0) {
            for (auto transition: transitions[i]) {
                transitions[i + count].push_back({transition.node + count, transition.symbol});
            }
            transitions[i].clear();
        }
}

void Transitions::addTransitions(const vector<int> &states, vector<int> targets) {
    for (auto node : states) {
        for (auto destination: targets)
            this->transitions[node].push_back({destination, '$'});
    }
}

void Transitions::addTransitions(vector<TransitionNode> transitions[n]) {
    for (int i = 0; i < n; i++)
        if (transitions[i].size() != 0) {
            if (this->transitions[i].size() != 0)
                cout << "invalid transition update";
            this->transitions[i] = transitions[i];
        }
}

vector<TransitionNode> *Transitions::getTransitions() {
    return transitions;
}

Transitions::~Transitions() {
}
