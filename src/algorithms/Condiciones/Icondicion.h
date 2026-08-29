#ifndef ICONDICION_H
#define ICONDICION_H

template <typename T>
class Icondicion {
public:
    virtual bool ordenar(T object1, T object2) = 0;
    virtual ~Icondicion() = default;
};

#endif