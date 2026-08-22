#ifndef GESTORHISTORIAL_H
#define GESTORHISTORIAL_H
#include "../PilaComando/PilaIComando.h"
#include "../../persistence/AuditoriaDataBase/AuditoriaDataBase.h"

class GestorHistorial {
private:
    PilaIComando pilaDeshacer;
    PilaIComando pilaRehacer;
    AuditoriaDataBase* auditoria;   // bitacora transaccional (puede ser nullptr)
    int idUsuarioSesion;            // usuario con la sesion iniciada
public:
    GestorHistorial(AuditoriaDataBase* auditoria = nullptr, int idUsuarioSesion = -1);
    ~GestorHistorial();
    void setAuditoria(AuditoriaDataBase* auditoria);
    void setIdUsuarioSesion(int idUsuarioSesion);
    void ejecutarComando(IComando* comando);
    void deshacer();
    void rehacer();
};

#endif