#ifndef USUARIOVIEW_H
#define USUARIOVIEW_H
#include "../../Controllers/UsuarioController.h"
#include "../../models/Usuario/Usuario.h"
#include "../../Utilities/ValidarEntrada.h"
#include "../../structures/Gestor/GestorHistorial.h"
#include <iostream>
#include <string>
#include <limits>

class UsuarioView
{
private:
    UsuarioController* uc;
    Usuario* usuarioActivo;
    GestorHistorial* gestorHistorial;
    void agregarUsuario ();
    void actualizarUsuario ();
    void eliminarUsuario ();
    void mostrarLista ();
    void mostrarUsuarioPorId ();

public:
    UsuarioView(UsuarioController* uc, Usuario* uA, GestorHistorial* gH);
    void mostrarMenuAdministrador();
    void mostrarMenuUsuarioNormal();
    Usuario* leerNuevoUsuario();
};


#endif
