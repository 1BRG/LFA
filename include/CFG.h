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
    std::map<char, std::vector <std::vector<char>>> prod;
    std::map<std::string, bool> temp;
    char start{NULL};
    std::set<char> terminals, nonTerminals;
public:
    CFG() = default;
    explicit CFG(std::string &filename);

    int numberOfNonTerminals(const std::string &current);

    static bool terminal(char c);

    static int numberOfTerminals(const std::string &current);

    void dfsGenerate(int maxLen, int maxCount, int &count, std::vector<std::string> &result, std::string current);

    std::vector<std::string> generate(int max, int count);

    static bool potential(const std::string &input, const std::string &target);

    void dfsDerivation(const std::string &target, std::vector<std::string> &result, std::string &current, bool &isFound);

    std::vector<std::string> derivation(std::string target);

    bool recognize(std::string &target);
};



#endif //CFG_H
