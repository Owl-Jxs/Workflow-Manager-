#ifndef LISTADOBLE_H
#define LISTADOBLE_H
#include <vector>
#include "../../models/Usuario/Usuario.h"
#include "nodoUsuario.h"

class ListaDoble {

    //lista doble
private:

    nodoUsuario* cabeza;
    nodoUsuario* cola;
    int cantidad;

public:

    ListaDoble();
    ~ListaDoble();

    void agregarUsuario(Usuario* usuario);

    bool actualizarUsuario(int id, Usuario* usuario);

    bool eliminarUsuario(int id);

    std::vector<Usuario*> listarUsuarios();

    Usuario* buscarPorId(int id);
};

#endif