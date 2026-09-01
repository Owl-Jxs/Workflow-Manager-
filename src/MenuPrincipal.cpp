#include "MenuPrincipal.h"

#include <iostream>
#include <limits>
#include <exception>
#include "Utilities/ValidarEntrada.h"

using namespace std;

void MenuPrincipal::crearPrimerUsuario () {
    std::cout << "No hay ningun registro de usuarios, debe de registrar el primero para utilizar el sistema" << std::endl;
    Usuario* PrimerUsuario;

    int id;
    std::string nombre;
    id = ValidarEntrada::validarCodigoNumerico ("Ingrese su identificacion", 9);
    nombre =ValidarEntrada::validarNombreCompleto ();

    PrimerUsuario = new Usuario (id, nombre, Usuario::Rol::ADMINISTRADOR);
    
    std::string contrasena = ValidarEntrada::validarContrasena ("Ingrese la contrasena: ", '*');
    PrimerUsuario->setHashContrasena (contrasena);
        
    try {
        uc->agregarUsuario  (PrimerUsuario);
    } catch (std::exception &e) {
        std::cout << e.what (); delete PrimerUsuario; PrimerUsuario = nullptr;
    }
    
    if (PrimerUsuario != nullptr) usuarioActivo = PrimerUsuario;
}

void MenuPrincipal::iniciarSesion() {
    bool seguirBuscando = true;
    Usuario* usuario = nullptr;
    
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
            std::cout << "Lo sentimos, numero de intentos maximos alcanzados. Inicie sesion mas tarde"; return;
        }
        
        usuarioActivo = usuario;
        cout << "\nInicio de sesion exitoso.\n";
        cout << "Bienvenido/a, " << usuarioActivo->getNombre() << "!\n";
    }
    
}

MenuPrincipal::MenuPrincipal ()
{
    uc = new UsuarioController ();
    tc = new TareaController ();
    ac = new AsignacionController ();
    usuarioActivo = nullptr;
    gestorHistorial = nullptr;
    auditoria = new AuditoriaDataBase ();
    menuAdmin = nullptr;
    menuNormal = nullptr;
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
            menuNormal = new MenuUsuarioNormal (tc, ac, usuarioActivo, gestorHistorial);
            
            switch (usuarioActivo->getRol ()) 
            {
                case Usuario::Rol::ADMINISTRADOR: {
                    mostrarMenuAdministrador (usuarioActivo); break;
                }
                case Usuario::Rol::USUARIO_NORMAL: {
                    menuNormal->mostrarMenuUsuarioNormal(); break;
                }   
            }

        } else {
            std::cout << "Error al abrir el sistema";
        }

    } catch (std::exception &e) {
        std::cout << e.what () << std::endl;
    }

} 

void MenuPrincipal::mostrarMenuAdministrador(Usuario* /*usuario*/)
{
    menuAdmin->mostrarMenuAdministrador ();
}

MenuPrincipal::~MenuPrincipal()
{   
    try { if (tc != nullptr) tc->guardarArchivos(); } catch(...) {}
    try { if (ac != nullptr) ac->guardarAsignaciones(); } catch(...) {}
    delete uc;
    delete tc;
    delete ac;
    delete auditoria;
    if (gestorHistorial != nullptr) delete gestorHistorial;
    if (menuAdmin != nullptr ) delete menuAdmin;
    if (menuNormal != nullptr ) delete menuNormal;
}
