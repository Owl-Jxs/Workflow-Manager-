#ifndef GESTORHISTORIAL_H
#define GESTORHISTORIAL_H
#include "../structures/PilaComando/PilaIComando.h"

class GestorHistorial {
private:
    PilaIComando pilaDeshacer;
    PilaIComando pilaRehacer;
public:
    GestorHistorial();
    ~GestorHistorial();
    void ejecutarComando(IComando* comando);
    void deshacer();
    void rehacer();
};

#endif