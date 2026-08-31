#include "UsuarioDataBase.h"
#include <iostream>
#include <direct.h>
#include "../../structures/ListaDoble/ListaDoble.h"
#include "../../structures/ListaDoble/nodoUsuario.h"

const std::string UsuarioDataBase::FILENAME_USUARIOS = "data/usuarios.csv"; // Nombre del archivo para almacenar los usuarios
namespace { void asegurarDirectorioData(){ _mkdir("data"); _mkdir("bin/data"); } }
// Formato de guardado ----> idUsuario,"nombreUsuario",rolUsuario,hashContrasena

//                                  === === ===     CONSTRUCTOR Y DESTRUCTOR    === === ===

UsuarioDataBase::UsuarioDataBase() {}

UsuarioDataBase::~UsuarioDataBase() {}

//                                  === === ===     FUNCIONES PRIVATE AUXILIARES    === === ===

namespace {
	// Escapa las comillas dentro de un texto para poder guardarlo como campo CSV.
	std::string escaparCSV(const std::string& texto) {
		std::string resultado;
		for (char c : texto) {
			if (c == '"') {
				resultado += "\"\"";
			}
			else {
				resultado += c;
			}
		}
		return resultado;
	}

	// Divide una linea CSV en campos, respetando comillas dobles que envuelven
	// los campos. Una comilla doble dentro de un campo se escribe como "".
	std::vector<std::string> dividirCamposCSV(const std::string& linea) {
		std::vector<std::string> campos;
		std::string actual;
		bool dentroDeComillas = false;

		for (size_t i = 0; i < linea.size(); ++i) {
			char c = linea[i];
			if (c == '"') {
				if (dentroDeComillas && i + 1 < linea.size() && linea[i + 1] == '"') {
					actual += '"';
					++i;
				}
				else {
					dentroDeComillas = !dentroDeComillas;
				}
			}
			else if (c == ',' && !dentroDeComillas) {
				campos.push_back(actual);
				actual.clear();
			}
			else {
				actual += c;
			}
		}

		campos.push_back(actual);
		return campos;
	}
}

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
		}
		catch (...) {}
	}
	return false;
}

std::string UsuarioDataBase::formularLinea(Usuario* usuario) { // formula la linea que se guardara en el archivo
	std::stringstream linea;
	linea << usuario->getId() << ",\"" << escaparCSV(usuario->getNombre()) << "\","														
		  << rolATexto(usuario->getRol()) << "," << usuario->getHashContrasena();
	return linea.str();
}

//                                  === === ===     METODOS PUBLICOS    === === ===

void UsuarioDataBase::guardarUsuariosEnArchivo(ListaDoble* lista) { // guarda todos los usuarios de la lista en el archivo
	if (lista == nullptr) throw std::invalid_argument("La lista de usuarios es nula");
	asegurarDirectorioData();
	std::ofstream archivoTemporal("data/temporalUsuarios.csv");
	if (!archivoTemporal.is_open()) throw std::runtime_error("Error al abrir el archivo temporal de usuarios.");

	// Guardamos todos los usuarios de la lista en el archivo temporal
	std::vector<Usuario*> usuarios = lista->listarUsuarios();
	for (Usuario* usuario : usuarios) {
		archivoTemporal << formularLinea(usuario) << std::endl;
	}
	archivoTemporal.close();

	// Reemplazamos el archivo original con el temporal
	std::remove(FILENAME_USUARIOS.c_str()); // si el archivo no existia, no es un error
	if (std::rename("data/temporalUsuarios.csv", FILENAME_USUARIOS.c_str()) != 0)
		throw std::runtime_error("Error al renombrar el archivo temporal de usuarios.");
}

ListaDoble* UsuarioDataBase::cargarUsuariosDesdeArchivo() { // carga los usuarios del archivo hacia una nueva lista
	std::ifstream archivoUsuarios(FILENAME_USUARIOS);
	if (!archivoUsuarios.is_open()) return new ListaDoble(); // archivo no existe, retorna lista vacia

	ListaDoble* nuevaLista = new ListaDoble();
	std::string linea;

	while (std::getline(archivoUsuarios, linea)) {
		Usuario* nuevoUsuario = nullptr;
		bool agregado = false;
		try {
			std::vector<std::string> campos = dividirCamposCSV(linea);
			if (campos.size() < 4) {
				std::cerr << "Advertencia: linea con formato invalido en usuarios.csv: " << linea << std::endl;
				continue;
			}

			int id = std::stoi(campos[0]);
			Usuario::Rol rol = textoARol(campos[2]);
			size_t hashContrasena = std::stoull(campos[3]);

			nuevoUsuario = new Usuario(id, campos[1], rol);
			nuevoUsuario->setHashDirecto(hashContrasena);
			nuevaLista->agregarUsuario(nuevoUsuario); // se agrega directo, no hay dependencia de orden
			agregado = true;
		}
		catch (const std::exception& e) {
			if (nuevoUsuario != nullptr && !agregado) delete nuevoUsuario;
			std::cerr << "Advertencia: error al procesar linea de usuarios.csv: " << e.what() << std::endl;
		}
	}
	archivoUsuarios.close();
	return nuevaLista;
}

void UsuarioDataBase::agregarUsuario(Usuario* usuario) { // agrega un usuario nuevo al archivo (append)
	if (usuario == nullptr) throw std::invalid_argument("No se puede agregar un usuario nulo");
	if (existeUsuarioConId(usuario->getId())) throw std::invalid_argument("Ya existe un usuario con el id proporcionado");
	asegurarDirectorioData();
	std::ofstream archivoUsuarios(FILENAME_USUARIOS, std::ios::app);
	if (!archivoUsuarios.is_open()) throw std::runtime_error("Error al abrir el archivo de usuarios.");
	archivoUsuarios << formularLinea(usuario) << std::endl;
	archivoUsuarios.close();
}

void UsuarioDataBase::eliminarUsuario(int idUsuario) { // elimina un usuario del archivo por id
	if (idUsuario < 0) throw std::invalid_argument("Error: El id del usuario no puede ser negativo.");
	asegurarDirectorioData();
	std::ifstream archivoUsuarios(FILENAME_USUARIOS);
	if (!archivoUsuarios.is_open()) throw std::runtime_error("Error al abrir el archivo de usuarios.");

	std::ofstream archivoTemporal("data/temporalUsuarios.csv");
	if (!archivoTemporal.is_open()) throw std::runtime_error("Error al crear el archivo temporal.");

	std::string linea;
	bool encontrado = false;
	while (std::getline(archivoUsuarios, linea)) {
		std::stringstream lineaActual(linea);
		std::string idTXT;
		std::getline(lineaActual, idTXT, ',');

		if (idTXT != std::to_string(idUsuario)) {
			archivoTemporal << linea << std::endl;
		}
		else {
			encontrado = true;
		}
	}
	archivoUsuarios.close();
	archivoTemporal.close();

	if (!encontrado) {
		std::remove("data/temporalUsuarios.csv");
		throw std::invalid_argument("No existe un usuario con el id proporcionado.");
	}

	std::remove(FILENAME_USUARIOS.c_str()); // si el archivo no existia, no es un error
	if (std::rename("data/temporalUsuarios.csv", FILENAME_USUARIOS.c_str()) != 0)
		throw std::runtime_error("Error al renombrar el archivo temporal de usuarios.");
}