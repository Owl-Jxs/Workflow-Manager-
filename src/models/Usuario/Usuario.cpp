#include "Usuario.h"

/// Constructores y destructores
Usuario::Usuario()
    : idUsuario(0), nombreUsuario(""), rolUsuario(ROL_USER) {
}

Usuario::Usuario(int id, const std::string& nombre, const std::string& rol)
    : idUsuario(id), nombreUsuario(nombre), rolUsuario(rol) {
}

Usuario::~Usuario() {
    // Sin punteros que liberar
}

/// Getters y setters
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

void Usuario::setRol(const std::string& rol) {
    this->rolUsuario = rol;
}

std::string Usuario::getRol() const {
    return this->rolUsuario;
}
