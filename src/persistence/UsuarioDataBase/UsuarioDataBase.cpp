#include "UsuarioDataBase.h"
#include <iostream>
#include "../../structures/ListaDoble/ListaDoble.h"
#include "../../structures/ListaDoble/nodoUsuario.h"

const std::string UsuarioDataBase::FILENAME_USUARIOS = "data/usuarios.csv"; // Nombre del archivo para almacenar los usuarios
// Formato de guardado ----> idUsuario,"nombreUsuario",rolUsuario

//                                  === === ===     CONSTRUCTOR Y DESTRUCTOR    === === ===

UsuarioDataBase::UsuarioDataBase() {}

UsuarioDataBase::~UsuarioDataBase() {}

//                                  === === ===     FUNCIONES PRIVATE AUXILIARES    === === ===

std::string UsuarioDataBase::rolATexto(Usuario::Rol rol) { // convierte el enum Rol a texto para el csv
    return (rol == Usuario::Rol::ADMINISTRADOR) ? "ADMINISTRADOR" : "USUARIO_NORMAL";
}

Usuario::Rol UsuarioDataBase::textoARol(const std::string& texto) { // convierte el texto del csv de vuelta a Rol
    if (texto == "ADMINISTRADOR") return Usuario::Rol::ADMINISTRADOR;
    if (texto == "USUARIO_NORMAL") return Usuario::Rol::USUARIO_NORMAL;
    throw std::invalid_argument("Rol desconocido en usuarios.csv: " + texto);
}

bool UsuarioDataBase::existeUsuarioConId(int id) { // verifica si un id ya existe en el archivo
    std::ifstream archivo(FILENAME_USUARIOS);
    if (!archivo.is_open()) return false; // si no existe, el id no esta duplicado

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream lineaActual(linea);
        std::string idTXT;
        std::getline(lineaActual, idTXT, ',');
        try {
            if (std::stoi(idTXT) == id) return true;
        } catch (...) { }
    }
    return false;
}

std::string UsuarioDataBase::formularLinea(Usuario* usuario) { // formula la linea que se guardara en el archivo
    std::stringstream linea;
    linea << usuario->getId() << ",\"" << usuario->getNombre() << "\"," << rolATexto(usuario->getRol());
    return linea.str();
}

//                                  === === ===     METODOS PUBLICOS    === === ===

void UsuarioDataBase::guardarUsuariosEnArchivo(ListaDoble* lista) { // guarda todos los usuarios de la lista en el archivo
    if (lista == nullptr) throw std::invalid_argument("La lista de usuarios es nula");

    std::ofstream archivoTemporal("data/temporalUsuarios.csv");
    if (!archivoTemporal.is_open()) throw std::runtime_error("Error al abrir el archivo temporal de usuarios.");

    // Guardamos todos los usuarios de la lista en el archivo temporal
    std::vector<Usuario*> usuarios = lista->listarUsuarios();
    for (Usuario* usuario : usuarios) {
        archivoTemporal << formularLinea(usuario) << std::endl;
    }
    archivoTemporal.close();

    // Reemplazamos el archivo original con el temporal
    std::remove(FILENAME_USUARIOS.c_str());
    if (std::rename("data/temporalUsuarios.csv", FILENAME_USUARIOS.c_str()) != 0)
        throw std::runtime_error("Error al renombrar el archivo temporal de usuarios.");
}

ListaDoble* UsuarioDataBase::cargarUsuariosDesdeArchivo() { // carga los usuarios del archivo hacia una nueva lista
    std::ifstream archivoUsuarios(FILENAME_USUARIOS);
    if (!archivoUsuarios.is_open()) return new ListaDoble(); // archivo no existe, retorna lista vacia

    ListaDoble* nuevaLista = new ListaDoble();
    std::string linea;

    while (std::getline(archivoUsuarios, linea)) {
        try {
            std::stringstream lineaActual(linea);
            std::string idTXT, nombreTXT, rolTXT;

            if (std::getline(lineaActual, idTXT, ',') && std::getline(lineaActual, nombreTXT, ',')
                && std::getline(lineaActual, rolTXT)) {

                // Quitamos las comillas que rodean al nombre
                if (nombreTXT.size() >= 2 && nombreTXT.front() == '"' && nombreTXT.back() == '"') {
                    nombreTXT = nombreTXT.substr(1, nombreTXT.size() - 2);
                }

                int id = std::stoi(idTXT);
                Usuario::Rol rol = textoARol(rolTXT);

                Usuario* nuevoUsuario = new Usuario(id, nombreTXT, rol);
                nuevaLista->agregarUsuario(nuevoUsuario); // se agrega directo, no hay dependencia de orden
            }
            else {
                std::cerr << "Advertencia: linea con formato invalido en usuarios.csv: " << linea << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Advertencia: error al procesar linea de usuarios.csv: " << e.what() << std::endl;
        }
    }
    archivoUsuarios.close();
    return nuevaLista;
}

void UsuarioDataBase::agregarUsuario(Usuario* usuario) { // agrega un usuario nuevo al archivo (append)
    if (usuario == nullptr) throw std::invalid_argument("No se puede agregar un usuario nulo");
    if (existeUsuarioConId(usuario->getId())) throw std::invalid_argument("Ya existe un usuario con el id proporcionado");

    std::ofstream archivoUsuarios(FILENAME_USUARIOS, std::ios::app);
    if (!archivoUsuarios.is_open()) throw std::runtime_error("Error al abrir el archivo de usuarios.");
    archivoUsuarios << formularLinea(usuario) << std::endl;
    archivoUsuarios.close();
}

void UsuarioDataBase::eliminarUsuario(int idUsuario) { // elimina un usuario del archivo por id
    std::ifstream archivoUsuarios(FILENAME_USUARIOS);
    if (!archivoUsuarios.is_open()) throw std::runtime_error("Error al abrir el archivo de usuarios.");

    std::ofstream archivoTemporal("data/temporalUsuarios.csv");
    if (!archivoTemporal.is_open()) throw std::runtime_error("Error al crear el archivo temporal.");

    std::string linea;
    while (std::getline(archivoUsuarios, linea)) {
        std::stringstream lineaActual(linea);
        std::string idTXT;
        std::getline(lineaActual, idTXT, ',');

        if (idTXT != std::to_string(idUsuario)) {
            archivoTemporal << linea << std::endl;
        }
    }
    archivoUsuarios.close();
    archivoTemporal.close();

    std::remove(FILENAME_USUARIOS.c_str());
    if (std::rename("data/temporalUsuarios.csv", FILENAME_USUARIOS.c_str()) != 0)
        throw std::runtime_error("Error al renombrar el archivo temporal de usuarios.");
}