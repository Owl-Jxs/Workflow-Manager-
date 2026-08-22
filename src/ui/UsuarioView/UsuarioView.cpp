#include "UsuarioView.h"

#include <iostream>
#include <limits>
#include <vector>
#include <exception>

using namespace std;

UsuarioView::UsuarioView (UsuarioController* controller, Usuario* uA, GestorHistorial* gH){
    uc = controller;
    usuarioActivo = uA;
    gestorHistorial = gH;
}

Usuario* UsuarioView::leerNuevoUsuario() {
    int id;
    string nombre;
    int opcionRol;
    Usuario::Rol rol;
    Usuario* nuevoUsuario;
    id = ValidarEntrada::validarCodigoNumerico ("Ingrese su identificacion", 9);
    nombre =ValidarEntrada::validarNombreCompleto ();

    std::cout << "\nSeleccione el rol:\n";
    std::cout << "1. Administrador\n";
    std::cout << "2. Usuario Normal\n";
    opcionRol = ValidarEntrada::validarEntradaRango ("Ingrese su opcion", 1,2);

    switch (opcionRol) {
        case 1: {
            rol = Usuario::Rol::ADMINISTRADOR; break;
        }
        case 2: {
            rol = Usuario::Rol::USUARIO_NORMAL; break;
        }
        default:{
            cout << "Error: seleccione 1 o 2.\n"; break;
        }
            nuevoUsuario = new Usuario (id, nombre, rol);
    };

    
    string contrasena = ValidarEntrada::validarContrasena ("Ingrese la contrasena: ", '*');
    nuevoUsuario->setHashContrasena (contrasena);
    return nuevoUsuario;
}

void UsuarioView::agregarUsuario  () {
    Usuario* usuario = leerNuevoUsuario();
    try {
        
        uc->agregarUsuario(usuario);
        cout << "Usuario agregado correctamente.\n";

    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
        delete usuario;
    }
}

void UsuarioView::actualizarUsuario () {
    int id = ValidarEntrada::validarCodigoNumerico ("Ingrese el id del usuario buscado", 9);
    Usuario* usuarioActual = uc->buscarUsuarioPorId(id);

    if (usuarioActual == nullptr) {
        std::cout << "No existe un usuario con ese ID.\n";
        return;
    }

    std::cout << "\nUsuario encontrado:\n";
    std::cout << "ID: " << usuarioActual->getId() << endl;
    std::cout << "Nombre: " << usuarioActual->getNombre() << endl;

    bool cambioNombre = ValidarEntrada::respuestas_Si_O_No ("Cambiar el nombre", "mantener nombre actual");
    if (cambioNombre) {
        std::string nuevoNombre = ValidarEntrada::validarNombreCompleto ();
        usuarioActual->setNombre (nuevoNombre);
    }

    bool cambioRol = ValidarEntrada::respuestas_Si_O_No ("Cambiar el rol", "mantener el Rol actual");
    if (cambioRol) {
        std::cout << "\nNuevo rol:\n";
        std::cout << "1. Administrador\n";
        std::cout << "2. Usuario Normal\n";
        int opcionRol = ValidarEntrada::validarEntradaRango ("Ingrese su opcion", 1,2);

        Usuario::Rol rol = ((opcionRol == 1) ? Usuario::Rol::ADMINISTRADOR : Usuario::Rol::USUARIO_NORMAL); 
        usuarioActual->setRol (rol);
    }
}

void UsuarioView::eliminarUsuario () {
    int id = ValidarEntrada::validarCodigoNumerico ("Digite el id del usuario", 9);
    Usuario* usuario = uc->buscarUsuarioPorId(id);

    if (usuario == nullptr) {
        std::cout << "No existe un usuario con ese ID.\n"; return;
    }

    std::cout << "\nUsuario encontrado:\n";
    std::cout << "ID: " << usuario->getId() << endl;
    std::cout << "Nombre: " << usuario->getNombre() << endl;
    std::cout << "¿Esta seguro de eliminarlo? \n";
    bool eliminarlo = ValidarEntrada::respuestas_Si_O_No ("Si", "No");
     
    if (eliminarlo) { 
        if (uc->eliminarUsuario(id)){
                cout << "Usuario eliminado correctamente.\n";
        } else {
            cout << "No se pudo eliminar el usuario.\n";
        }

    } else {
    cout << "Operacion cancelada.\n";
    }
}

