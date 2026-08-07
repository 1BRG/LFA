//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Automat.h"


void Automat::dfs(bool &valid, char cuv[], int stare, int poz, int len) const {
    if (valid)
        return;
    if (poz == len) {
        if (S.stareFinala(stare))
            valid = true;
        for (int i = 0; i < T.Size(stare); i++) {
            char ch = T.caracter(stare, i);
            int nod = T.nod(stare, i);
            if (!valid && ch == '$')
                dfs(valid, cuv, nod, poz, len);
        }
        return;
    }
    for (int i = 0; i < T.Size(stare); i++) {
        char ch = T.caracter(stare, i);
        int nod = T.nod(stare, i);
        if (!valid && (ch == cuv[poz]))
            dfs(valid, cuv, nod, poz + 1, len);
        else if (!valid && ch == '$')
            dfs(valid, cuv, nod, poz, len);
    }
}

Automat::Automat(char value): A(value),  T(value), S(value){

}

Automat &Automat::operator=(const Automat &other) {
    // std::cout << "Operator egal\n";
    this->S = other.S;
    this->T = other.T;
    this->A = other.A;
    this->ok = other.ok;
    return *this;
}

Automat::Automat(const Automat &other) {
    //std::cout << "Constructor de Copiere\n";
    this->S = other.S;
    this->T = other.T;
    this->A = other.A;
    this->ok = other.ok;
}
void Automat::parsareRegex(string &regex) {
        for (int i = 0; regex[i]; i ++) {
            if (i != regex.length() - 1) {
                if (!strchr("(|.", regex[i]) && !strchr("|*.+?)", regex[i + 1])) {
                    regex.insert(regex.begin() + i + 1, '.');
                }
            }
        }
}
deque <string> Automat::postfixat(string &s)
{
    map<string, int> prec;
    prec["+"] = prec["*"] = prec["?"] = 3;
    prec["."] = 2;
    prec["|"] = 1;
    prec["("] = -1;
    long long nr = 0;
    deque <string> rez, q;
    for(int i = 0; s[i]; i ++)
    {
        if(s[i] <= '9' && s[i] >= '0')
        {
            while(s[i] <= '9' && s[i] >= '0')
            {
                nr = nr * 10 + s[i] - '0';
                i ++;
            }
            i --;
            rez.push_back(to_string(nr));
            nr = 0;
        }
        else if (s[i] <= 'z' && s[i] >= 'a') {
            rez.push_back(string(1, s[i]));
        }
        else
        {
            string c;
            c += s[i];
            if(s[i] == '(')
                q.push_back(c);
            else if(s[i] == ')')
            {
                while(q.back() != "(")
                    rez.push_back(q.back()), q.pop_back();
                q.pop_back();
            }
            else
            {
                while(!q.empty() && prec[c] <= prec[q.back()])
                    rez.push_back(q.back()), q.pop_back();
                q.push_back(c);
            }
        }
    }
    while(!q.empty())
        rez.push_back(q.back()), q.pop_back();
    return rez;
}
Automat :: Automat(string &regex) {
    parsareRegex(regex);
    deque <string> q = postfixat(regex);
    *this = toAutomat(q);
}
bool Automat::apartine_automat(string &cuv) {
    char c[2000];
    strcpy(c, cuv.c_str());
    return cuvant(c);
}
Automat Automat:: toAutomat(deque <string> p) {
    /*
     : . -> concatenare
     : | -> alternare
     : * -> stelat
     : + -> plus
     : ? -> misterios
     */
    deque <Automat> q;
    while(!p.empty())
    {
        string c = p.front();
        p.pop_front();
        if((c[0] >= '0' && c[0] <= '9') || (c[0] <= 'z' && c[0] >= 'a')) {
            Automat aux(c[0]);
            q.push_back(aux);
        }
        else
        {
            Automat a2 = q.back(); q.pop_back();
            if (c[0] == '.') {
                Automat a1 = q.back(); q.pop_back();
                a1.concatenare(a2);
                q.push_back(a1);
                //  cout << a1 << endl << "size:" << a1.s().size() << endl;
            }
            else if (c[0] == '|') {
                Automat a1 = q.back(); q.pop_back();
                a1.alternare(a2);
                q.push_back(a1);
                //    cout << a1 << endl << "size:" << a1.s().size() << endl;
            }
            else if (c[0] == '*') {
                a2.stelat();
                q.push_back(a2);
                //              cout << a2 << endl << "size:"
                //                << a2.s().size() << endl;
            }
            else if (c[0] == '+') {
                a2.plus();
                q.push_back(a2);
                //cout << a2 << endl << "size:" << a2.s().size() << endl;
            }
            else if (c[0] == '?') {
                a2.misterios();
                q.push_back(a2);
                //    cout << a2 << endl << "size:" << a2.s().size() << endl;
            }
            else {
                cout << "Nu trebuia ...\n";
            }
        }
    }
    return q.back();
}
Automat::Automat(const Input &citire): S(citire), A(citire) {
    T = Transitions{citire, S, A};
    bool ok = true;
    ok = ok & S.validStates() & A.validSigma() & T.validTransitions();
    if (!ok)
        cout << "Automat invalid\n", this->ok = false;
}

