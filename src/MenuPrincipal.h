#ifndef MENUPRINCIPAL_H
#define MENUPRINCIPAL_H

#include "Controllers/UsuarioController.h"
#include "ui/UsuarioView.h"

class MenuPrincipal
{
private:

    UsuarioController* uc;
    UsuarioView* uv;

public:

    MenuPrincipal();
    ~MenuPrincipal();

    void iniciarSesion();

    void mostrarMenuSegunRol(Usuario* usuario);

    void mostrarMenuAdministrador();

    void mostrarMenuUsuarioNormal();
};

#endif