void UsuarioView::mostrarLista () {
    vector<Usuario*> usuarios = uc->listarUsuarios();
    if (usuarios.empty()) 
    {
        cout << "No hay usuarios registrados.\n"; return;
    }

    cout << "\n========== LISTA DE USUARIOS ==========\n";
    for (Usuario* usuario : usuarios) {
        
        std::cout << "ID: " << usuario->getId() << endl;
        std::cout << "Nombre: " << usuario->getNombre() << endl;
        std::cout << ((usuario->getRol() == Usuario::Rol::ADMINISTRADOR) ? "Rol: ADMINISTRADOR\n" : "Rol: USUARIO_NORMAL\n");
        std::cout << "--------------------------------------\n";
    }
}

void UsuarioView::mostrarUsuarioPorId () {
    int id =  ValidarEntrada::validarCodigoNumerico ("Ingrese el id del usuario", 9);
    Usuario* usuario = uc->buscarUsuarioPorId(id);

    if (usuario == nullptr) {
        std::cout << "No existe un usuario con ese ID.\n"; return;
    }

    std::cout << "\n========== USUARIO ENCONTRADO ==========\n";
    std::cout << "ID: " << usuario->getId() << endl;
    std::cout << "Nombre: " << usuario->getNombre() << endl;
    ((usuario->getRol() == Usuario::Rol::ADMINISTRADOR) ? "Rol: ADMINISTRADOR\n" : "Rol: USUARIO_NORMAL\n");
}


void UsuarioView::mostrarMenuAdministrador() {
    int opcion;
    do
    {
        std::cout << "\n==============================\n"
         << "      GESTION DE USUARIOS\n"
         << "==============================\n"
         << "1. Agregar usuario\n"
         << "2. Actualizar usuario\n"
         << "3. Eliminar usuario\n"
         << "4. Listar usuarios\n"
         << "5. Buscar usuario por ID\n"
         << "0. Salir\n";

        opcion = ValidarEntrada::validarEntradaRango ("Seleccione una opcion: ",0,5);

        switch (opcion)
        {
            case 1:
            {
                agregarUsuario (); break;
            }

            case 2:
            {
                actualizarUsuario (); break;
            }

            case 3:
            {
               eliminarUsuario ();
            }

            case 4:
            {
               mostrarLista ();
            }

            case 5:
            {
               mostrarUsuarioPorId ();
            }

            case 0:
                cout << "Saliendo del menu de usuarios...\n"; break;

            default:
                cout << "Error: opcion no valida.\n"; break;
        }

    } while (opcion != 0);
}

/* <---------- Quitar, esto va en tarea, ya que no hay acceso al viewUsuario si el usuario activo no es administrador
void UsuarioView::mostrarMenuUsuarioNormal()
{
    int opcion;

    do
    {
        cout << "\n==============================\n";
        cout << "      MENU USUARIO NORMAL\n";
        cout << "==============================\n";
        cout << "1. Ver tareas\n";
        cout << "2. Cambiar estado de tarea asignada\n";
        cout << "0. Cerrar sesion\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion))
        {
            cout <<
                "Error: debe ingresar un numero.\n";

            cin.clear();
            cin.ignore(
                numeric_limits<streamsize>::max(),
                '\n'
            );

            continue;
        }

        cin.ignore(
            numeric_limits<streamsize>::max(),
            '\n'
        );

        switch (opcion)
        {
            case 1:
                cout <<
                    "Modulo de tareas: pendiente de integrar.\n";
                break;

            case 2:
                cout <<
                    "Cambio de estado: pendiente de integrar.\n";
                break;

            case 0:
                cout <<
                    "Cerrando sesion...\n";
                break;

            default:
                cout <<
                    "Opcion invalida.\n";
                break;
        }

    } while (opcion != 0);
}

*/