Automat::Automat(const States &state, const Sigma &sigma, const Transitions &trans): S(state), T(trans), A(sigma) {
    bool ok = true;
    ok = ok & state.validStates() & sigma.validSigma() & trans.validTransitions();
    if (!ok)
        cout << "Automat invalid\n", this->ok = false;
}

bool Automat::cuvant(char cuv[]) {
    bool valid = false;
    dfs(valid, cuv, S.nodStart(), 0, strlen(cuv));
    return valid;
}

bool Automat::isValid() {
    return ok;
}

bool Automat::isDFA() const {
    return T.isDFA();
}

bool Automat::isNFA() const {
    return T.isNFA();
}

void Automat::toDFA() {
    toNFA();
    if (T.isDFA() == true)
        return;
    bool auxf[n];
    for (int i = 0; i < n; i++)
        auxf[i] = 0;
    auxf[S.nodStart()] = S.stareFinala(S.nodStart());
    vector<nu> aux[n];
    map<char, set<int> > w[n];
    T.getTransition(w);


    // hash pentru un set de noduri
    map<long long, map<char, set<int> > > hash;
    // daca hash a mai fost vizitat
    map<long long, bool> viz;
    // map<long long, bool> viz1;
    // din hash in nod
    map<long long, int> trad;

    const int b = 5009, mod = 1e9 + 7;
    int ct = 0;
    deque<long long> q;
    q.push_back(S.nodStart());

    for (int i = 0; i < n; i++) {
        if (!w[i].empty())
            hash[i] = w[i], w[i].clear();
    }

    hash[S.nodStart()][1].insert(S.nodStart());
    trad[S.nodStart()] = ++ct;
    /*
    for (map<char, set<int>> :: iterator j = hash[3].begin(); j != hash[3].end(); j ++) {
        cout << j->first << ":   ";
        for (set<int>:: iterator p = j->second.begin(); p != j->second.end(); p ++) {
            cout << *p << " ";
            ok = ok | S.stareFinala(*p);
        }
        cout << "\n";
    }
    */
    while (!q.empty()) {
        int nod = q.front();
        q.pop_front();


        /*
                        cout << "Componeneta nodului " << nod << ":\n";
                        bool ok = false;
                        for (map<char, set<int>> :: iterator j = hash[nod].begin(); j != hash[nod].end(); j ++) {
                            cout << j->first << ":   ";
                            for (set<int>:: iterator p = j->second.begin(); p != j->second.end(); p ++) {
                                cout << *p << " ";
                                ok = ok | S.stareFinala(*p);
                            }
                            cout << "\n";
                            break;
                        }

        */
        viz[nod] = true;
        map<char, set<int> > dest;
        for (set<int>::iterator i = hash[nod][1].begin(); i != hash[nod][1].end(); i++)
            for (map<char, set<int> >::iterator j = hash[*i].begin(); j != hash[*i].end(); j++) {
                if (j->first != 1)
                    dest[j->first].insert(j->second.begin(), j->second.end());
            }
        for (map<char, set<int> >::iterator i = dest.begin(); i != dest.end(); i++) {
            long long node = 0;
            bool ok = 0;
            for (set<int>::iterator j = i->second.begin(); j != i->second.end(); j++) {
                ok = ok | S.stareFinala(*j);
                node *= b, node += *j, node %= mod;
            }
            //   if (i->second.size() > 1) {
            node += n + 1;
            // }
            //  else if (i->second.size() == 0)
            //    continue;
            if (viz[node] == 0) {
                hash[node][1].insert(dest[i->first].begin(), dest[i->first].end());
                hash[nod][i->first] = dest[i->first];
                viz[node] = 1;
                q.push_back(node);
                trad[node] = ++ct;
                aux[trad[nod]].push_back({ct, i->first});
                // auxf[ct] = ok;
            } else {
                aux[trad[nod]].push_back({trad[node], i->first});
                hash[node][i->first] = dest[i->first];
            }
        }
    }
    for (map<long long, int>::iterator i = trad.begin(); i != trad.end(); i++) {
        cout << "Componeneta nodului " << i->second << ":\n";
        bool ok = false;
        for (map<char, set<int> >::iterator j = hash[i->first].begin(); j != hash[i->first].end(); j++) {
            cout << j->first << ":   ";
            for (set<int>::iterator p = j->second.begin(); p != j->second.end(); p++) {
                cout << *p << " ";
                ok = ok | S.stareFinala(*p);
            }
            auxf[i->second] = ok;
            cout << ((ok == 1) ? "final" : "") << "\n";
            break;
            cout << "\n";
        }
        cout << "\n";
    }
    S = {States{1, auxf}};
    T.modificareTrans(aux);
}

