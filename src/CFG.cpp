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
    std::ifstream f(filename.c_str());
    char s[205];
    while (f.getline(s, sizeof(s))) {
        if (strcmp(s, "") == 0)
            continue;
        char *p = strtok(s, " ,->|");
        char simbol = *p;
        if (!start)
            start = simbol;
        neterminale.insert(simbol);
        p = strtok(NULL, " ,->|");
        while (p) {
            char c[205];
            strcpy(c, p);
            std::vector <char> v;
            for (int i = 0; c[i]; i ++) {
                if (terminal(c[i]))
                    terminale.insert(c[i]);
                else neterminale.insert(c[i]);
                v.push_back(c[i]);
            }
            prod[simbol].push_back(v);
            p = strtok(NULL, " ,->|");
        }
    }
    for (std::set<char> :: iterator it = neterminale.begin(); it != neterminale.end(); it++) {
        if (prod.find(*it) == prod.end()) {
            throw std::runtime_error("Neterminal fara productii");
        }
    }
    for (auto i = prod.begin(); i != prod.end(); i++) {
        int ct = 0;
        for (int j = 1; j < prod[i->first].size(); j ++) {
            bool ok = true;
            for (int p = 0; p < prod[i->first][j].size(); p ++) {
                if (!terminal(prod[i->first][j][p])) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                std::swap(prod[i->first][ct], prod[i->first][j]);
                ct ++;
            }
        }
    }


}
int CFG::nrNonTerminale(const std::string &curr) {
    int ct = 0;
    for (char c : curr) {
        if (!terminal(c))
            ct++;
    }
    return ct;
}
bool CFG::terminal(char i) {
    if (!(i <= 'Z' && i >= 'A'))
        return true;
    return false;
}

int CFG::nrTerminale(const std::string &curr) {
    int ct = 0;
    for (int i = 0; curr[i]; i ++) {
        if (terminal(curr[i]))
            ct ++;
    }
    return ct;
}

void CFG::dfsGen(int maxLen, int maxCt, int &ct, std::vector<std::string> &rez, std::string curr) {
    bool ok = true;
    if (curr.size() / 2 > maxLen) {
        return;
    }
    temp[curr] = true;
    for (int i = 0; curr[i]; i ++) {
        if (!terminal(curr[i]))
        {
            ok = false;
            char non = curr[i];
            std::string inceput = curr.substr(0, i), final = curr.substr(i + 1);
            for (auto p : prod[non])
            {
                std::string s = inceput;
                for (auto c : p)
                    if (c != '$')
                        s += c;
                s += final;
                dfsGen(maxLen, maxCt, ct, rez, s);
                if (ct == maxCt)
                    break;
            }
            if (ct == maxCt)
                break;
        }
    }
    if (ok && ct < maxCt) {
        if (curr.length() <= maxLen)
            rez.push_back(curr), ct ++;
    }
}
std::vector<std::string> CFG::generate(int maxLen, int maxCt) {
    std::vector<std::string> rez;
    int total = 0;
    std::string curr;
    curr += start;
    dfsGen(maxLen, maxCt, total, rez, curr);
    temp.clear();
    return rez;
}

bool CFG::potential(const std::string &str, const std::string &tinta) {
    std::string s;
    for (auto c: str)
        if (terminal(c))
            s += c;
        else s += '*';
    int n = tinta.size(), m = s.size();
    int i = 0, j = 0;
    int last = -1, match = 0;

    while (i < n) {
        if (j < m && (s[j] == tinta[i])) {
            i++;
            j++;
        }
        else if (j < m && s[j] == '*') {
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
    while (j < m && s[j] == '*') {
        j++;
    }
    return j == m;
}

std::ofstream cout("afara");
void CFG::dfsDer(const std::string &tinta, std::vector<std::string> &rez, std::string &curr, bool &isIn) {
    bool ok = true;
    if (temp.find(curr) != temp.end())
        return;
    temp[curr] = true;
    cout << curr << "\n";
    if (nrTerminale(curr) > tinta.length() || nrNonTerminale(curr) > tinta.length()) {
        return;
    }
    for (int i = 0; curr[i]; i ++) {
        if (!terminal(curr[i]))
        {
            ok = false;
            char non = curr[i];
            std::string inceput = curr.substr(0, i), final = curr.substr(i + 1);
            for (auto p : prod[non])
            {
                std::string s = inceput;
                for (auto c : p)
                    if (c != '$')
                        s += c;
                s += final;
                rez.push_back(s);
                if (potential(s, tinta))
                  dfsDer(tinta, rez, s, isIn);
                if (isIn)
                    break;
                rez.pop_back();
            }
            if (isIn)
                break;
        }
    }
    if (ok && curr == tinta) {
        isIn = true;
    }
}

std::vector<std::string> CFG::derivation(std::string tinta) {
    std::vector<std::string> rez;
    bool ok = false;
    std::string curr;
    curr += start;
    rez.push_back(curr);
    dfsDer(tinta, rez, curr, ok);
    temp.clear();
    if (!ok)
        return {"Empty"};
    return rez;
}
bool CFG::recognize(std::string &tinta) {
    std::vector<std::string> rez;
    bool ok = false;
    std::string curr;
    curr += start;
    dfsDer(tinta, rez, curr, ok);
    temp.clear();
    return ok;
}

