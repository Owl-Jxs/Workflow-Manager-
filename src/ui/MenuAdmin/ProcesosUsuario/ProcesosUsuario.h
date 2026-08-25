#ifndef PROCESOSUSUARIO_H
#define PROCESOSUSUARIO_H

#include "../../../Controllers/UsuarioController.h"
#include "../../../models/Usuario/Usuario.h"
#include "../../../Utilities/ValidarEntrada.h"
#include "../../../structures/Gestor/GestorHistorial.h"
#include "../../../models/Comandos/ComandoUsuario.h"

#include <iostream>
#include <string>
#include <exception>
class ProcesosUsuario {

private:
    UsuarioController* uc;
    Usuario* usuarioActivo;
    GestorHistorial* gestorHistorial;
    

public:
    ProcesosUsuario (UsuarioController* uc, Usuario* uA, GestorHistorial* gH);
    void agregarUsuario ();
    void actualizarUsuario ();
    void eliminarUsuario ();
    void mostrarLista ();
    void mostrarUsuarioPorId ();
    Usuario* leerNuevoUsuario();

};

#endif