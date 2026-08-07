//
// Created by Gabriel on 4/16/2025.
//

#ifndef AUTOMAT_H
#define AUTOMAT_H
#include "Transitions.h"

#include <deque>
#include <string>

class Automat {
private:
    static const int kNodeLimit = 2000;
    static const int kStringLength = 50;

    States stateMachine;
    Transitions transitions;
    Sigma alphabet;
    bool ok = true;

    // --- Private Helper Methods ---
    void dfs(bool &isValid, char word[], int state, int position, int length) const;
    void returnNfa(int node, bool visited[], std::vector<TransitionNode> &aux,
                   std::vector<TransitionNode> transitions[], bool &isFinal);

public:
    // --- Constructors, Destructor, and Assignment ---
    Automat() = default;
    explicit Automat(char value);
    explicit Automat(const Input &input);
    explicit Automat(std::string &regex);
    Automat(const States &state, const Sigma &sigma, const Transitions &trans);

    Automat(const Automat &other);
    Automat &operator=(const Automat &other);
    ~Automat();

    // --- Static Factory Methods ---
    static Automat toAutomat(std::deque<std::string> tokens);

    // --- Core API / Operations ---
    bool belongsToAutomaton(const std::string &word) const;
    bool acceptsWord(char word[]) const;
    void toDFA();
    void toNFA();

    // --- Regex Builder Operations ---
    void concatenate(Automat &other);
    void alternate(Automat &other);
    void star();
    void plus();
    void optional();

    // --- Getters / State Queries ---
    bool isValid() const;
    bool isDFA() const;
    bool isNFA() const;
    std::vector<int> initialStates() const;
    std::vector<int> finalStates() const;
    std::vector<TransitionNode> *getTransitions();

    States states() const { return stateMachine; }
    Transitions transitionStructure() const { return transitions; }
    Sigma alphabetSymbols() const { return alphabet; }

    // --- Modifiers ---
    void increaseN(int count);

    // --- Static Helper Methods ---
    static void parseRegex(std::string &regex);
    static std::deque<std::string> postfixNotation(const std::string &expression);

    // --- Operator Overloads ---
    friend std::ostream &operator<<(std::ostream &output, const Automat &automaton);
};

#endif //AUTOMAT_H