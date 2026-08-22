#include "UsuarioView.h"

#include <iostream>
#include <limits>
#include <vector>
#include <exception>

using namespace std;

UsuarioView::UsuarioView(UsuarioController* controller)
{
    uc = controller;
}

int UsuarioView::leerIdUsuario()
{
    int id;

    while (true)
    {
        cout << "Ingrese el ID del usuario: ";

        if (cin >> id)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return id;
        }

        cout << "Error: el ID debe ser un numero entero.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

Usuario* UsuarioView::leerNuevoUsuario()
{
    int id;
    string nombre;
    int opcionRol;

    id = leerIdUsuario();

    cout << "Ingrese el nombre del usuario: ";
    getline(cin, nombre);

    while (nombre.empty())
    {
        cout << "Error: el nombre no puede estar vacio.\n";
        cout << "Ingrese el nombre del usuario: ";
        getline(cin, nombre);
    }

    while (true)
    {
        cout << "\nSeleccione el rol:\n";
        cout << "1. Administrador\n";
        cout << "2. Usuario Normal\n";
        cout << "Opcion: ";

        if (cin >> opcionRol)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (opcionRol == 1)
            {
                return new Usuario(
                    id,
                    nombre,
                    Usuario::Rol::ADMINISTRADOR
                );
            }

            if (opcionRol == 2)
            {
                return new Usuario(
                    id,
                    nombre,
                    Usuario::Rol::USUARIO_NORMAL
                );
            }
        }

        cout << "Error: seleccione 1 o 2.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void UsuarioView::mostrarMenuAdministrador()
{
    int opcion;

    do
    {
        cout << "\n==============================\n";
        cout << "      GESTION DE USUARIOS\n";
        cout << "==============================\n";
        cout << "1. Agregar usuario\n";
        cout << "2. Actualizar usuario\n";
        cout << "3. Eliminar usuario\n";
        cout << "4. Listar usuarios\n";
        cout << "5. Buscar usuario por ID\n";
        cout << "0. Salir\n";
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
            {
                Usuario* usuario = leerNuevoUsuario();

                try
                {
                    uc->agregarUsuario(usuario);
                    cout << "Usuario agregado correctamente.\n";
                }
                catch (const exception& e)
                {
                    cout << "Error: " << e.what() << endl;
                    delete usuario;
                }

                break;
            }

            case 2:
            {
                int id = leerIdUsuario();

                Usuario* usuarioActual =
                    uc->buscarUsuarioPorId(id);

                if (usuarioActual == nullptr)
                {
                    cout << "No existe un usuario con ese ID.\n";
                    break;
                }

                cout << "\nUsuario encontrado:\n";
                cout << "ID: " << usuarioActual->getId() << endl;
                cout << "Nombre: "
                     << usuarioActual->getNombre() << endl;

                string nuevoNombre;

                cout << "\nNuevo nombre: ";
                getline(cin, nuevoNombre);

                while (nuevoNombre.empty())
                {
                    cout << "El nombre no puede estar vacio.\n";
                    cout << "Nuevo nombre: ";
                    getline(cin, nuevoNombre);
                }

                int opcionRol;

                while (true)
                {
                    cout << "\nNuevo rol:\n";
                    cout << "1. Administrador\n";
                    cout << "2. Usuario Normal\n";
                    cout << "Opcion: ";

                    if (cin >> opcionRol)
                    {
                        cin.ignore(
                            numeric_limits<streamsize>::max(),
                            '\n'
                        );

                        if (opcionRol == 1 ||
                            opcionRol == 2)
                        {
                            Usuario::Rol nuevoRol =
                                (opcionRol == 1)
                                    ? Usuario::Rol::ADMINISTRADOR
                                    : Usuario::Rol::USUARIO_NORMAL;

                            Usuario* usuarioNuevo =
                                new Usuario(
                                    id,
                                    nuevoNombre,
                                    nuevoRol
                                );

                            if (uc->actualizarUsuario(
                                    id,
                                    usuarioNuevo))
                            {
                                cout <<
                                    "Usuario actualizado correctamente.\n";
                            }
                            else
                            {
                                cout <<
                                    "No se pudo actualizar el usuario.\n";

                                delete usuarioNuevo;
                            }

                            break;
                        }
                    }

                    cout <<
                        "Opcion invalida. Seleccione 1 o 2.\n";

                    cin.clear();
                    cin.ignore(
                        numeric_limits<streamsize>::max(),
                        '\n'
                    );
                }

                break;
            }

            case 3:
            {
                int id = leerIdUsuario();

                Usuario* usuario =
                    uc->buscarUsuarioPorId(id);

                if (usuario == nullptr)
                {
                    cout <<
                        "No existe un usuario con ese ID.\n";
                    break;
                }

                cout << "\nUsuario encontrado:\n";
                cout << "ID: "
                     << usuario->getId() << endl;
                cout << "Nombre: "
                     << usuario->getNombre() << endl;

                char confirmacion;

                cout <<
                    "¿Esta seguro de eliminarlo? (S/N): ";

                cin >> confirmacion;

                cin.ignore(
                    numeric_limits<streamsize>::max(),
                    '\n'
                );

                if (confirmacion == 'S' ||
                    confirmacion == 's')
                {
                    if (uc->eliminarUsuario(id))
                    {
                        cout <<
                            "Usuario eliminado correctamente.\n";
                    }
                    else
                    {
                        cout <<
                            "No se pudo eliminar el usuario.\n";
                    }
                }
                else
                {
                    cout << "Operacion cancelada.\n";
                }

                break;
            }

            case 4:
            {
                vector<Usuario*> usuarios =
                    uc->listarUsuarios();

                if (usuarios.empty())
                {
                    cout <<
                        "No hay usuarios registrados.\n";
                    break;
                }

                cout <<
                    "\n========== LISTA DE USUARIOS ==========\n";

                for (Usuario* usuario : usuarios)
                {
                    cout << "ID: "
                         << usuario->getId() << endl;

                    cout << "Nombre: "
                         << usuario->getNombre() << endl;

                    if (usuario->getRol() ==
                        Usuario::Rol::ADMINISTRADOR)
                    {
                        cout << "Rol: ADMINISTRADOR\n";
                    }
                    else
                    {
                        cout << "Rol: USUARIO_NORMAL\n";
                    }

                    cout <<
                        "--------------------------------------\n";
                }

                break;
            }

            case 5:
            {
                int id = leerIdUsuario();

                Usuario* usuario =
                    uc->buscarUsuarioPorId(id);

                if (usuario == nullptr)
                {
                    cout <<
                        "No existe un usuario con ese ID.\n";
                    break;
                }

                cout <<
                    "\n========== USUARIO ENCONTRADO ==========\n";

                cout << "ID: "
                     << usuario->getId() << endl;

                cout << "Nombre: "
                     << usuario->getNombre() << endl;

                if (usuario->getRol() ==
                    Usuario::Rol::ADMINISTRADOR)
                {
                    cout << "Rol: ADMINISTRADOR\n";
                }
                else
                {
                    cout << "Rol: USUARIO_NORMAL\n";
                }

                break;
            }

            case 0:
                cout <<
                    "Saliendo del menu de usuarios...\n";
                break;

            default:
                cout <<
                    "Error: opcion no valida.\n";
                break;
        }

    } while (opcion != 0);
}

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