#include "UsuarioController.h"

// ======================================================
// CONSTRUCTOR Y DESTRUCTOR
// ======================================================

UsuarioController::UsuarioController()
{
    this->listaUsuarios = new ListaDoble();
    this->archivoUsuarios = new UsuarioDataBase();
}

UsuarioController::~UsuarioController()
{
    delete listaUsuarios;
    delete archivoUsuarios;
}

// ======================================================
// GUARDAR USUARIOS
// ======================================================

void UsuarioController::guardarUsuarios()
{
    archivoUsuarios->guardarUsuariosEnArchivo(listaUsuarios);
}

// ======================================================
// CARGAR USUARIOS
// ======================================================

void UsuarioController::cargarUsuarios()
{
    ListaDoble* nuevaLista = nullptr;

    try
    {
        nuevaLista = archivoUsuarios->cargarUsuariosDesdeArchivo();
    }
    catch (const std::exception& e)
    {
        delete nuevaLista;
        throw;
    }

    if (listaUsuarios != nullptr)
    {
        delete listaUsuarios;
    }

    listaUsuarios = nuevaLista;
}

// ======================================================
// AGREGAR USUARIO
// ======================================================

void UsuarioController::agregarUsuario(Usuario* usuario)
{
    if (usuario == nullptr)
    {
        throw std::invalid_argument(
            "No se puede agregar un usuario nulo."
        );
    }

    // Validamos que no exista otro usuario con el mismo ID
    if (listaUsuarios->buscarPorId(usuario->getId()) != nullptr)
    {
        throw std::invalid_argument(
            "Ya existe un usuario con ese ID."
        );
    }

    listaUsuarios->agregarUsuario(usuario);

    archivoUsuarios->agregarUsuario(usuario);
}

// ======================================================
// ACTUALIZAR USUARIO
// ======================================================

bool UsuarioController::actualizarUsuario(
    int id,
    Usuario* usuario)
{
    if (usuario == nullptr)
    {
        return false;
    }

    bool actualizado =
        listaUsuarios->actualizarUsuario(id, usuario);

    if (actualizado)
    {
        guardarUsuarios();
    }

    return actualizado;
}

// ======================================================
// ELIMINAR USUARIO
// ======================================================

bool UsuarioController::eliminarUsuario(int idUsuario)
{
    bool eliminado =
        listaUsuarios->eliminarUsuario(idUsuario);

    if (eliminado)
    {
        guardarUsuarios();
    }

    return eliminado;
}

// ======================================================
// BUSCAR USUARIO POR ID
// ======================================================

Usuario* UsuarioController::buscarUsuarioPorId(
    int idUsuario)
{
    return listaUsuarios->buscarPorId(idUsuario);
}

std::vector<Usuario*> UsuarioController::listarUsuarios()
{
    return listaUsuarios->listarUsuarios();
}