//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Automat.h"

#include <cstring>
#include <iostream>

// --- Private Helper Methods ---

void Automat::dfs(bool &isValid, char inputWord[], int currentState, int currentPosition, int wordLength) const {
    if (isValid) {
        return;
    }

    if (currentPosition == wordLength) {
        if (stateMachine.isFinalState(currentState)) {
            isValid = true;
        }

        for (int index = 0; index < transitions.size(currentState); ++index) {
            char symbol = transitions.character(currentState, index);
            int nextState = transitions.node(currentState, index);
            if (!isValid && symbol == '$') {
                dfs(isValid, inputWord, nextState, currentPosition, wordLength);
            }
        }
        return;
    }

    for (int index = 0; index < transitions.size(currentState); ++index) {
        char symbol = transitions.character(currentState, index);
        int nextState = transitions.node(currentState, index);
        if (!isValid && symbol == inputWord[currentPosition]) {
            dfs(isValid, inputWord, nextState, currentPosition + 1, wordLength);
        } else if (!isValid && symbol == '$') {
            dfs(isValid, inputWord, nextState, currentPosition, wordLength);
        }
    }
}

void Automat::returnNfa(int node, bool visited[], std::vector<TransitionNode> &aux,
                        std::vector<TransitionNode> transitions[], bool &isFinal) {
    visited[node] = true;
    if (stateMachine.isFinalState(node)) {
        isFinal = true;
    }

    for (int index = 0; index < transitions[node].size(); ++index) {
        if (visited[transitions[node][index].node] == false) {
            if (transitions[node][index].symbol != '$') {
                visited[transitions[node][index].node] = true;
                aux.push_back(transitions[node][index]);
            } else {
                returnNfa(transitions[node][index].node, visited, aux, transitions, isFinal);
            }
        } else if (transitions[node][index].symbol != '$') {
            visited[transitions[node][index].node] = false;
            aux.push_back(transitions[node][index]);
        }
    }
}

// --- Constructors, Destructor, and Assignment ---

Automat::Automat(char value) : stateMachine(value), transitions(value), alphabet(value) {}

Automat::Automat(const Input &input) : stateMachine(input), alphabet(input) {
    transitions = Transitions{input, stateMachine, alphabet};
    bool isValidFlag = true;
    isValidFlag = isValidFlag & stateMachine.validStates() & alphabet.validSigma() & transitions.validTransitions();
    if (!isValidFlag) {
        std::cout << "Automaton is invalid\n";
        this->ok = false;
    }
}

Automat::Automat(std::string &regex) {
    parseRegex(regex);
    std::deque<std::string> tokens = postfixNotation(regex);
    *this = toAutomat(tokens);
}

Automat::Automat(const States &state, const Sigma &sigma, const Transitions &trans)
    : stateMachine(state), transitions(trans), alphabet(sigma) {
    bool isValidFlag = true;
    isValidFlag = isValidFlag & state.validStates() & sigma.validSigma() & trans.validTransitions();
    if (!isValidFlag) {
        std::cout << "Automaton is invalid\n";
        this->ok = false;
    }
}

Automat::Automat(const Automat &other) {
    this->stateMachine = other.stateMachine;
    this->transitions = other.transitions;
    this->alphabet = other.alphabet;
    this->ok = other.ok;
}

Automat &Automat::operator=(const Automat &other) {
    if (this != &other) { // Added self-assignment protection
        this->stateMachine = other.stateMachine;
        this->transitions = other.transitions;
        this->alphabet = other.alphabet;
        this->ok = other.ok;
    }
    return *this;
}

Automat::~Automat() = default;

// --- Static Factory Methods ---

Automat Automat::toAutomat(std::deque<std::string> tokens) {
    std::deque<Automat> automatonStack;
    while (!tokens.empty()) {
        std::string currentToken = tokens.front();
        tokens.pop_front();

        if ((currentToken[0] >= '0' && currentToken[0] <= '9') ||
            (currentToken[0] <= 'z' && currentToken[0] >= 'a')) {
            Automat automaton(currentToken[0]);
            automatonStack.push_back(automaton);
        } else {
            Automat secondAutomaton = automatonStack.back();
            automatonStack.pop_back();

            if (currentToken[0] == '.') {
                Automat firstAutomaton = automatonStack.back();
                automatonStack.pop_back();
                firstAutomaton.concatenate(secondAutomaton);
                automatonStack.push_back(firstAutomaton);
            } else if (currentToken[0] == '|') {
                Automat firstAutomaton = automatonStack.back();
                automatonStack.pop_back();
                firstAutomaton.alternate(secondAutomaton);
                automatonStack.push_back(firstAutomaton);
            } else if (currentToken[0] == '*') {
                secondAutomaton.star();
                automatonStack.push_back(secondAutomaton);
            } else if (currentToken[0] == '+') {
                secondAutomaton.plus();
                automatonStack.push_back(secondAutomaton);
            } else if (currentToken[0] == '?') {
                secondAutomaton.optional();
                automatonStack.push_back(secondAutomaton);
            } else {
                std::cout << "Unexpected operator.\n";
            }
        }
    }
    return automatonStack.back();
}

