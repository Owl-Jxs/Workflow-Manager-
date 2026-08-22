#ifndef AGREGARUSUARIOCOMANDO_H
#define AGREGARUSUARIOCOMANDO_H

#include "IComando.h"
#include "../../Controllers/UsuarioController.h"

class AgregarUsuarioComando : public IComando {
private:
    UsuarioController* controller;
    Usuario* usuario;
public:
    AgregarUsuarioComando(UsuarioController* controller, Usuario* usuario);
    ~AgregarUsuarioComando() override;
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

#endif
