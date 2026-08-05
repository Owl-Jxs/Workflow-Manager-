#include "Usuario.h"
///constructors and destructors
Usuario::Usuario() {
    this->idUsuario = 0;
    this->nombreUsuario = "";
    this->rolUsuario = "USER";
}

Usuario::Usuario(int id, std::string nombre, std::string rol) {
    this->idUsuario = id;
    this->nombreUsuario = nombre;
    this->rolUsuario = rol;
}

Usuario::~Usuario() {
    // sin punteros que liberar por ahora
}

///getters and setters
void Usuario::setId(int id) {
    this->idUsuario = id;
}

int Usuario::getId() const {
    return this->idUsuario;
}

void Usuario::setNombre(std::string nombre) {
    this->nombreUsuario = nombre;
}

std::string Usuario::getNombre() const {
    return this->nombreUsuario;
}

void Usuario::setRol(std::string rol) {
    this->rolUsuario = rol;
}

std::string Usuario::getRol() const {
    return this->rolUsuario;
}

///conexion con el archivo CSV
std::string Usuario::toCSV() const {
    return std::to_string(this->idUsuario) + "," + this->nombreUsuario + "," + this->rolUsuario;
}

Usuario Usuario::fromCSV(const std::string& linea) {
    std::string campos[3];
    int campoActual = 0;
    std::string buffer;
    bool dentroDeComillas = false;

    for (char c : linea) {
        if (c == '"') {
            dentroDeComillas = !dentroDeComillas;
        } else if (c == ',' && !dentroDeComillas) {
            campos[campoActual++] = buffer;
            buffer.clear();
        } else {
            buffer += c;
        }
    }
    campos[campoActual] = buffer;

    int id = std::stoi(campos[0]);
    std::string nombre = campos[1];
    std::string rol = campos[2];

    return Usuario(id, nombre, rol);
}
