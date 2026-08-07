//
// Created by Gabriel on 5/22/2025.
//

#include "../include/CFG.h"

#include <algorithm>
#include <fstream>
#include <cstring>
#include <exception>
#include <iostream>

CFG::CFG(std::string &filename) {
    std::ifstream file(filename.c_str());
    char line[205];
    while (file.getline(line, sizeof(line))) {
        if (strcmp(line, "") == 0)
            continue;
        char *p = strtok(line, " ,->|");
        char symbol = *p;
        if (!start)
            start = symbol;
        nonTerminals.insert(symbol);
        p = strtok(NULL, " ,->|");
        while (p) {
            char production[205];
            strcpy(production, p);
            std::vector<char> values;
            for (int i = 0; production[i]; i++) {
                if (terminal(production[i]))
                    terminals.insert(production[i]);
                else nonTerminals.insert(production[i]);
                values.push_back(production[i]);
            }
            prod[symbol].push_back(values);
            p = strtok(NULL, " ,->|");
        }
    }
    for (std::set<char>::iterator it = nonTerminals.begin(); it != nonTerminals.end(); ++it) {
        if (prod.find(*it) == prod.end()) {
            throw std::runtime_error("Non-terminal without productions");
        }
    }
    for (auto it = prod.begin(); it != prod.end(); ++it) {
        int count = 0;
        for (int index = 1; index < prod[it->first].size(); ++index) {
            bool isTerminalOnly = true;
            for (int position = 0; position < prod[it->first][index].size(); ++position) {
                if (!terminal(prod[it->first][index][position])) {
                    isTerminalOnly = false;
                    break;
                }
            }
            if (isTerminalOnly) {
                std::swap(prod[it->first][count], prod[it->first][index]);
                count++;
            }
        }
    }
}

int CFG::numberOfNonTerminals(const std::string &current) {
    int count = 0;
    for (char symbol : current) {
        if (!terminal(symbol))
            count++;
    }
    return count;
}

bool CFG::terminal(char symbol) {
    return !(symbol <= 'Z' && symbol >= 'A');
}

int CFG::numberOfTerminals(const std::string &current) {
    int count = 0;
    for (int i = 0; current[i]; i++) {
        if (terminal(current[i]))
            count++;
    }
    return count;
}

void CFG::dfsGenerate(int maxLen, int maxCount, int &count, std::vector<std::string> &result, std::string current) {
    bool isTerminalOnly = true;
    if (current.size() / 2 > maxLen) {
        return;
    }
    temp[current] = true;
    for (int i = 0; current[i]; i++) {
        if (!terminal(current[i])) {
            isTerminalOnly = false;
            char nonTerminal = current[i];
            std::string prefix = current.substr(0, i), suffix = current.substr(i + 1);
            for (auto production : prod[nonTerminal]) {
                std::string next = prefix;
                for (auto symbol : production)
                    if (symbol != '$')
                        next += symbol;
                next += suffix;
                dfsGenerate(maxLen, maxCount, count, result, next);
                if (count == maxCount)
                    break;
            }
            if (count == maxCount)
                break;
        }
    }
    if (isTerminalOnly && count < maxCount) {
        if (current.length() <= maxLen)
            result.push_back(current), count++;
    }
}

std::vector<std::string> CFG::generate(int maxLen, int maxCount) {
    std::vector<std::string> result;
    int total = 0;
    std::string current;
    current += start;
    dfsGenerate(maxLen, maxCount, total, result, current);
    temp.clear();
    return result;
}

bool CFG::potential(const std::string &input, const std::string &target) {
    std::string pattern;
    for (auto symbol : input)
        if (terminal(symbol))
            pattern += symbol;
        else pattern += '*';
    int n = target.size(), m = pattern.size();
    int i = 0, j = 0;
    int last = -1, match = 0;

    while (i < n) {
        if (j < m && (pattern[j] == target[i])) {
            i++;
            j++;
        }
        else if (j < m && pattern[j] == '*') {
            last = j;
            match = i;
            j++;
        }
        else if (last != -1) {
            j = last + 1;
            match++;
            i = match;
        }
        else {
            return false;
        }
    }
    while (j < m && pattern[j] == '*') {
        j++;
    }
    return j == m;
}

std::ofstream output("output.txt");

void CFG::dfsDerivation(const std::string &target, std::vector<std::string> &result, std::string &current, bool &isFound) {
    bool isTerminalOnly = true;
    if (temp.find(current) != temp.end())
        return;
    temp[current] = true;
    output << current << "\n";
    if (numberOfTerminals(current) > target.length() || numberOfNonTerminals(current) > target.length()) {
        return;
    }
    for (int i = 0; current[i]; i++) {
        if (!terminal(current[i])) {
            isTerminalOnly = false;
            char nonTerminal = current[i];
            std::string prefix = current.substr(0, i), suffix = current.substr(i + 1);
            for (auto production : prod[nonTerminal]) {
                std::string next = prefix;
                for (auto symbol : production)
                    if (symbol != '$')
                        next += symbol;
                next += suffix;
                result.push_back(next);
                if (potential(next, target))
                    dfsDerivation(target, result, next, isFound);
                if (isFound)
                    break;
                result.pop_back();
            }
            if (isFound)
                break;
        }
    }
    if (isTerminalOnly && current == target) {
        isFound = true;
    }
}

std::vector<std::string> CFG::derivation(std::string target) {
    std::vector<std::string> result;
    bool isFound = false;
    std::string current;
    current += start;
    result.push_back(current);
    dfsDerivation(target, result, current, isFound);
    temp.clear();
    if (!isFound)
        return {"Empty"};
    return result;
}

bool CFG::recognize(std::string &target) {
    std::vector<std::string> result;
    bool isFound = false;
    std::string current;
    current += start;
    dfsDerivation(target, result, current, isFound);
    temp.clear();
    return isFound;
}

