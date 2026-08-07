//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Automat.h"

void Automat::dfs(bool &valid, char word[], int state, int position, int length) const {
    if (valid)
        return;
    if (position == length) {
        if (S.isFinalState(state))
            valid = true;
        for (int i = 0; i < T.size(state); i++) {
            char ch = T.character(state, i);
            int node = T.node(state, i);
            if (!valid && ch == '$')
                dfs(valid, word, node, position, length);
        }
        return;
    }
    for (int i = 0; i < T.size(state); i++) {
        char ch = T.character(state, i);
        int node = T.node(state, i);
        if (!valid && (ch == word[position]))
            dfs(valid, word, node, position + 1, length);
        else if (!valid && ch == '$')
            dfs(valid, word, node, position, length);
    }
}

Automat::Automat(char value): A(value), T(value), S(value) {}

Automat &Automat::operator=(const Automat &other) {
    this->S = other.S;
    this->T = other.T;
    this->A = other.A;
    this->ok = other.ok;
    return *this;
}

Automat::Automat(const Automat &other) {
    this->S = other.S;
    this->T = other.T;
    this->A = other.A;
    this->ok = other.ok;
}

void Automat::parseRegex(string &regex) {
    for (int i = 0; regex[i]; i++) {
        if (i != regex.length() - 1) {
            if (!strchr("(|.", regex[i]) && !strchr("|*.+?)", regex[i + 1])) {
                regex.insert(regex.begin() + i + 1, '.');
            }
        }
    }
}

deque<string> Automat::postfixNotation(string &s) {
    map<string, int> prec;
    prec["+"] = prec["*"] = prec["?"] = 3;
    prec["."] = 2;
    prec["|"] = 1;
    prec["("] = -1;
    long long number = 0;
    deque<string> result, queue;
    for (int i = 0; s[i]; i++) {
        if (s[i] <= '9' && s[i] >= '0') {
            while (s[i] <= '9' && s[i] >= '0') {
                number = number * 10 + s[i] - '0';
                i++;
            }
            i--;
            result.push_back(to_string(number));
            number = 0;
        }
        else if (s[i] <= 'z' && s[i] >= 'a') {
            result.push_back(string(1, s[i]));
        }
        else {
            string c;
            c += s[i];
            if (s[i] == '(')
                queue.push_back(c);
            else if (s[i] == ')') {
                while (queue.back() != "(")
                    result.push_back(queue.back()), queue.pop_back();
                queue.pop_back();
            }
            else {
                while (!queue.empty() && prec[c] <= prec[queue.back()])
                    result.push_back(queue.back()), queue.pop_back();
                queue.push_back(c);
            }
        }
    }
    while (!queue.empty())
        result.push_back(queue.back()), queue.pop_back();
    return result;
}

Automat::Automat(string &regex) {
    parseRegex(regex);
    deque<string> q = postfixNotation(regex);
    *this = toAutomat(q);
}

bool Automat::belongsToAutomaton(string &word) {
    char c[2000];
    strcpy(c, word.c_str());
    return acceptsWord(c);
}

Automat Automat::toAutomat(deque<string> p) {
    deque<Automat> queue;
    while (!p.empty()) {
        string c = p.front();
        p.pop_front();
        if ((c[0] >= '0' && c[0] <= '9') || (c[0] <= 'z' && c[0] >= 'a')) {
            Automat aux(c[0]);
            queue.push_back(aux);
        }
        else {
            Automat second = queue.back();
            queue.pop_back();
            if (c[0] == '.') {
                Automat first = queue.back();
                queue.pop_back();
                first.concatenate(second);
                queue.push_back(first);
            }
            else if (c[0] == '|') {
                Automat first = queue.back();
                queue.pop_back();
                first.alternate(second);
                queue.push_back(first);
            }
            else if (c[0] == '*') {
                second.star();
                queue.push_back(second);
            }
            else if (c[0] == '+') {
                second.plus();
                queue.push_back(second);
            }
            else if (c[0] == '?') {
                second.optional();
                queue.push_back(second);
            }
            else {
                cout << "Unexpected operator.\n";
            }
        }
    }
    return queue.back();
}

Automat::Automat(const Input &input): S(input), A(input) {
    T = Transitions{input, S, A};
    bool isValid = true;
    isValid = isValid & S.validStates() & A.validSigma() & T.validTransitions();
    if (!isValid)
        cout << "Automaton is invalid\n", this->ok = false;
}

Automat::Automat(const States &state, const Sigma &sigma, const Transitions &trans): S(state), T(trans), A(sigma) {
    bool isValid = true;
    isValid = isValid & state.validStates() & sigma.validSigma() & trans.validTransitions();
    if (!isValid)
        cout << "Automaton is invalid\n", this->ok = false;
}

bool Automat::acceptsWord(char word[]) {
    bool valid = false;
    dfs(valid, word, S.startNode(), 0, strlen(word));
    return valid;
}

bool Automat::isValid() { return ok; }
bool Automat::isDFA() const { return T.isDFA(); }
bool Automat::isNFA() const { return T.isNFA(); }

