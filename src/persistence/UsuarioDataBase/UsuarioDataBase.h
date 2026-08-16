#ifndef USUARIODATABASE_H
#define USUARIODATABASE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "../../models/Usuario/Usuario.h"

class ListaDoble;

class UsuarioDataBase {

private:
	static const std::string FILENAME_USUARIOS; // Nombre del archivo para almacenar los usuarios
	// Formato de guardado ----> idUsuario,"nombreUsuario",rolUsuario,hashContrasena

	// === Funciones privadas auxiliares ===
	std::string formularLinea(Usuario* usuario);                    // arma la linea de texto para un usuario
	std::string rolATexto(Usuario::Rol rol);                         // convierte el enum Rol a texto para el csv
	Usuario::Rol textoARol(const std::string& texto);                 // convierte el texto del csv de vuelta a Rol
	bool existeUsuarioConId(int id);                                  // verifica si un id ya existe en el archivo

public:
	UsuarioDataBase();
	~UsuarioDataBase();

	void guardarUsuariosEnArchivo(ListaDoble* lista);          // guarda todos los usuarios de la lista en el archivo
	ListaDoble* cargarUsuariosDesdeArchivo();                   // carga los usuarios del archivo hacia una nueva lista

	void agregarUsuario(Usuario* usuario);                      // agrega un usuario nuevo al archivo (append)
	void eliminarUsuario(int idUsuario);                         // elimina un usuario del archivo por id
};

#endif