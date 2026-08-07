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
    std::set<char> terminale, neterminale;
public:
    CFG() = default;
    explicit CFG(std::string &filename);

    int nrNonTerminale(const std::string &curr);

    static bool terminal(char c);

    static int nrTerminale(const std::string & curr);

    void dfsGen(int maxLen, int maxCt, int &ct, std::vector<std::string> &rez, std::string curr);

    std::vector<std::string> generate(int max, int ct);

    static bool potential(const std::string & string, const std::string & tinta);

    void dfsDer(const std::string &tinta, std::vector<std::string> &rez, std::string &curr, bool &isIn);

    std::vector<std::string> derivation(std::string tinta);

    bool recognize(std::string &tinta);
};



#endif //CFG_H
