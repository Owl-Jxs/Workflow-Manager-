#include "MenuPrincipal.h"

#include <iostream>
#include <limits>
#include <exception>
#include "Utilities/ValidarEntrada.h"

using namespace std;

MenuPrincipal::MenuPrincipal()
{
    uc = new UsuarioController();
    tc = new TareaController();
    ac = new AsignacionController();
    auditoria = new AuditoriaDataBase();
    gestorHistorial = new GestorHistorial(auditoria);
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

    try
    {
        tc->cargarArchivos();
    }
    catch (const exception& e)
    {
        cout << "Advertencia: no se pudieron cargar las tareas.\n";
        cout << e.what() << endl;
    }

    try
    {
        ac->cargarAsignaciones();
    }
    catch (const exception& e)
    {
        cout << "Advertencia: no se pudieron cargar las asignaciones.\n";
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

        string contrasena = ValidarEntrada::validarContrasena(
            "Ingrese su contrasena: ", '*');

        hash<string> hasher;
        if (hasher(contrasena) != usuario->getHashContrasena())
        {
            cout << "Contrasena incorrecta.\n";
            continue;
        }

        gestorHistorial->setIdUsuarioSesion(usuario->getId());

        cout << "\nInicio de sesion exitoso.\n";
        cout << "Bienvenido/a, "
             << usuario->getNombre() << "!\n";

        mostrarMenuSegunRol(usuario);

        return;
    }
}

void MenuPrincipal::mostrarMenuSegunRol(Usuario* usuario)
{
    if (usuario->getRol() == Usuario::Rol::ADMINISTRADOR)
    {
        mostrarMenuAdministrador(usuario);
    }
    else
    {
        mostrarMenuUsuarioNormal(usuario);
    }
}

void MenuPrincipal::mostrarMenuAdministrador(Usuario* usuario)
{
    int opcion;

    do
    {
        cout << "\n==============================\n";
        cout << "    MENU ADMINISTRADOR\n";
        cout << "==============================\n";
        cout << "1. Gestionar usuarios\n";
        cout << "2. Gestionar tareas\n";
        cout << "3. Deshacer ultima accion\n";
        cout << "4. Rehacer ultima accion\n";
        cout << "0. Cerrar sesion\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion))
        {
            cout << "Error: debe ingresar un numero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion)
        {
            case 1:
                uv->mostrarMenuAdministrador();
                break;

            case 2:
                cout << "Modulo de gestion de tareas: pendiente de integrar.\n";
                break;

            case 3:
                gestorHistorial->deshacer();
                cout << "Accion deshecha.\n";
                break;

            case 4:
                gestorHistorial->rehacer();
                cout << "Accion rehecha.\n";
                break;

            case 0:
                cout << "Cerrando sesion...\n";
                break;

            default:
                cout << "Opcion no valida.\n";
                break;
        }

    } while (opcion != 0);
}

void MenuPrincipal::mostrarMenuUsuarioNormal(Usuario* usuario)
{
    int opcion;

    do
    {
        cout << "\n==============================\n";
        cout << "    MENU USUARIO NORMAL\n";
        cout << "==============================\n";
        cout << "1. Ver tareas\n";
        cout << "2. Cambiar estado de tarea asignada\n";
        cout << "0. Cerrar sesion\n";
        cout << "Seleccione una opcion: ";

        if (!(cin >> opcion))
        {
            cout << "Error: debe ingresar un numero.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (opcion)
        {
            case 1:
                cout << "Modulo de tareas: pendiente de integrar.\n";
                break;

            case 2:
                cout << "Cambio de estado: pendiente de integrar.\n";
                break;

            case 0:
                cout << "Cerrando sesion...\n";
                break;

            default:
                cout << "Opcion invalida.\n";
                break;
        }

    } while (opcion != 0);
}

MenuPrincipal::~MenuPrincipal()
{
    delete gestorHistorial;
    delete auditoria;
    delete uv;
    delete ac;
    delete tc;
    delete uc;
}