ostream &operator<<(ostream &os, const Automat &a) {
    os << "Despre Automat: \n";
    os << a.S << "\n"
    //<< a.A << "\n"
    << a.T << "\n";
    return os;
}
void Automat::returneaza_nfa(int nod, bool ok[], vector<nu> &aux, vector<nu> v[], bool &esteFinal) {
    ok[nod] = true;
    if (S.stareFinala(nod))
        esteFinal = true;
    for (int i = 0; i < v[nod].size(); i ++)
        if (ok[v[nod][i].nod] == false) {
            if (v[nod][i].a != '$')
                ok[v[nod][i].nod] = true, aux.push_back(v[nod][i]);
            else returneaza_nfa(v[nod][i].nod, ok, aux, v, esteFinal);
        }
        else if (v[nod][i].a != '$')
        ok[v[nod][i].nod] = false, aux.push_back(v[nod][i]);
}
void Automat::toNFA() {
    vector<nu> *v, nou[n];
    vector <int> final;
    v = T.get_transitions();
    bool ok[n] = {false};
    for (int i = 0; i < n; i ++) {
        bool esteFinal = false;
        memset(ok, 0, sizeof(ok));
        vector <nu> aux;
        returneaza_nfa(i, ok, aux, v, esteFinal);
        nou[i] = aux;
        if (esteFinal)
            final.push_back(i);
    }
    T.modificareTrans(nou);
    S.changeStariFinale(final);
}


void Automat::increaseN(int n) {
    T.increaseN(n);
    S.increaseN(n);
}

vector<nu> *Automat::get_transitions() {
    return T.get_transitions();
}

vector<int> Automat::stareInitiala() {
    return S.stareInitiala();
}
vector<int> Automat::stariFinale() {
    return S.stariFinale();
}
void Automat::concatenare(Automat &other) {
    other.increaseN(S.size());
    T.addTransitions(other.get_transitions());
    T.addTransitions(S.stariFinale(), other.stareInitiala());
    S.changeStariFinale(other.stariFinale());
    S.nNoduri(other.s().size() - S.size());
}

void Automat::alternare(Automat &other) {
    increaseN(1);
    other.increaseN(S.size());
    T.addTransitions(other.get_transitions());
    T.addTransitions({1}, {S.stareInitiala()[0], other.stareInitiala()[0]});
    S.changeStareInitiala(1);
    vector <int> v = S.stariFinale();
    vector <int> p = other.stariFinale();
    for (int i = 0; i < p.size(); i ++)
        v.push_back(p[i]);
    S.changeStariFinale(v);
    S.nNoduri(other.s().size() - S.size());
}

void Automat::stelat() {
    increaseN(1);
    T.addTransitions(S.stariFinale(), {1});
    T.addTransitions({1}, S.stareInitiala());
    S.changeStariFinale({1});
    S.changeStareInitiala({1});
   // S.nNoduri(1);
}
void Automat::plus() {
    increaseN(1);
    T.addTransitions(S.stariFinale(), {1});
    T.addTransitions({1}, S.stareInitiala());
  //  S.changeStariFinale({1});
    S.changeStareInitiala({1});
  //  S.nNoduri(1);
}
void Automat::misterios() {
    increaseN(1);
    T.addTransitions({1}, S.stareInitiala());
    vector <int> v = S.stariFinale();
    v.push_back(1);
    S.changeStariFinale(v);
    S.changeStareInitiala({1});
}



Automat::~Automat() {
    //cout << "Destructor";
}
