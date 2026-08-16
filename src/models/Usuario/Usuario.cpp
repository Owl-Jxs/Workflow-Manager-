#include "Usuario.h"

Usuario::Usuario()
    : idUsuario(0), nombreUsuario(""), rolUsuario(Rol::USUARIO_NORMAL), hashContrasena(0) {
}

Usuario::Usuario(int id, const std::string& nombre, Rol rol)
    : idUsuario(id), nombreUsuario(nombre), rolUsuario(rol), hashContrasena(0) {
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

void Usuario::setHashContrasena(const std::string& contrasena) {
	// Aquí podríamos implementar un algoritmo de hash seguro,
	// Para simplificar, usaremos std::hash, pero no es seguro para contraseñas reales.
	std::hash<std::string> hasher;
	this->hashContrasena = hasher(contrasena);
}

void Usuario::setHashDirecto(size_t hash) {
	this->hashContrasena = hash;
}

size_t Usuario::getHashContrasena() const {
	return this->hashContrasena;
}
