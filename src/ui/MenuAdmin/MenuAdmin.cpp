#include "MenuAdmin.h"


MenuAdmin::MenuAdmin (UsuarioController* _uc, TareaController* _tc, AsignacionController* _ac,Usuario* uA, GestorHistorial* gH){
    uc = _uc;
    tc = _tc;
    ac = _ac;
    usuarioActivo = uA;
    gestorHistorial = gH;
    procesosUsuarios = new ProcesosUsuario (uc, usuarioActivo, gestorHistorial);
    procesosTareas = new ProcesosTarea (uc, tc, ac, usuarioActivo, gestorHistorial);
}

MenuAdmin::~MenuAdmin () {
    delete procesosUsuarios; delete procesosTareas;
}



void MenuAdmin::mostrarMenuAdministrador() {
    int opcion;
    do
    {
        std::cout << "\n==============================\n"
         << "      MENU ADMINISTRADOR     \n"
         << "==============================\n"
         << "1. Desplegar menu de gestion de usuarios\n"
         << "2. Desplegar menu de gestion de tareas\n"
         << "0. Salir\n";

        opcion = ValidarEntrada::validarEntradaRango ("Seleccione una opcion: ",0,2);

        switch (opcion)
        {
            case 1:
            {
               mostrarMenuGestionUsuarios (); break;
            }   

            case 2:
            {
                mostrarMenuGestionTareas (); break;
            }
            case 0:
                std::cout << "Saliendo del menu de usuarios...\n"; break;

            default:
                std::cout << "Error: opcion no valida.\n"; break;
        }
        std::system ("cls");
    } while (opcion != 0);
}


void MenuAdmin::mostrarMenuGestionUsuarios (){
    int opcion;
    do
    {
        std::cout << "\n==============================\n"
         << "      MENU DE GESTION DE USUARIOS     \n"
         << "==============================\n"
         << "1. Agregar usuario\n"
         << "2. Actualizar usuario\n"
         << "3. Eliminar usuario\n"
         << "4. Listar usuarios\n"
         << "5. Buscar usuario por ID\n"
         << "0. volver al menu Administrador\n";

        opcion = ValidarEntrada::validarEntradaRango ("Seleccione una opcion: ",0,5);

        switch (opcion)
        {
            case 1:
            {
                procesosUsuarios-> agregarUsuario (); break;
            }   

            case 2:
            {
                procesosUsuarios-> actualizarUsuario (); break;
            }

            case 3:
            {
               procesosUsuarios-> eliminarUsuario (); break;
            }

            case 4:
            {
               procesosUsuarios-> mostrarLista (); break;
            }

            case 5:
            {
               procesosUsuarios-> mostrarUsuarioPorId (); break;
            }

            case 0:
                std::cout << "Saliendo del menu de usuarios...\n"; break;

            default:
                std::cout << "Error: opcion no valida.\n"; break;
        }
        std::system ("cls");
    } while (opcion != 0);   
}


void MenuAdmin::mostrarMenuGestionTareas (){
     int opcion;
    do
    {
        std::cout << "\n==============================\n"
         << "      MENU DE GESTION DE TAREAS     \n"
         << "==============================\n"
         << "1. Agregar tarea\n"
         << "2. Actualizar tarea\n"
         << "3. Eliminar tarea\n"
         << "4. Mostrar el tablero kanban\n"
         << "5. Mostrar tareas ordenadamente\n"
         << "6. Mostrar tarea por ID\n"
         << "7. Mostrar subTareas de una tarea\n"
         << "0. volver al menu Administrador\n";

        opcion = ValidarEntrada::validarEntradaRango ("Seleccione una opcion: ",0,7);

        switch (opcion)
        {
            case 1:
            {
                procesosTareas->agregarTarea (); break;
            }   

            case 2:
            {
                procesosTareas->ActualizarTarea (); break;
            }

            case 3:
            {
               procesosTareas->eliminarTarea (); break;
            }

            case 4:
            {
               procesosTareas->mostrarTableroKanban(); break;
            }
            case 5:
            {
                procesosTareas->ordenarLista (); break;
            }
            case 6:
            {
                procesosTareas->mostrarTarea (); break;
            }
            case 7:
            {
                procesosTareas->listarArbolTarea (); break;
            }

            case 0:
                std::cout << "Saliendo del menu de usuarios...\n"; break;

            default:
                std::cout << "Error: opcion no valida.\n"; break;
        }
        std::system ("pause"); std::system ("cls");
    } while (opcion != 0);       
}