// --- Core API / Operations ---

bool Automat::belongsToAutomaton(const std::string &word) const {
    char buffer[2000];
    std::strcpy(buffer, word.c_str());
    return acceptsWord(buffer);
}

bool Automat::acceptsWord(char word[]) const {
    bool isValidFlag = false;
    dfs(isValidFlag, word, stateMachine.startNode(), 0, std::strlen(word));
    return isValidFlag;
}

void Automat::toDFA() {
    toNFA();
    if (transitions.isDFA() == true) {
        return;
    }

    bool finalStateFlags[kNodeLimit];
    for (bool & finalStateFlag : finalStateFlags) {
        finalStateFlag = false;
    }
    finalStateFlags[stateMachine.startNode()] = stateMachine.isFinalState(stateMachine.startNode());

    std::vector<TransitionNode> componentTransitions[kNodeLimit];
    std::map<char, std::set<int>> transitionTable[kNodeLimit];
    transitions.getTransition(transitionTable);

    std::map<long long, std::map<char, std::set<int>>> componentMap;
    std::map<long long, bool> visitedComponents;
    std::map<long long, int> componentIds;

    const int base = 5009;
    const int mod = 1e9 + 7;
    int count = 0;
    std::deque<long long> stateQueue;
    stateQueue.push_back(stateMachine.startNode());

    for (int index = 0; index < kNodeLimit; ++index) {
        if (!transitionTable[index].empty()) {
            componentMap[index] = transitionTable[index];
            transitionTable[index].clear();
        }
    }

    componentMap[stateMachine.startNode()][1].insert(stateMachine.startNode());
    componentIds[stateMachine.startNode()] = ++count;

    while (!stateQueue.empty()) {
        int currentNode = stateQueue.front();
        stateQueue.pop_front();
        visitedComponents[currentNode] = true;

        std::map<char, std::set<int>> destination;
        for (auto it = componentMap[currentNode][1].begin(); it != componentMap[currentNode][1].end(); ++it) {
            for (auto & transitionIt : componentMap[*it]) {
                if (transitionIt.first != 1) {
                    destination[transitionIt.first].insert(transitionIt.second.begin(), transitionIt.second.end());
                }
            }
        }

        for (auto it = destination.begin(); it != destination.end(); ++it) {
            long long state = 0;
            bool isFinal = false;
            for (int transitionIt : it->second) {
                isFinal = isFinal | stateMachine.isFinalState(transitionIt);
                state *= base;
                state += transitionIt;
                state %= mod;
            }
            state += kNodeLimit + 1;

            if (visitedComponents[state] == 0) {
                componentMap[state][1].insert(destination[it->first].begin(), destination[it->first].end());
                componentMap[currentNode][it->first] = destination[it->first];
                visitedComponents[state] = true;
                stateQueue.push_back(state);
                componentIds[state] = ++count;
                componentTransitions[componentIds[currentNode]].push_back({count, it->first});
            } else {
                componentTransitions[componentIds[currentNode]].push_back({componentIds[state], it->first});
                componentMap[state][it->first] = destination[it->first];
            }
        }
    }

    for (auto & componentId : componentIds) {
        std::cout << "Component of node " << componentId.second << ":\n";
        bool isFinal = false;
        for (auto transitionIt = componentMap[componentId.first].begin(); transitionIt != componentMap[componentId.first].end(); ++transitionIt) {
            std::cout << transitionIt->first << ":   ";
            for (const int stateIt : transitionIt->second) {
                std::cout << stateIt << " ";
                isFinal = isFinal | stateMachine.isFinalState(stateIt);
            }
            finalStateFlags[componentId.second] = isFinal;
            std::cout << ((isFinal == 1) ? "final" : "") << "\n";
            break;
        }
        std::cout << "\n";
    }

    stateMachine = {States{1, finalStateFlags}};
    transitions.modifyTransitions(componentTransitions);
}

void Automat::toNFA() {
    std::vector<TransitionNode> normalizedTransitions[kNodeLimit];
    std::vector<int> finalStateList;
    std::vector<TransitionNode> *transitionList = transitions.getTransitions();
    bool visited[kNodeLimit] = {false};

    for (int index = 0; index < kNodeLimit; ++index) {
        bool isFinal = false;
        std::memset(visited, 0, sizeof(visited));
        std::vector<TransitionNode> aux;
        returnNfa(index, visited, aux, transitionList, isFinal);
        normalizedTransitions[index] = aux;
        if (isFinal) {
            finalStateList.push_back(index);
        }
    }

    transitions.modifyTransitions(normalizedTransitions);
    stateMachine.changeFinalStates(finalStateList);
}

