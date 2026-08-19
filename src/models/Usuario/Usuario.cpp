#include "Usuario.h"
#include <stdexcept>

// Funciones privadas para validar
int Usuario::validarId(int id) {
	if (id < 0) {
		throw std::invalid_argument("ID inválido: debe ser un número entero no negativo.");
	}
	return id;
}

const std::string& Usuario::validarNombre(const std::string& nombre) {
	if (nombre.empty()) {
		throw std::invalid_argument("Nombre inválido: no puede estar vacío.");
	}
	return nombre;
}

// Constructores
Usuario::Usuario()
    : idUsuario(0), nombreUsuario(""), rolUsuario(Rol::USUARIO_NORMAL), hashContrasena(0) {
}

Usuario::Usuario(int id, const std::string& nombre, Rol rol)
    : idUsuario(validarId(id)), nombreUsuario(validarNombre(nombre)), rolUsuario(rol), hashContrasena(0) {
}

Usuario::~Usuario() {
}

void Usuario::setId(int id) {
    this->idUsuario = validarId(id);
}

int Usuario::getId() const {
    return this->idUsuario;
}

void Usuario::setNombre(const std::string& nombre) {
    this->nombreUsuario = validarNombre(nombre);
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
	if (contrasena.empty()) {
		throw std::invalid_argument("Contraseña inválida: no puede estar vacía.");
	}
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
