//
// Created by Gabriel on 4/16/2025.
//

#ifndef INPUT_H
#define INPUT_H

#include <fstream>
#include <ostream>
#include <string>

class Input {
    static const int kLineCount = 2000;
    static const int kLineLength = 50;
    std::string lines[kLineCount];
public:
    Input() = default;

    explicit Input(const std::string &filename);
    int findState() const;
    int findTrans() const;
    int findSigma() const;
    void matrix(std::string copy[]) const;
    friend std::ostream &operator<<(std::ostream &output, const Input &input);
    Input &operator=(const Input &other);
    Input(const Input &other);
    ~Input();
};



#endif //INPUT_H
