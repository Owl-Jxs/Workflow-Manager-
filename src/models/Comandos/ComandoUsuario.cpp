#include "ComandoUsuario.h"

// ==========================================
// AgregarUsuarioComando
// ==========================================

AgregarUsuarioComando::AgregarUsuarioComando(
    UsuarioController* controller, Usuario* usuario)
    : controller(controller), usuario(usuario) {}

AgregarUsuarioComando::~AgregarUsuarioComando() {
    if (usuario != nullptr) delete usuario;
}

void AgregarUsuarioComando::ejecutar() {
    controller->agregarUsuario(usuario);
}

void AgregarUsuarioComando::deshacer() {
    controller->eliminarUsuario(usuario->getId());
}

std::string AgregarUsuarioComando::getAccionAuditoria() const {
    return "AGREGAR_USUARIO";
}

int AgregarUsuarioComando::getIdTareaAuditoria() const {
    return -1;
}

// ==========================================
// ActualizarUsuarioComando
// ==========================================

ActualizarUsuarioComando::ActualizarUsuarioComando(
    UsuarioController* controller, int idUsuario, Usuario* usuarioNuevo)
    : controller(controller), idUsuario(idUsuario),
      usuarioNuevo(usuarioNuevo), usuarioAnterior(nullptr), ejecutado(false) {}

ActualizarUsuarioComando::~ActualizarUsuarioComando() {
    if (usuarioNuevo != nullptr) delete usuarioNuevo;
    if (usuarioAnterior != nullptr) delete usuarioAnterior;
}

void ActualizarUsuarioComando::ejecutar() {
    if (!ejecutado) {
        usuarioAnterior = controller->buscarUsuarioPorId(idUsuario);
        if (usuarioAnterior != nullptr) {
            usuarioAnterior = new Usuario(
                usuarioAnterior->getId(),
                usuarioAnterior->getNombre(),
                usuarioAnterior->getRol()
            );
            usuarioAnterior->setHashDirecto(usuarioAnterior->getHashContrasena());
        }
    }

    controller->actualizarUsuario(idUsuario, usuarioNuevo);
    ejecutado = true;
}

void ActualizarUsuarioComando::deshacer() {
    if (usuarioAnterior != nullptr) {
        controller->actualizarUsuario(idUsuario, usuarioAnterior);
    }
}

std::string ActualizarUsuarioComando::getAccionAuditoria() const {
    return "ACTUALIZAR_USUARIO";
}

int ActualizarUsuarioComando::getIdTareaAuditoria() const {
    return -1;
}

// ==========================================
// EliminarUsuarioComando
// ==========================================

EliminarUsuarioComando::EliminarUsuarioComando(
    UsuarioController* controller, int idUsuario)
    : controller(controller), idUsuario(idUsuario), usuarioGuardado(nullptr) {}

EliminarUsuarioComando::~EliminarUsuarioComando() {
    if (usuarioGuardado != nullptr) delete usuarioGuardado;
}

void EliminarUsuarioComando::ejecutar() {
    usuarioGuardado = controller->buscarUsuarioPorId(idUsuario);
    if (usuarioGuardado != nullptr) {
        usuarioGuardado = new Usuario(
            usuarioGuardado->getId(),
            usuarioGuardado->getNombre(),
            usuarioGuardado->getRol()
        );
        usuarioGuardado->setHashDirecto(usuarioGuardado->getHashContrasena());
    }
    controller->eliminarUsuario(idUsuario);
}

void EliminarUsuarioComando::deshacer() {
    if (usuarioGuardado != nullptr) {
        controller->agregarUsuario(usuarioGuardado);
        usuarioGuardado = nullptr;
    }
}

std::string EliminarUsuarioComando::getAccionAuditoria() const {
    return "ELIMINAR_USUARIO";
}

int EliminarUsuarioComando::getIdTareaAuditoria() const {
    return -1;
}
