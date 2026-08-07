//
// Created by Gabriel on 5/22/2025.
//

#include "../include/CFG.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

// Global output stream for derivations
std::ofstream output("output.txt");

// --- Constructors ---

CFG::CFG(std::string &filename) {
    std::ifstream file(filename.c_str());
    char line[205];

    while (file.getline(line, sizeof(line))) {
        if (strcmp(line, "") == 0) {
            continue;
        }

        char *token = strtok(line, " ,->|");
        char symbol = *token;

        if (!start) {
            start = symbol;
        }

        nonTerminals.insert(symbol);
        token = strtok(nullptr, " ,->|");

        while (token) {
            char production[205];
            strcpy(production, token);
            std::vector<char> productionSymbols;

            for (int index = 0; production[index]; ++index) {
                if (terminal(production[index])) {
                    terminals.insert(production[index]);
                } else {
                    nonTerminals.insert(production[index]);
                }
                productionSymbols.push_back(production[index]);
            }

            productions[symbol].push_back(productionSymbols);
            token = strtok(nullptr, " ,->|");
        }
    }

    for (char nonTerminal : nonTerminals) {
        if (productions.find(nonTerminal) == productions.end()) {
            throw std::runtime_error("Non-terminal without productions");
        }
    }

    for (auto & production : productions) {
        int count = 0;
        for (int index = 1; index < productions[production.first].size(); ++index) {
            bool isTerminalOnly = true;
            for (char position : productions[production.first][index]) {
                if (!terminal(position)) {
                    isTerminalOnly = false;
                    break;
                }
            }
            if (isTerminalOnly) {
                std::swap(productions[production.first][count], productions[production.first][index]);
                ++count;
            }
        }
    }
}

// --- Core API / Operations ---

std::vector<std::string> CFG::generate(int maxLen, int maxCount) {
    std::vector<std::string> result;
    int total = 0;
    std::string current;
    current += start;

    generateFromCurrent(maxLen, maxCount, total, result, current);
    visitedStates.clear();
    return result;
}

std::vector<std::string> CFG::derivation(const std::string& target) {
    std::vector<std::string> result;
    bool isFound = false;
    std::string current;
    current += start;

    result.push_back(current);
    deriveFromCurrent(target, result, current, isFound);
    visitedStates.clear();

    if (!isFound) {
        return {"Empty"};
    }
    return result;
}

bool CFG::recognize(const std::string &target) {
    std::vector<std::string> result;
    bool isFound = false;
    std::string current;
    current += start;

    deriveFromCurrent(target, result, current, isFound);
    visitedStates.clear();
    return isFound;
}

// --- Recursive Helpers ---

void CFG::generateFromCurrent(int maxLen, int maxCount, int &count,
                              std::vector<std::string> &result,
                              const std::string &current) {
    bool isTerminalOnly = true;
    if (current.size() / 2 > maxLen) {
        return;
    }

    visitedStates[current] = true;

    for (int index = 0; current[index]; ++index) {
        if (!terminal(current[index])) {
            isTerminalOnly = false;
            char nonTerminal = current[index];
            std::string prefix = current.substr(0, index);
            std::string suffix = current.substr(index + 1);

            for (const auto& production : productions[nonTerminal]) {
                std::string next = prefix;
                for (auto symbol : production) {
                    if (symbol != '$') {
                        next += symbol;
                    }
                }
                next += suffix;

                generateFromCurrent(maxLen, maxCount, count, result, next);
                if (count == maxCount) {
                    break;
                }
            }

            if (count == maxCount) {
                break;
            }
        }
    }

    if (isTerminalOnly && count < maxCount) {
        if (current.length() <= maxLen) {
            result.push_back(current);
            ++count;
        }
    }
}

void CFG::deriveFromCurrent(const std::string &target,
                            std::vector<std::string> &result,
                            const std::string &current,
                            bool &isFound) {
    bool isTerminalOnly = true;
    if (visitedStates.find(current) != visitedStates.end()) {
        return;
    }

    visitedStates[current] = true;
    output << current << "\n";

    if (numberOfTerminals(current) > target.length() ||
        numberOfNonTerminals(current) > target.length()) {
        return;
    }

    for (int index = 0; current[index]; ++index) {
        if (!terminal(current[index])) {
            isTerminalOnly = false;
            char nonTerminal = current[index];
            std::string prefix = current.substr(0, index);
            std::string suffix = current.substr(index + 1);

            for (const auto& production : productions[nonTerminal]) {
                std::string next = prefix;
                for (auto symbol : production) {
                    if (symbol != '$') {
                        next += symbol;
                    }
                }
                next += suffix;

                result.push_back(next);
                if (potential(next, target)) {
                    deriveFromCurrent(target, result, next, isFound);
                }
                if (isFound) {
                    break;
                }
                result.pop_back();
            }

            if (isFound) {
                break;
            }
        }
    }

    if (isTerminalOnly && current == target) {
        isFound = true;
    }
}

// --- Static Helper Methods ---

bool CFG::terminal(char symbol) {
    return !(symbol <= 'Z' && symbol >= 'A');
}

int CFG::numberOfNonTerminals(const std::string &current) {
    int count = 0;
    for (char symbol : current) {
        if (!terminal(symbol)) {
            ++count;
        }
    }
    return count;
}

int CFG::numberOfTerminals(const std::string &current) {
    int count = 0;
    for (int index = 0; current[index]; ++index) {
        if (terminal(current[index])) {
            ++count;
        }
    }
    return count;
}

bool CFG::potential(const std::string &input, const std::string &target) {
    std::string pattern;
    for (auto symbol : input) {
        if (terminal(symbol)) {
            pattern += symbol;
        } else {
            pattern += '*';
        }
    }

    const int targetLength = target.size();
    const int patternLength = pattern.size();
    int index = 0;
    int patternIndex = 0;
    int lastMatch = -1;
    int matchPosition = 0;

    while (index < targetLength) {
        if (patternIndex < patternLength && pattern[patternIndex] == target[index]) {
            ++index;
            ++patternIndex;
        } else if (patternIndex < patternLength && pattern[patternIndex] == '*') {
            lastMatch = patternIndex;
            matchPosition = index;
            ++patternIndex;
        } else if (lastMatch != -1) {
            patternIndex = lastMatch + 1;
            ++matchPosition;
            index = matchPosition;
        } else {
            return false;
        }
    }

    while (patternIndex < patternLength && pattern[patternIndex] == '*') {
        ++patternIndex;
    }
    return patternIndex == patternLength;
}