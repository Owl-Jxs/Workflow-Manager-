#ifndef ICONDICION_H
#define ICONDICION_H

template <typename T>
class ICondicion {
public:
    virtual bool comparar(const T& objeto1, const T& objeto2) const = 0;
    virtual ~ICondicion() {}
};

#endif