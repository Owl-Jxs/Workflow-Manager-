#ifndef ICOMANDO_H
#define ICOMANDO_H

class IComando {
public:
    virtual ~IComando() {}
    virtual void ejecutar() = 0;
    virtual void deshacer() = 0;
};

#endif