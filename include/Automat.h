//
// Created by Gabriel on 4/16/2025.
//

#ifndef AUTOMAT_H
#define AUTOMAT_H
#include "Transitions.h"

#include <deque>
#include <string>

class Automat {
    static const int kNodeLimit = 2000;
    static const int kStringLength = 50;
    States stateMachine;
    Transitions transitions;
    Sigma alphabet;

    bool ok = true;
    void dfs(bool &isValid, char word[], int state, int position, int length) const;
    void returnNfa(int node, bool visited[], std::vector<TransitionNode> &aux,
                   std::vector<TransitionNode> transitions[], bool &isFinal);
public:
    Automat &operator=(const Automat &other);
    Automat(const Automat &other);

    static Automat toAutomat(std::deque<std::string> tokens);

    Automat() = default;
    explicit Automat(const Input &input);
    Automat(const States &state, const Sigma &sigma, const Transitions &trans);

    explicit Automat(char value);

    static void parseRegex(std::string &regex);

    static std::deque<std::string> postfixNotation(const std::string &expression);

    explicit Automat(std::string &regex);

    bool belongsToAutomaton(const std::string &word) const;

    bool acceptsWord(char word[]) const;
    bool isValid() const;
    bool isDFA() const;
    bool isNFA() const;
    void toDFA();
    friend std::ostream &operator<<(std::ostream &output, const Automat &automaton);
    void concatenate(Automat &other);
    void alternate(Automat &other);
    void star();
    void plus();
    void optional();
    void increaseN(int count);
    void toNFA();
    std::vector<TransitionNode> *getTransitions();
    std::vector<int> initialStates();
    std::vector<int> finalStates();

    States states() const {
        return stateMachine;
    }

    Transitions transitionStructure() const {
        return transitions;
    }

    Sigma alphabetSymbols() const {
        return alphabet;
    }

    ~Automat();
};



#endif //AUTOMAT_H
