#include <iostream>
#include "tests/PruebasTarea/PruebasTarea.h"
#include "tests/PruebasUsuario/PruebasUsuario.h"
#include "Utilities/ValidarEntrada.h"
#include "MenuPrincipal.h"

int main () {
    //PruebasTarea::ejecutarTodas();
    //PruebasUsuario::ejecutarTodas();
    //std::string contrasena = ValidarEntrada::validarContrasena ("Ingrese su contrasena", '*');
    //std::cout << contrasena;
    MenuPrincipal* menu = new MenuPrincipal ();
    menu->ejecutar ();

    //int id = ValidarEntrada::validarCodigoNumerico ("Ingrese id", 9);
    //std::cout << id;
    delete menu;
    return 0;
}   