//
// Created by Gabriel on 5/22/2025.
//

#ifndef CFG_H
#define CFG_H

#include <map>
#include <set>
#include <string>
#include <vector>

class CFG {
private:
    std::map<char, std::vector<std::vector<char>>> productions;
    std::map<std::string, bool> visitedStates;
    char start{'\0'};
    std::set<char> terminals, nonTerminals;

public:
    // --- Constructors ---
    CFG() = default;
    explicit CFG(std::string &filename);

    // --- Core API / Operations ---
    std::vector<std::string> generate(int maxLen, int maxCount);
    std::vector<std::string> derivation(const std::string& target);
    bool recognize(const std::string &target);

    // --- Recursive Helpers ---
    void generateFromCurrent(int maxLen, int maxCount, int &count,
                             std::vector<std::string> &result,
                             const std::string &current);

    void deriveFromCurrent(const std::string &target,
                           std::vector<std::string> &result,
                           const std::string &current,
                           bool &isFound);

    // --- Static Helper Methods ---
    static bool terminal(char symbol);
    static int numberOfNonTerminals(const std::string &current);
    static int numberOfTerminals(const std::string &current);
    static bool potential(const std::string &input, const std::string &target);
};

#endif //CFG_H