#ifndef PROCESOSUSUARIO_H
#define PROCESOSUSUARIO_H

#include "../../../Controllers/UsuarioController.h"
#include "../../../models/Usuario/Usuario.h"
#include "../../../Utilities/ValidarEntrada.h"
#include "../../../structures/Gestor/GestorHistorial.h"
#include "../../../models/Comandos/ComandoUsuario.h"
#include "../../../algorithms/Condiciones/Icondicion.h"
#include "../../../algorithms/Condiciones/UsuarioCondicion.h"
#include "../../../algorithms/MergeSort/MergeSort.h"
#include "../../../algorithms/QuickSort/QuickSort.h"


#include <iostream>
#include <string>
#include <exception>
class ProcesosUsuario {

private:
    UsuarioController* uc;
    Usuario* usuarioActivo;
    GestorHistorial* gestorHistorial;
    void mostrarInformacionUsuario (Usuario* usuario);   

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