void Automat::toDFA() {
    toNFA();
    if (T.isDFA() == true)
        return;
    bool auxf[n];
    for (int i = 0; i < n; i++)
        auxf[i] = 0;
    auxf[S.startNode()] = S.isFinalState(S.startNode());
    vector<TransitionNode> aux[n];
    map<char, set<int> > w[n];
    T.getTransition(w);

    map<long long, map<char, set<int> > > hash;
    map<long long, bool> visited;
    map<long long, int> translation;

    const int base = 5009, mod = 1e9 + 7;
    int count = 0;
    deque<long long> q;
    q.push_back(S.startNode());

    for (int i = 0; i < n; i++) {
        if (!w[i].empty())
            hash[i] = w[i], w[i].clear();
    }

    hash[S.startNode()][1].insert(S.startNode());
    translation[S.startNode()] = ++count;

    while (!q.empty()) {
        int node = q.front();
        q.pop_front();
        visited[node] = true;
        map<char, set<int> > destination;
        for (set<int>::iterator i = hash[node][1].begin(); i != hash[node][1].end(); i++)
            for (map<char, set<int> >::iterator j = hash[*i].begin(); j != hash[*i].end(); j++) {
                if (j->first != 1)
                    destination[j->first].insert(j->second.begin(), j->second.end());
            }
        for (map<char, set<int> >::iterator i = destination.begin(); i != destination.end(); i++) {
            long long state = 0;
            bool isFinal = false;
            for (set<int>::iterator j = i->second.begin(); j != i->second.end(); j++) {
                isFinal = isFinal | S.isFinalState(*j);
                state *= base, state += *j, state %= mod;
            }
            state += n + 1;
            if (visited[state] == 0) {
                hash[state][1].insert(destination[i->first].begin(), destination[i->first].end());
                hash[node][i->first] = destination[i->first];
                visited[state] = 1;
                q.push_back(state);
                translation[state] = ++count;
                aux[translation[node]].push_back({count, i->first});
            } else {
                aux[translation[node]].push_back({translation[state], i->first});
                hash[state][i->first] = destination[i->first];
            }
        }
    }
    for (map<long long, int>::iterator i = translation.begin(); i != translation.end(); i++) {
        cout << "Component of node " << i->second << ":\n";
        bool isFinal = false;
        for (map<char, set<int> >::iterator j = hash[i->first].begin(); j != hash[i->first].end(); j++) {
            cout << j->first << ":   ";
            for (set<int>::iterator p = j->second.begin(); p != j->second.end(); p++) {
                cout << *p << " ";
                isFinal = isFinal | S.isFinalState(*p);
            }
            auxf[i->second] = isFinal;
            cout << ((isFinal == 1) ? "final" : "") << "\n";
            break;
        }
        cout << "\n";
    }
    S = {States{1, auxf}};
    T.modifyTransitions(aux);
}

ostream &operator<<(ostream &os, const Automat &a) {
    os << "About the automaton: \n";
    os << a.S << "\n" << a.T << "\n";
    return os;
}

void Automat::returnNfa(int node, bool visited[], vector<TransitionNode> &aux, vector<TransitionNode> transitions[], bool &isFinal) {
    visited[node] = true;
    if (S.isFinalState(node))
        isFinal = true;
    for (int i = 0; i < transitions[node].size(); i++)
        if (visited[transitions[node][i].node] == false) {
            if (transitions[node][i].symbol != '$') {
                visited[transitions[node][i].node] = true;
                aux.push_back(transitions[node][i]);
            }
            else {
                returnNfa(transitions[node][i].node, visited, aux, transitions, isFinal);
            }
        }
        else if (transitions[node][i].symbol != '$') {
            visited[transitions[node][i].node] = false;
            aux.push_back(transitions[node][i]);
        }
}

void Automat::toNFA() {
    vector<TransitionNode> *transitions;
    vector<TransitionNode> newTransitions[n];
    vector<int> finalStates;
    transitions = T.getTransitions();
    bool visited[n] = {false};
    for (int i = 0; i < n; i++) {
        bool isFinal = false;
        memset(visited, 0, sizeof(visited));
        vector<TransitionNode> aux;
        returnNfa(i, visited, aux, transitions, isFinal);
        newTransitions[i] = aux;
        if (isFinal)
            finalStates.push_back(i);
    }
    T.modifyTransitions(newTransitions);
    S.changeFinalStates(finalStates);
}

void Automat::increaseN(int n) { T.increaseN(n); S.increaseN(n); }
vector<TransitionNode> *Automat::getTransitions() { return T.getTransitions(); }
vector<int> Automat::initialStates() { return S.initialStates(); }
vector<int> Automat::finalStates() { return S.finalStates(); }

void Automat::concatenate(Automat &other) {
    other.increaseN(S.size());
    T.addTransitions(other.getTransitions());
    T.addTransitions(S.finalStates(), other.initialStates());
    S.changeFinalStates(other.finalStates());
    S.updateNodeCount(other.s().size() - S.size());
}

void Automat::alternate(Automat &other) {
    increaseN(1);
    other.increaseN(S.size());
    T.addTransitions(other.getTransitions());
    T.addTransitions({1}, {S.initialStates()[0], other.initialStates()[0]});
    S.changeInitialState(1);
    vector<int> v = S.finalStates();
    vector<int> p = other.finalStates();
    for (int i = 0; i < p.size(); i++)
        v.push_back(p[i]);
    S.changeFinalStates(v);
    S.updateNodeCount(other.s().size() - S.size());
}

void Automat::star() {
    increaseN(1);
    T.addTransitions(S.finalStates(), {1});
    T.addTransitions({1}, S.initialStates());
    S.changeFinalStates({1});
    S.changeInitialState(1);
}

void Automat::plus() {
    increaseN(1);
    T.addTransitions(S.finalStates(), {1});
    T.addTransitions({1}, S.initialStates());
    S.changeInitialState(1);
}

void Automat::optional() {
    increaseN(1);
    T.addTransitions({1}, S.initialStates());
    vector<int> v = S.finalStates();
    v.push_back(1);
    S.changeFinalStates(v);
    S.changeInitialState(1);
}

Automat::~Automat() {}
