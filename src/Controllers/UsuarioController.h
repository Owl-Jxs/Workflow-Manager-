#ifndef USUARIOCONTROLLER_H
#define USUARIOCONTROLLER_H

#include "UsuarioDataBase.h"
#include "ListaDoble.h"
#include <vector>

class UsuarioController
{
private:

    ListaDoble* listaUsuarios;
    UsuarioDataBase* archivoUsuarios;

public:

    UsuarioController();
    ~UsuarioController();

    void guardarUsuarios();
    void cargarUsuarios();

    void agregarUsuario(Usuario* usuario);

    bool actualizarUsuario(int id, Usuario* usuario);

    bool eliminarUsuario(int idUsuario);

    Usuario* buscarUsuarioPorId(int idUsuario);
    std::vector<Usuario*> listarUsuarios();
};

#endif