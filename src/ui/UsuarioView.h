#ifndef USUARIOVIEW_H
#define USUARIOVIEW_H

#include "UsuarioController.h"
#include <iostream>
#include <string>
#include <limits>

class UsuarioView
{
private:

    UsuarioController* uc;

public:

    UsuarioView(UsuarioController* uc);

    void mostrarMenuAdministrador();

    void mostrarMenuUsuarioNormal();

    Usuario* leerNuevoUsuario();

    int leerIdUsuario();
};

#endif