//
// Created by Gabriel on 4/16/2025.
//

#include "../include/Input.h"


Input::Input(const string &filename)
{
    ifstream f(filename);
    int ct = 0, i = 0;
    while (getline(f, mat[++i]) && ct - 3)
        ct += (mat[i] == "End");
}
int Input::findState() const
{
    for(int i = 0; true; i ++)
        if(mat[i] == "States:")
            return i;
}
int Input::findTrans() const
{
    for(int i = 0; true; i ++)
        if(mat[i] == "Transitions:")
            return i;
}
int Input::findSigma() const
{
    for(int i = 0; true; i ++)
        if(mat[i] == "Sigma:")
            return i;
}
void Input::Matrice(string copie[]) const
{
    for (int i = 0; i < n; i++)
        copie[i] = mat[i];
}

ostream&  operator<<(ostream& os, const Input& a)
{
    os << "Despre input \n";
    os << "States incepe la linia: " << a.findState() << "\n";
    os << "Sigma incepe la linia: " << a.findSigma() << "\n";
    os << "Transitions incepe la linia: " << a.findTrans() << "\n";
    return os;
}
Input& Input:: operator=(const Input& other) {
    // std::cout << "Operator egal\n";
    for (int i = 0; i < n; i ++)
        this->mat[i] = other.mat[i];
    return *this;
}
Input::Input(const Input& other) {
    //std::cout << "Constructor de Copiere\n";
    for (int i = 0; i < n; i ++)
        this->mat[i] = other.mat[i];
}
Input::~Input()
{
    //cout << "Destructor";
}