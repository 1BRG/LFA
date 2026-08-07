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
    std::map<char, std::vector<std::vector<char>>> productions;
    std::map<std::string, bool> visitedStates;
    char start{'\0'};
    std::set<char> terminals, nonTerminals;
public:
    CFG() = default;
    explicit CFG(std::string &filename);

    static int numberOfNonTerminals(const std::string &current);

    static bool terminal(char c);

    static int numberOfTerminals(const std::string &current);

    void generateFromCurrent(int maxLen, int maxCount, int &count,
                             std::vector<std::string> &result,
                             const std::string &current);

    std::vector<std::string> generate(int max, int count);

    static bool potential(const std::string &input, const std::string &target);

    void deriveFromCurrent(const std::string &target,
                           std::vector<std::string> &result,
                           const std::string &current,
                           bool &isFound);

    std::vector<std::string> derivation(const std::string& target);

    bool recognize(const std::string &target);
};



#endif //CFG_H
