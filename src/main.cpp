#include <deque>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>
#include <map>
#include <set>
#include "../include/json.hpp"
#include "../include/Automat.h"
#include "../include/CFG.h"
using namespace std;
using json = nlohmann::json;

int o = 0;
char cuv[50];
void tema1() {
    const int n = 2000, m = 50;
    char cuv[n];
    Automat a, b;

    Input citire_aux("../input.txt");
    Input citire;
    citire = citire_aux;
    // cout << citire << "\n";
    States S{citire};
    // cout << S << "\n";
    Sigma A(citire);
    // cout << A << "\n";
    Transitions T(citire, S, A);
    // cout << T << "\n";
    a = Automat{citire};
    b = a;
    if (a.isValid() == false) {
        exit(0);
    }
    cout << b;
    a.toDFA();
    cout << "\n\n Automat to DFA\n\n";
    cout << a;
    while (true) {
        cout << "\n\n Introduceti cuvantul pentru verificare:\n\n";
        cin >> cuv;
        cout << "(?)DFA: " << a.cuvant(cuv) << "\n*NFA / (DFA initial): " << b.cuvant(cuv) << "\n\n";
    }
    exit(0);
}


void tema2()
{
    ifstream file("../src/LFA-Assignment2_Regex_DFA.json");
    auto data = json::parse(file);
    for (auto ex : data) {
        cout << "\n\nName: " << ex["name"] << "\n" << "Regex: " << ex["regex"] << "\n";
        string regex = ex["regex"];
        Automat a(regex);
        a.toDFA();
       // cout << a;
        for (auto test : ex["test_strings"]) {
            string cuv = test["input"];
            bool rez = a.apartine_automat(cuv);
            bool exp = test["expected"];
            cout << test["input"] << " : " << ((rez == 1) ? "true" : "false") << " corect: " << test["expected"] << "\n";
            if ( rez != exp) {cout << "TIPAAAAAAA!"; exit(4 );};
        }
    }
}

void tema3() {
    string filename = "../src/aSb.in";
    ifstream f(filename);
    if (!f.is_open()) {
        cout << "File doesn't exist: " << filename << "\n";
    }
    CFG a{filename};
    vector<string> Gen = a.generate(10, 10);
    cout << "Cuvinte generate de gramatica (maxim 10 cuvinte, fiecare cu maxim 10 caractere) : \n";
    for (int i = 0; i < Gen.size(); i ++)
        cout << i + 1 << ")" << Gen[i] << "\n";
    string cuv = "aaabbb";
    vector<string> Der = a.derivation(cuv);
    cout << "Derivarea cuvantului \"" << cuv << "\"\n";
    for (int i = 0; i < Der.size(); i ++) {
        cout << Der[i];
        if (i != Der.size() - 1)
            cout << " -> ";
        else cout << "\n";
    }
    string cuv1 = "aaaabbb";
    bool ok = a.recognize(cuv1);
    cout << "Cuvantul \"" << cuv1 << "\" apartine gramaticii: ";
    if (ok == true)
        cout << "Adevarat";
    else cout << "Fals";
    cout << "\n";
}
void bonusTema3() {
    string filename = "../src/aNbNcN.in";
    ifstream f(filename);
    if (!f.is_open()) {
        cout << "File doesn't exist: " << filename << "\n";
    }
    /*
     Gramatica explicata la seminar folosind lema de pompare de ce nu este independenta de context
     Dar intuitiv ideea este nu putem implementa un automat push_down pentru ca am avea nevoie de doua stive
     pentru a tine minte numarul de a, b, c si asta rezulta ca gramatica nu e independenta de context
     */
    CFG b{filename};
    vector <string> cuv = {"aabcc", "aabbc", "aaabbbccc", "bac"};
    for (auto c: cuv) {
        auto verif = [](string &cuv) {
            int ct = 0, ct1 = 0;
            int i = 0;
            if (cuv.length() < 3)
                return false;
            while (cuv[i] == 'a')
                ct ++, i ++;
            ct1 = ct;
            while (cuv[i] == 'b')
                ct --, i ++;
            if (ct)
                return false;
            while (cuv[i] == 'c')
                ct ++, i ++;
            if (ct != ct1)
                return false;
            return true;
        };
        cout << "Cuvantul \"" << c << "\" apartine gramaticii? \n";
        cout << "CFG recognizer: ";
        bool ok = b.recognize(c);
        if (ok == true)
            cout << "Adevarat";
        else cout << "Fals";
        cout << "\nFunctia de verificare:";
        bool ok1 = verif(c);
        if (ok1 == true)
            cout << "Adevarat";
        else cout << "Fals";
        cout << "\n";
        if (ok1) {
            cout << "Derivarea este:\n";
            vector <string> Der = b.derivation(c);
            for (int i = 0; i < Der.size(); i ++) {
                cout << Der[i];
                if (i != Der.size() - 1)
                    cout << " -> ";
                else cout << "\n";
            }
        }
        else {
            cout << "Deci cuvantul NU apartine gramticii!\n";
        }
        cout << "\n";
    }

}
void cfgOarecare() {
    string s, filename = "../src/CFG.in";
    string s1;
    std::cout << "Citeste din CFG.in ? (Nu/Da)" << std::endl;
    std::cin >> s1;
    if (s1 == "Nu") {
        cout << "Introduceti gramatica: (STOP la final)" << std::endl;
        ofstream g(filename);
        while (getline(cin, s) && s != "STOP")
            g << s << "\n";
        g.close();
    }
    CFG a(filename);
    vector<string> Gen = a.generate(10, 10);
    cout << "Cuvinte generate de gramatica (maxim 10 cuvinte, fiecare cu maxim 10 caractere) : \n";
    for (int i = 0; i < Gen.size(); i ++)
        cout << i + 1 << ")" << Gen[i] << "\n";
    cout << "Introduceti cuvinte:" << std::endl;
    while (getline(cin , s) && s != "STOP") {
        cout << "CFG recognizer: ";
        bool ok = a.recognize(s);
        cout << "Cuvantul \"" << s << "\" apartine gramaticii: ";
        if (ok == true)
            cout << "Adevarat";
        else cout << "Fals";
        cout << "\n\n";
    }
}

int main()
{
    tema3();
    bonusTema3();
    //cfgOarecare();
}