#include "EliminarUsuarioComando.h"

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
