//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Transitions.h"

Transitions &Transitions::operator=(const Transitions &other) {
    // std::cout << "Operator egal\n";
    this->ok = other.ok;
    this->dfa = other.dfa;
    this->ct = other.ct;
    for (int i = 0; i < n; i++)
        this->v[i] = other.v[i], this->w[i] = other.w[i], this->cuv[i] = other.cuv[i];
    return *this;
}
Transitions::Transitions(char value) {
    v[1].push_back({2, value});
    w[1][value].insert(2);
}
Transitions::Transitions(const Transitions &other) {
    //std::cout << "Constructor de Copiere\n";
    this->ok = other.ok;
    this->dfa = other.dfa;
    this->ct = other.ct;
    for (int i = 0; i < n; i++)
        this->v[i] = other.v[i], this->w[i] = other.w[i], this->cuv[i] = other.cuv[i];
}

Transitions::Transitions(const vector<nu> v[n]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < v[i].size(); j++) {
            this->v[i].push_back(v[i][j]);
            w[i][v[i][j].a].insert(v[i][j].nod);
        }
    }
}

Transitions::Transitions(const Input &citire, const States &state, const Sigma &sigma) {
    char s[m];
    string mat[n];
    citire.Matrice(mat);
    for (int i = citire.findTrans() + 1; ok; i++) {
        if (mat[i] == "End")
            break;
        if (mat[i][0] == '#')
            continue;
        strcpy(s, mat[i].c_str());
        char *p = strtok(s, ", "), cuv[m];
        int nod;
        char litera;
        int ct = 0;
        while (p && ok) {
            ct++;
            strcpy(cuv, p);
            if (ct == 1 || ct == 3) {
                if (state.translate(cuv) == -1) {
                    ok = false;
                    break;
                }
                if (ct == 1)
                    nod = state.translate(cuv);
                else {
                    int dest = state.translate(cuv);
                    v[nod].push_back({dest, litera});
                    w[nod][litera].insert(dest);
                    if (w[nod][litera].size() > 1)
                        this->dfa = false;
                }
            } else if (strlen(cuv) > 1 || !sigma.apartineAlfabet(cuv[0]))
                ok = false;
            else litera = cuv[0];
            p = strtok(NULL, ", ");
        }
    }
}

void Transitions::getTransition(map<char, set<int> > w[n]) const {
    for (int i = 0; i < 255; i++) {
        if (this->w[i].size() > 0)
            w[i] = this->w[i];
    }
}

bool Transitions::validTransitions() const {
    return ok;
}

char Transitions::caracter(int stare, int i) const {
    return v[stare][i].a;
}

int Transitions::Size(int stare) const {
    return v[stare].size();
}

int Transitions::nod(int stare, int i) const {
    return v[stare][i].nod;
}

bool Transitions::isDFA() const {
    return dfa;
}

bool Transitions::isNFA() const {
    return !dfa;
}

ostream &operator<<(ostream &os, const Transitions &a) {
    os << "Despre Transitions: \n";

    if (a.isDFA())
        cout << "DFA\n";
    else cout << "NFA\n";
    for (int i = 0; i < Transitions::n; i++)
        if (a.v[i].size() != 0) {
            os << i << ": ";
            for (int j = 0; j < a.v[i].size(); j++)
                os << "[" << a.v[i][j].a << "," << a.v[i][j].nod << "] ";
            os << "\n";
        }

    cout << "Pentru https://csacademy.com/app/graph_editor/" << endl;
    for (int i = 0; i < Transitions::n; i++)
        if (a.v[i].size() != 0) {
            //os << i << ": ";
            for (int j = 0; j < a.v[i].size(); j++)
                os << i << " " << a.v[i][j].nod << " " << a.v[i][j].a << "\n";
          //  os << "\n";
        }
    return os;
}

void Transitions::modificareTrans(vector<nu> v[n]) {
    for (int i = 0; i < n; i++) {
        this->v[i] = v[i];
    }
    dfa = true;
}

void Transitions::increaseN(int n) {
    for (int i = this->n - 1; i >= 0; i--)
        if (v[i].size() != 0) {
            for (auto tran: v[i]) {
                v[i + n].push_back({tran.nod + n, tran.a});
            }
            v[i].clear();
        }
}


void Transitions::addTransitions(const vector<int>& stari, vector<int> v) {
    for (auto nod : stari) {
        for (auto dest: v)
            this->v[nod].push_back({dest, '$'});
    }
}

void Transitions::addTransitions(vector<nu> v[n]) {
    for (int i = 0; i < n; i++)
        if (v[i].size() != 0) {
            if (this->v[i].size() != 0)
                cout << "cevaRau";
            this->v[i] = v[i];
        }
}

vector<nu> *Transitions::get_transitions() {
    return v;
}

Transitions::~Transitions() {
    //cout << "Destructor";
}
