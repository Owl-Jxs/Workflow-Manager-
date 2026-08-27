#include "MenuPrincipal.h"

#include <iostream>
#include <limits>
#include <exception>
#include "Utilities/ValidarEntrada.h"

using namespace std;

void MenuPrincipal::crearPrimerUsuario () {
    std::cout << "No hay ningun registro de usarios,  debe de registrar el primero para utilizar el sistema" << std::endl;
    Usuario* PrimerUsuario;

    int id;
    std::string nombre;
    int opcionRol;
    Usuario::Rol rol;
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
            
    };
    PrimerUsuario = new Usuario (id, nombre, rol);
    
    std::string contrasena = ValidarEntrada::validarContrasena ("Ingrese la contrasena: ", '*');
    PrimerUsuario->setHashContrasena (contrasena);
        
    try {
        uc->agregarUsuario  (PrimerUsuario);
    } catch (std::exception &e) {
        std::cout << e.what (); delete PrimerUsuario;
    }
    
    if (PrimerUsuario != nullptr) usuarioActivo = PrimerUsuario;
}

void MenuPrincipal::iniciarSesion() {
    bool seguirBuscando = true;
    Usuario* usuario;
    
    do { //buscamos el usuario

        int idUsuario = ValidarEntrada::validarCodigoNumerico  ("Ingrese su ID de usuario", 9);
        usuario = uc->buscarUsuarioPorId(idUsuario);

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
    if (usuario != nullptr){
        do {
            std::string contrasena = ValidarEntrada::validarContrasena("Ingrese su contrasena: ", '*');
            hash<string> hasher;

            if (hasher(contrasena) != usuario->getHashContrasena()) {
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
        
        usuarioActivo = usuario;
        gestorHistorial->setIdUsuarioSesion(usuarioActivo->getId());
        cout << "\nInicio de sesion exitoso.\n";
        cout << "Bienvenido/a, " << usuarioActivo->getNombre() << "!\n";
    }
    
}

MenuPrincipal::MenuPrincipal ()
{
    uc = new UsuarioController ();
    tc = new TareaController ();
    ac = new AsignacionController ();
    gestorHistorial = nullptr;
    auditoria = new AuditoriaDataBase ();
    menuAdmin = nullptr;
    //menuRegular = nullptr;
    try {
        uc->cargarUsuarios();
    }catch (const exception& e) {
        cout << "Advertencia: no se pudieron cargar los usuarios.\n";
        cout << e.what() << endl;
    }

    try{
        tc->cargarArchivos();
    }catch (const exception& e) {
        cout << "Advertencia: no se pudieron cargar las tareas.\n";
        cout << e.what() << endl;
    }

    try {
        ac->cargarAsignaciones();
    } catch (const exception& e) {
        cout << "Advertencia: no se pudieron cargar las asignaciones.\n";
        cout << e.what() << endl;
    }
}

void MenuPrincipal::ejecutar () {
    try {
        std::cout << "\n================================\n";
        std::cout << "       WORKFLOW MANAGER\n";
        std::cout << "================================\n\n";

        if ( (uc->listarUsuarios () ).empty () ) {
            crearPrimerUsuario ();
        } else {
            iniciarSesion ();
        }

        if (usuarioActivo != nullptr ){

        //inicializamos los atributos restantes que ocupan a usuario activo
            gestorHistorial  = new GestorHistorial (auditoria, usuarioActivo->getId ());
            menuAdmin = new  MenuAdmin (uc, tc, ac, usuarioActivo, gestorHistorial);
            //menu normal
            
            switch (usuarioActivo->getRol ()) 
            {
                case Usuario::Rol::ADMINISTRADOR: {
                    mostrarMenuAdministrador (usuarioActivo); break;
                }
                case Usuario::Rol::USUARIO_NORMAL: {
                    //mostrarMenuUsuarioNormal (usuarioActivo); break;
                }   
            }

        } else {
            std::cout << "Error al abrir el sistema";
        }

    } catch (std::exception &e) {
        std::cout << e.what ();
    }

}

        

void MenuPrincipal::mostrarMenuAdministrador(Usuario* usuario)
{
    menuAdmin->mostrarMenuAdministrador ();
}
/*
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

*/
MenuPrincipal::~MenuPrincipal()
{   
    delete uc;
    delete tc;
    delete ac;
    delete auditoria;
    delete gestorHistorial;
    if (menuAdmin != nullptr ) delete menuAdmin;
}
