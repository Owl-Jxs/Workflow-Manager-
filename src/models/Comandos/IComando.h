#ifndef ICOMANDO_H
#define ICOMANDO_H
#include <string>

class IComando {
public:
    virtual ~IComando() {}
    virtual void ejecutar() = 0;
    virtual void deshacer() = 0;

    // Metadatos para la bitacora de auditoria (Aclaratoria 3, parte 2).
    // Por defecto no aportan informacion; los comandos concretos los sobreescriben.
    virtual std::string getAccionAuditoria() const { return ""; }
    virtual int getIdTareaAuditoria() const { return -1; }
};

#endif