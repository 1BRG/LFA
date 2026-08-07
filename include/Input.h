//
// Created by Gabriel on 4/16/2025.
//

#ifndef INPUT_H
#define INPUT_H

using namespace std;
#include <string>

#include <fstream>

class Input
{
    static const int n = 2000, m = 50;
    string mat[n];
    ///bool ok = true;
public:
    Input() = default;

    explicit Input(const string &filename);
    int findState() const;
    int findTrans() const;
    int findSigma() const;
    void Matrice(string copie[]) const;
    friend ostream& operator<<(ostream& os, const Input& a);
    Input& operator=(const Input& other);
    Input(const Input& other);
    ~Input();
};



#endif //INPUT_H
