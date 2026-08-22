#include "AgregarUsuarioComando.h"

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
