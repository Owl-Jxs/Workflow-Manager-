#ifndef IDCONDICION_H
#define IDCONDICION_
#include "Icondicion.h"

//orden ascendente 
class CondicionId_Ascendente : public Icondicion<int> {
public:
    bool ordenar (const int& a, const int& b) override {
        return a <= b;
    }
};


// orden descendente
class CondicionId_Descendente : public Icondicion<int> {
public:
    bool ordenar (const int& a, const int& b) override {
        return a >= b;
    }
};

#endif