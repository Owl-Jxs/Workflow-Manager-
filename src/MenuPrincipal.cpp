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
        cout << "Advertencia: no se pudieron cargar los usuarios.\n";
        cout << e.what() << endl;
    }
}

void MenuPrincipal::iniciarSesion()
{
    int id;

    cout << "\n================================\n";
    cout << "       WORKFLOW MANAGER\n";
    cout << "================================\n";

    while (true)
    {
        cout << "\nIngrese su ID de usuario";
        cout << " (0 para salir): ";

        if (!(cin >> id))
        {
            cout << "Error: debe ingresar un numero entero.\n";

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (id == 0)
        {
            cout << "Saliendo del programa...\n";
            return;
        }

        Usuario* usuario = uc->buscarUsuarioPorId(id);

        if (usuario == nullptr)
        {
            cout << "\nNo existe un usuario con ese ID.\n";
            cout << "Intente nuevamente.\n";
            continue;
        }

        cout << "\nInicio de sesion exitoso.\n";
        cout << "Bienvenido/a, "
             << usuario->getNombre() << "!\n";

        mostrarMenuSegunRol(usuario);

        return;
    }
}

MenuPrincipal::~MenuPrincipal()
{
    delete uv;
    delete uc;
}