// --- Regex Builder Operations ---

void Automat::concatenate(Automat &other) {
    other.increaseN(stateMachine.size());
    transitions.addTransitions(other.getTransitions());
    transitions.addTransitions(stateMachine.finalStates(), other.initialStates());
    stateMachine.changeFinalStates(other.finalStates());
    stateMachine.updateNodeCount(other.states().size() - stateMachine.size());
}

void Automat::alternate(Automat &other) {
    increaseN(1);
    other.increaseN(stateMachine.size());
    transitions.addTransitions(other.getTransitions());
    transitions.addTransitions({1}, {stateMachine.initialStates()[0], other.initialStates()[0]});
    stateMachine.changeInitialState(1);

    std::vector<int> statesList = stateMachine.finalStates();
    std::vector<int> otherStates = other.finalStates();
    for (int otherState : otherStates) {
        statesList.push_back(otherState);
    }
    stateMachine.changeFinalStates(statesList);
    stateMachine.updateNodeCount(other.states().size() - stateMachine.size());
}

void Automat::star() {
    increaseN(1);
    transitions.addTransitions(stateMachine.finalStates(), {1});
    transitions.addTransitions({1}, stateMachine.initialStates());
    stateMachine.changeFinalStates({1});
    stateMachine.changeInitialState(1);
}

void Automat::plus() {
    increaseN(1);
    transitions.addTransitions(stateMachine.finalStates(), {1});
    transitions.addTransitions({1}, stateMachine.initialStates());
    stateMachine.changeInitialState(1);
}

void Automat::optional() {
    increaseN(1);
    transitions.addTransitions({1}, stateMachine.initialStates());
    std::vector<int> statesList = stateMachine.finalStates();
    statesList.push_back(1);
    stateMachine.changeFinalStates(statesList);
    stateMachine.changeInitialState(1);
}

// --- Getters / State Queries ---

bool Automat::isValid() const { return ok; }
bool Automat::isDFA() const { return transitions.isDFA(); }
bool Automat::isNFA() const { return transitions.isNFA(); }

std::vector<TransitionNode> *Automat::getTransitions() {
    return transitions.getTransitions();
}

std::vector<int> Automat::initialStates() const {
    return stateMachine.initialStates();
}

std::vector<int> Automat::finalStates() const {
    return stateMachine.finalStates();
}

// --- Modifiers ---

void Automat::increaseN(int count) {
    transitions.increaseN(count);
    stateMachine.increaseN(count);
}

// --- Static Helper Methods ---

void Automat::parseRegex(std::string &regex) {
    for (int index = 0; regex[index]; ++index) {
        if (index != regex.length() - 1) {
            if (!strchr("(|.", regex[index]) && !strchr("|*.+?)", regex[index + 1])) {
                regex.insert(regex.begin() + index + 1, '.');
            }
        }
    }
}

std::deque<std::string> Automat::postfixNotation(const std::string &expression) {
    std::map<std::string, int> precedence;
    precedence["+"] = precedence["*"] = precedence["?"] = 3;
    precedence["."] = 2;
    precedence["|"] = 1;
    precedence["("] = -1;

    long long number = 0;
    std::deque<std::string> outputTokens;
    std::deque<std::string> operatorStack;

    for (int index = 0; expression[index]; ++index) {
        if (expression[index] <= '9' && expression[index] >= '0') {
            while (expression[index] <= '9' && expression[index] >= '0') {
                number = number * 10 + expression[index] - '0';
                ++index;
            }
            --index;
            outputTokens.push_back(std::to_string(number));
            number = 0;
        } else if (expression[index] <= 'z' && expression[index] >= 'a') {
            outputTokens.push_back(std::string(1, expression[index]));
        } else {
            std::string symbol;
            symbol += expression[index];
            if (expression[index] == '(') {
                operatorStack.push_back(symbol);
            } else if (expression[index] == ')') {
                while (operatorStack.back() != "(") {
                    outputTokens.push_back(operatorStack.back());
                    operatorStack.pop_back();
                }
                operatorStack.pop_back();
            } else {
                while (!operatorStack.empty() && precedence[symbol] <= precedence[operatorStack.back()]) {
                    outputTokens.push_back(operatorStack.back());
                    operatorStack.pop_back();
                }
                operatorStack.push_back(symbol);
            }
        }
    }

    while (!operatorStack.empty()) {
        outputTokens.push_back(operatorStack.back());
        operatorStack.pop_back();
    }
    return outputTokens;
}

// --- Operator Overloads ---

std::ostream &operator<<(std::ostream &output, const Automat &automaton) {
    output << "About the automaton: \n";
    output << automaton.stateMachine << "\n" << automaton.transitions << "\n";
    return output;
}