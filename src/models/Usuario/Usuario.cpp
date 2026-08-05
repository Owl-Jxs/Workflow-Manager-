#include "Usuario.h"

Usuario::Usuario()
    : idUsuario(0), nombreUsuario(""), rolUsuario(Rol::USUARIO_NORMAL) {
}

Usuario::Usuario(int id, const std::string& nombre, Rol rol)
    : idUsuario(id), nombreUsuario(nombre), rolUsuario(rol) {
}

Usuario::~Usuario() {
}

void Usuario::setId(int id) {
    this->idUsuario = id;
}

int Usuario::getId() const {
    return this->idUsuario;
}

void Usuario::setNombre(const std::string& nombre) {
    this->nombreUsuario = nombre;
}

std::string Usuario::getNombre() const {
    return this->nombreUsuario;
}

void Usuario::setRol(Rol rol) {
    this->rolUsuario = rol;
}

Usuario::Rol Usuario::getRol() const {
    return this->rolUsuario;
}
