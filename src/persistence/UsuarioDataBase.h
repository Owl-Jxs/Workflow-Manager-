#ifndef USUARIO_DATABASE_H
#define USUARIO_DATABASE_H

#include "ListaDoble.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

class UsuarioDataBase {

private:

    static const std::string FILENAME_USUARIOS;

    // Funciones auxiliares para guardar y cargar
    std::string formularLinea(Usuario* usuario);

    void guardarLista(ListaDoble* lista, std::string nombreArchivo);

    ListaDoble* cargarLista(std::string nombreArchivo);

public:

    UsuarioDataBase();
    ~UsuarioDataBase();

    // Funciones para manipular la base de datos de usuarios
    void guardarListaEnArchivo(ListaDoble* listaCompleta);

    ListaDoble* cargarListaDelArchivo();

    // Función para agregar un usuario al archivo
    void guardarNuevoUsuarioEnArchivo(Usuario* usuario);
};

#endif