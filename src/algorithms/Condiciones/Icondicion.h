#ifndef ICONDICION_H
#define ICONDICION_H

template <typename t>
class Icondicion {
public:
    virtual bool ordenar (const t& object1, const t& object2) = 0;
    
};


#endif