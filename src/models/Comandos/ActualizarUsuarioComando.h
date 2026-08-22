#ifndef ACTUALIZARUSUARIOCOMANDO_H
#define ACTUALIZARUSUARIOCOMANDO_H

#include "IComando.h"
#include "../../Controllers/UsuarioController.h"

class ActualizarUsuarioComando : public IComando {
private:
    UsuarioController* controller;
    int idUsuario;
    Usuario* usuarioNuevo;
    Usuario* usuarioAnterior;
    bool ejecutado;
public:
    ActualizarUsuarioComando(UsuarioController* controller, int idUsuario, Usuario* usuarioNuevo);
    ~ActualizarUsuarioComando() override;
    void ejecutar() override;
    void deshacer() override;
    std::string getAccionAuditoria() const override;
    int getIdTareaAuditoria() const override;
};

#endif
