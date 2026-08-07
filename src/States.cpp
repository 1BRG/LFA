//
// Created by Gabriel on 4/16/2025.
//

#include "../include/States.h"

States &States::operator=(const States &other) {
    for (int i = 0; i < n; i++)
        this->final[i] = other.final[i];
    this->start = other.start;
    this->ok = other.ok;
    this->q = other.q;
    this->nodeCount = other.nodeCount;
    return *this;
}

States::States(char value): nodeCount(2), start(1) {
    final[2] = true;
    q[to_string(1)] = 1;
    q[to_string(2)] = 2;
}

States::States(const States &other) {
    for (int i = 0; i < n; i++)
        this->final[i] = other.final[i];
    this->start = other.start;
    this->ok = other.ok;
    this->q = other.q;
    this->nodeCount = other.nodeCount;
}

States::States(int start, bool final1[]) {
    this->start = start;
    memcpy(this->final, final1, sizeof(final));
    for (int i = 0; i < n; i++)
        q[to_string(i)] = i;
}

States::States(const Input &input) {
    for (int i = 0; i < n; i++)
        final[i] = false;
    string matrix[n];
    input.matrix(matrix);
    char stateLine[m];
    int count = 0;
    bool hasStartState = false;
    for (int i = input.findState() + 1; true; i++) {
        if (matrix[i] == "End")
            break;
        if (matrix[i][0] == '#')
            continue;
        strcpy(stateLine, matrix[i].c_str());
        char *p = strtok(stateLine, ", ");
        int node;
        int stateIndex = 0;
        while (p) {
            char word[m];
            strcpy(word, p);
            if (word[0] == 'S' && stateIndex != 0) {
                if (start != 0)
                    ok = false;
                else start = node, hasStartState = true;
            } else if (word[0] == 'F' && stateIndex != 0)
                final[node] = true;
            else
                node = q[word] != 0 ? q[word] : ++count, q[word] = count;
            stateIndex += 1;
            p = strtok(NULL, ", ");
        }
    }
    if (!hasStartState)
        ok = false;
}

bool States::validStates() const {
    return ok;
}

int States::translate(const string &node) const {
    map<string, int>::const_iterator it = q.find(node);
    if (it != q.end())
        return it->second;
    else return -1;
}

bool States::isFinalState(int state) const {
    return final[state];
}

int States::startNode() const {
    return this->start;
}

ostream &operator<<(ostream &os, const States &a) {
    os << "About states:\n";
    if (a.ok == false || (a.start == 0)) {
        os << "States invalid\n";
        return os;
    }
    os << "Start node: " << a.start << "\n";
    os << "Final nodes: ";
    int count = 0;
    for (int i = 0; i < States::n; i++)
        if (a.final[i])
            os << i << " ", count += 1;
    if (!count)
        os << "The automaton has no final states";
    os << "\n";
    return os;
}

void States::updateNodeCount(int count) {
    if (nodeCount == 0)
        nodeCount = q.size();
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
    for (int i = this->n; i >= 0; i--) {
        if (final[i])
            final[i + count] = 1;
        final[i] = 0;
    }
    start += count;
    updateNodeCount(count);
}

vector<int> States::initialStates() {
    vector<int> states;
    states.push_back(start);
    return states;
}

void States::changeFinalStates(const vector<int> &states) {
    for (int i = 0; i < n; i++)
        final[i] = 0;
    for (auto node: states)
        final[node] = 1;
}

vector<int> States::finalStates() {
    vector<int> states;
    for (int i = 0; i < n; i++)
        if (final[i])
            states.push_back(i);
    return states;
}

States::~States() {
}
