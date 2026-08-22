#include "MenuPrincipal.h"

#include <iostream>
#include <limits>
#include <exception>

using namespace std;

MenuPrincipal::MenuPrincipal()
{
    uc = new UsuarioController();
    uv = new UsuarioView(uc);

    try
    {
        uc->cargarUsuarios();
    }
    catch (const exception& e)
    {
        cout << "Advertencia: no se pudieron cargar los usuarios." << endl;
        cout << e.what() << endl;
    }
}

MenuPrincipal::~MenuPrincipal()
{
    delete uv;
    delete uc;
}

void MenuPrincipal::iniciarSesion()
{
    int id;

    cout << "\n====================================" << endl;
    cout << "        WORKFLOW MANAGER" << endl;
    cout << "====================================" << endl;

    while (true)
    {
        cout << "\nIngrese su ID de usuario";
        cout << " (0 para salir): ";

        if (!(cin >> id))
        {
            cout << "Error: debe ingresar un numero entero." << endl;

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

        if (id == 0)
        {
            cout << "Saliendo del programa..." << endl;
            return;
        }

        Usuario* usuario = uc->buscarUsuarioPorId(id);

        if (usuario == nullptr)
        {
            cout << "\nNo existe un usuario con ese ID." << endl;
            cout << "Intente nuevamente." << endl;
            continue;
        }

        cout << "\nInicio de sesion exitoso." << endl;
        cout << "Bienvenido/a, "
             << usuario->getNombre() << "!" << endl;

        mostrarMenuSegunRol(usuario);

        return;
    }
}

void MenuPrincipal::mostrarMenuSegunRol(Usuario* usuario)
{
    if (usuario == nullptr)
    {
        cout << "Error: usuario invalido." << endl;
        return;
    }

    if (usuario->getRol() == Usuario::Rol::ADMINISTRADOR)
    {
        mostrarMenuAdministrador();
    }
    else
    {
        mostrarMenuUsuarioNormal();
    }
}

void MenuPrincipal::mostrarMenuAdministrador()
{
    uv->mostrarMenuAdministrador();
}

void MenuPrincipal::mostrarMenuUsuarioNormal()
{
    uv->mostrarMenuUsuarioNormal();
}