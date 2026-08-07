//
// Created by Gabriel on 4/16/2025.
//

#ifndef AUTOMAT_H
#define AUTOMAT_H
#include "Transitions.h"
#include <deque>


class Automat
{

    static const int n = 2000, m = 50;
    States S;
    Transitions T;
    Sigma A;

    bool ok = true;
    void dfs(bool &valid, char word[], int state, int position, int length) const;
    void returnNfa(int node, bool visited[], vector<TransitionNode> &aux, vector<TransitionNode> transitions[], bool &isFinal);
public:
    Automat& operator=(const Automat& other);
    Automat(const Automat& other);

    static Automat toAutomat(deque<string> p);

    Automat() = default;
    explicit Automat(const Input &input);
    Automat(const States &state, const Sigma &sigma, const Transitions &trans);

    explicit Automat(char value);

    static void parseRegex(string &regex);

    static deque<string> postfixNotation(string &s);

    explicit Automat(string &regex);

    bool belongsToAutomaton(string &word);

    bool acceptsWord(char word[]);
    bool isValid();
    bool isDFA() const;
    bool isNFA()const;
    void toDFA();
    friend ostream& operator<<(ostream& os, const Automat &a);
    void concatenate(Automat &other);
    void alternate(Automat &other);
    void star();
    void plus();
    void optional();
    void increaseN(int n);
    void toNFA();
    vector<TransitionNode> *getTransitions();
    vector<int> initialStates();
    vector<int> finalStates();
    States s() const {
        return S;
    }

    Transitions t() const {
        return T;
    }

    Sigma a() const {
        return A;
    }

    ~Automat();
};



#endif //AUTOMAT_H
