#include "ActualizarUsuarioComando.h"

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
