//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Input.h"

Input::Input(const string &filename) {
    ifstream inputFile(filename);
    int index = 0;
    string line;
    while (getline(inputFile, line)) {
        if (index >= n)
            break;
        lines[index++] = line;
    }
}

int Input::findState() const {
    for (int i = 0; true; i++)
        if (lines[i] == "States:")
            return i;
}

int Input::findTrans() const {
    for (int i = 0; true; i++)
        if (lines[i] == "Transitions:")
            return i;
}

int Input::findSigma() const {
    for (int i = 0; true; i++)
        if (lines[i] == "Sigma:")
            return i;
}

void Input::matrix(string copy[]) const {
    for (int i = 0; i < n; i++)
        copy[i] = lines[i];
}

ostream &operator<<(ostream &os, const Input &a) {
    os << "About input\n";
    os << "States begin at line: " << a.findState() << "\n";
    os << "Sigma begins at line: " << a.findSigma() << "\n";
    os << "Transitions begin at line: " << a.findTrans() << "\n";
    return os;
}

Input &Input::operator=(const Input &other) {
    for (int i = 0; i < n; i++)
        this->lines[i] = other.lines[i];
    return *this;
}

Input::Input(const Input &other) {
    for (int i = 0; i < n; i++)
        this->lines[i] = other.lines[i];
}

Input::~Input() {
}