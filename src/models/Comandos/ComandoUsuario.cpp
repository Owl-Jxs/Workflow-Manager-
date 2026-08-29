#include "ComandoUsuario.h"

// ==========================================
// AgregarUsuarioComando
// ==========================================

AgregarUsuarioComando::AgregarUsuarioComando(UsuarioController* controller, Usuario* usuario)
 : controller(controller), usuario(usuario) {}

AgregarUsuarioComando::~AgregarUsuarioComando() { delete usuario; }

void AgregarUsuarioComando::ejecutar() { 
//hacemos una copia para el controller
    Usuario* usuarioCopiado = new Usuario (usuario->getId (), usuario->getNombre (), usuario->getRol ());
    usuarioCopiado->setHashDirecto (usuario->getHashContrasena ());

    controller->agregarUsuario(usuarioCopiado); //le pasamos la copia al controller
}

void AgregarUsuarioComando::deshacer() { controller->eliminarUsuario(usuario->getId());}

std::string AgregarUsuarioComando::getAccionAuditoria() const {return "AGREGAR_USUARIO";}
int AgregarUsuarioComando::getIdTareaAuditoria() const {return -1; }

// ==========================================
// ActualizarUsuarioComando
// ==========================================

ActualizarUsuarioComando::ActualizarUsuarioComando(UsuarioController* controller, int idUsuario, Usuario* usuarioNuevo)
 : controller(controller), idUsuario(idUsuario), usuarioNuevo(usuarioNuevo),  usuarioAnterior(nullptr), ejecutado(false) { }

ActualizarUsuarioComando::~ActualizarUsuarioComando() { delete usuarioAnterior; delete usuarioNuevo; }

void ActualizarUsuarioComando::ejecutar() {
    if (!ejecutado) { //si no se ha ejecutado antes
        Usuario* usuarioActual = controller->buscarUsuarioPorId(idUsuario);
        
        if (usuarioActual != nullptr) { //creamos una copia del antiguo usuario
            if (usuarioAnterior != nullptr) delete usuarioAnterior;
            usuarioAnterior = new Usuario(usuarioActual->getId(), usuarioActual->getNombre(), usuarioActual->getRol() );
            usuarioAnterior->setHashDirecto( usuarioActual->getHashContrasena());
        }
    }
// Crear una copia del nuevo usuario 
    Usuario* copiaNuevo = new Usuario(usuarioNuevo->getId(), usuarioNuevo->getNombre(), usuarioNuevo->getRol());
    copiaNuevo->setHashDirecto(usuarioNuevo->getHashContrasena());
    
    controller->actualizarUsuario(idUsuario,copiaNuevo);
    ejecutado = true;
}

void ActualizarUsuarioComando::deshacer() {
   if (usuarioAnterior == nullptr) return;
//copiamos el anterior
    Usuario* copiaAnterior = new Usuario(usuarioAnterior->getId(), usuarioAnterior->getNombre(),usuarioAnterior->getRol());
    copiaAnterior->setHashDirecto(usuarioAnterior->getHashContrasena());

    controller->actualizarUsuario(idUsuario, copiaAnterior);
    ejecutado = false;
}

std::string ActualizarUsuarioComando::getAccionAuditoria() const { return "ACTUALIZAR_USUARIO"; }
int ActualizarUsuarioComando::getIdTareaAuditoria() const { return -1; }

// ==========================================
// EliminarUsuarioComando
// ==========================================

EliminarUsuarioComando::EliminarUsuarioComando(UsuarioController* controller, int idUsuario)
 : controller(controller), idUsuario(idUsuario), usuarioGuardado(nullptr) { }

EliminarUsuarioComando::~EliminarUsuarioComando(){ delete usuarioGuardado; }

void EliminarUsuarioComando::ejecutar() {
    Usuario* usuarioActual = controller->buscarUsuarioPorId(idUsuario);
    if (usuarioActual != nullptr) {

    // Creamos una copia independiente antes de eliminar el usuario original.
    if (usuarioGuardado != nullptr) delete usuarioGuardado;
        usuarioGuardado = new Usuario(usuarioActual->getId(), usuarioActual->getNombre(), usuarioActual->getRol());
        usuarioGuardado->setHashDirecto(usuarioActual->getHashContrasena());
        
        controller->eliminarUsuario(idUsuario); //lo eliminamos
    }
}

void EliminarUsuarioComando::deshacer() {
    if (usuarioGuardado != nullptr) {
        controller->agregarUsuario(usuarioGuardado);
        usuarioGuardado = nullptr; //pasamos la dependendencia totalmente al controller
    }
}

std::string EliminarUsuarioComando::getAccionAuditoria() const{ return "ELIMINAR_USUARIO"; }
int EliminarUsuarioComando::getIdTareaAuditoria() const { return -1; }