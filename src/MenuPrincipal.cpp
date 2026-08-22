#include "MenuPrincipal.h"

#include <iostream>
#include <limits>
#include <exception>
#include "Utilities/ValidarEntrada.h"

using namespace std;

void MenuPrincipal::crearPrimerUsuario () {
    std::cout << "No hay ningun registro de usarios,  debe de registrar el primero para utilizar el sistema" << std::endl;
    Usuario* PrimerUsuario = uv->leerNuevoUsuario ();
        
    try {
        uc->agregarUsuario  (PrimerUsuario);
    } catch (std::exception &e) {
        std::cout << e.what (); delete PrimerUsuario;
    }
        usuarioActivo = PrimerUsuario;
}

void MenuPrincipal::iniciarSesion()
{
    bool seguirBuscando = true;
    
    do { //buscamos el uusuario
        int idUsuario = ValidarEntrada::validarCodigoNumerico  ("Ingrese su ID de usuario", 9);
        Usuario* usuario = uc->buscarUsuarioPorId(idUsuario);

        if (usuario == nullptr){
                std::cout << "\nNo existe un usuario con ese ID.\n";
                std::cout << "Desea intentar nuevamente.\n";
                seguirBuscando = ValidarEntrada::respuestas_Si_O_No ("Si", "No, Salir");
            } else {
                seguirBuscando = false;
            }
    } while (seguirBuscando);

    //validamos su contrasena
    bool contraValida = false;
    int numeroIntentos = 8;

    do {
        std::string contrasena = ValidarEntrada::validarContrasena("Ingrese su contrasena: ", '*');
        hash<string> hasher;

        if (hasher(contrasena) != usuarioActivo->getHashContrasena())
        {
            numeroIntentos--;
            std::cout << "Contrasena incorrecta. Le quedan " << numeroIntentos << " intentos"; std::system ("pause"); std::system ("cls"); 
        } else {
            contraValida = true;
        }

    } while (!contraValida &&  numeroIntentos != 0);

   if (numeroIntentos == 0) {
    usuarioActivo = nullptr;
    std::cout << "Lo sentimos, numero de intentos maximos alcanzados. Ingrese sesion mas tarde"; return;
   }

   gestorHistorial->setIdUsuarioSesion(usuarioActivo->getId());
    cout << "\nInicio de sesion exitoso.\n";
    cout << "Bienvenido/a, " << usuarioActivo->getNombre() << "!\n";
}


MenuPrincipal::MenuPrincipal ()
{
    uc = new UsuarioController ();
    tc = new TareaController ();
    ac = new AsignacionController ();
    gestorHistorial = new GestorHistorial ();

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

void MenuPrincipal::ejecutar () {
    std::cout << "\n================================\n";
    std::cout << "       WORKFLOW MANAGER\n";
    std::cout << "================================\n\n";

    if ( (uc->listarUsuarios () ).empty () ) {
         crearPrimerUsuario ();
    } else {
        iniciarSesion ();
    }

    if (usuarioActivo != nullptr ){

        uv = new UsuarioView (uc, usuarioActivo, gestorHistorial);
        switch (usuarioActivo->getRol ()) 
        {
            case Usuario::Rol::ADMINISTRADOR: {
                mostrarMenuAdministrador (usuarioActivo); break;
            }
            case Usuario::Rol::USUARIO_NORMAL: {
                mostrarMenuUsuarioNormal (usuarioActivo); break;
            }   
        }
    } else {
        std::cout << "Error al abrir el sistema";
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
        opcion = ValidarEntrada::validarEntradaRango("Seleccione una opcion: ", 0, 4);

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
        opcion = ValidarEntrada::validarEntradaRango ("Seleccione una opcion: ", 0, 2);

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
    delete uc;
    delete tc;
    delete ac;
    delete gestorHistorial;
    delete uv;
    delete tv;
}
