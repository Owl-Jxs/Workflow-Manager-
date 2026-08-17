#include <iostream>
#include "tests/PruebasTarea/PruebasTarea.h"
#include "tests/PruebasUsuario/PruebasUsuario.h"
#include "Utilities/ValidarEntrada.h"

int main () {
    //PruebasTarea::ejecutarTodas();
    //PruebasUsuario::ejecutarTodas();
    std::string contrasena = ValidarEntrada::validarContrasena ("Ingrese su contrasena", '*');
    std::cout << contrasena;
    return 0;
}   