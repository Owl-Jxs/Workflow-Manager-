#ifndef COMANDOUSUARIO_H
#define COMANDOUSUARIO_H

#include "IComando.h"
#include "../../Controllers/UsuarioController.h"

// ==========================================
// AgregarUsuarioComando
// ==========================================
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

// ==========================================
// ActualizarUsuarioComando
// ==========================================
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

// ==========================================
// EliminarUsuarioComando
// ==========================================
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
