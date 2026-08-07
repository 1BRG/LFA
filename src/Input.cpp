//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Input.h"

Input::Input(const std::string &filename) {
    std::ifstream inputFile(filename);
    int currentIndex = 0;
    std::string line;

    while (std::getline(inputFile, line)) {
        if (currentIndex >= kLineCount) {
            break;
        }
        lines[currentIndex++] = line;
    }
}

int Input::findState() const {
    for (int index = 0; true; ++index) {
        if (lines[index] == "States:") {
            return index;
        }
    }
}

int Input::findTrans() const {
    for (int index = 0; true; ++index) {
        if (lines[index] == "Transitions:") {
            return index;
        }
    }
}

int Input::findSigma() const {
    for (int index = 0; true; ++index) {
        if (lines[index] == "Sigma:") {
            return index;
        }
    }
}

void Input::matrix(std::string copy[]) const {
    for (int index = 0; index < kLineCount; ++index) {
        copy[index] = lines[index];
    }
}

std::ostream &operator<<(std::ostream &output, const Input &input) {
    output << "About input\n";
    output << "States begin at line: " << input.findState() << "\n";
    output << "Sigma begins at line: " << input.findSigma() << "\n";
    output << "Transitions begin at line: " << input.findTrans() << "\n";
    return output;
}

Input &Input::operator=(const Input &other) {
    for (int index = 0; index < kLineCount; ++index) {
        this->lines[index] = other.lines[index];
    }
    return *this;
}

Input::Input(const Input &other) {
    for (int index = 0; index < kLineCount; ++index) {
        this->lines[index] = other.lines[index];
    }
}

Input::~Input() = default;