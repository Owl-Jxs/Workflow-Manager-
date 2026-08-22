#ifndef ELIMINARUSUARIOCOMANDO_H
#define ELIMINARUSUARIOCOMANDO_H

#include "IComando.h"
#include "../../Controllers/UsuarioController.h"

class EliminarUsuarioComando : public IComando {
private:
    UsuarioController* controller;
    int idUsuario;
    Usuario* usuarioGuardado;
public:
    EliminarUsuarioComando(UsuarioController* controller, int idUsuario);
    ~EliminarUsuarioComando() override;
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

#endif
