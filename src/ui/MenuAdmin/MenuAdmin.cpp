#include "MenuAdmin.h"

MenuAdmin::MenuAdmin (UsuarioController* _uc, TareaController* _tc, AsignacionController* _ac,Usuario* uA, GestorHistorial* gH){
    uc = _uc;
    tc = _tc;
    ac = _ac;
    usuarioActivo = uA;
    gestorHistorial = gH;
    procesosUsuarios = new ProcesosUsuario (uc, usuarioActivo, gestorHistorial);
    procesosTareas = new ProcesosTarea (uc, tc, ac, usuarioActivo, gestorHistorial);
    procesosUndoRedo = new Procesos_Undo_Redo (gestorHistorial);
}

MenuAdmin::~MenuAdmin () {
    delete procesosUsuarios; delete procesosTareas; delete procesosUndoRedo;
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
         << "3. Deshacer\n"
         << "4. Rehacer\n" 
         << "0. Salir\n";

        opcion = ValidarEntrada::validarEntradaRango ("Seleccione una opcion: ",0,4);

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

             case 3:
            {
                procesosUndoRedo->deshacer (); break;
            }
             case 4:
            {
                procesosUndoRedo->rehacer (); break;
            }

            case 0:
                std::cout << "Saliendo del menu de Admin...\n"; break;

            default:
                std::cout << "Error: opcion no valida.\n"; break;
        }
        std::system ("pause"); std::system ("cls");
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
         << "2. Agregar SubTarea\n" //agregar
         << "3. Actualizar tarea\n"
         << "4. Eliminar tarea/subTarea\n"
         << "5. Asignar Tarea\n"
         << "6. Validar Tarea para completarse\n" //agregar
         << "7. Mostrar el tablero kanban\n"
         << "8. Mostrar tareas ordenadamente\n"
         << "9. Mostrar tarea por ID\n"
         << "0. volver al menu Administrador\n";

        opcion = ValidarEntrada::validarEntradaRango ("Seleccione una opcion: ",0,9);

        switch (opcion)
        {
            case 1: //agregar Tarea
            {
                procesosTareas->agregarTarea (); break;
            }   
            case 2: //agregar subTarea
            {
                procesosTareas->agregarSubTarea (); break;
            }
            case 3: //actualizar tarea
            {
                procesosTareas->ActualizarTarea (); break;
            }

            case 4: //eliminar tarea
            {
               procesosTareas->eliminarTarea (); break;
            }

            case 5: //5. Asignar Tarea
            {
               procesosTareas->asignarResponsable(); break;
            }
            case 6://Validar Tarea para completarse
            {
                procesosTareas->verificarTareaEnColaRevision (); break;
            }
            case 7: //7. Mostrar el tablero kanban
            {
                procesosTareas->mostrarTableroKanban (); break;
            }
            case 8://Mostrar tareas ordenadamente"
            {
                procesosTareas->ordenarLista (); break;
            }
            case 9://Mostrar tarea por ID
            {
                procesosTareas->mostrarTareaPorId (); break;
            }
            case 0:
                std::cout << "Saliendo del menu de usuarios...\n"; break;

            default:
                std::cout << "Error: opcion no valida.\n"; break;
        }
        std::system ("pause"); std::system ("cls");
    } while (opcion != 0);       
}
