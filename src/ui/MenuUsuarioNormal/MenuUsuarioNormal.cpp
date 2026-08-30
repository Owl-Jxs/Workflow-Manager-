#include "MenuUsuarioNormal.h"
#include "../../Utilities/ValidarEntrada.h"

#include <iostream>

MenuUsuarioNormal::MenuUsuarioNormal(TareaController* _tc, AsignacionController* _ac, Usuario* _usuario, GestorHistorial* _gH) {
    this->tc = _tc;
    this->ac = _ac;
    this->usuarioActivo = _usuario;
    this->gestorHistorial = _gH;
    this->procesosTareas = new ProcesosTareaNormal(tc, ac, usuarioActivo, gestorHistorial);
}

MenuUsuarioNormal::~MenuUsuarioNormal() {
    delete procesosTareas;
}

void MenuUsuarioNormal::mostrarMenuUsuarioNormal() {
    int opcion;
    do {
        std::cout << "\n==============================\n";
        std::cout << "    MENU USUARIO NORMAL\n";
        std::cout << "==============================\n";
        std::cout << "Bienvenido/a, " << usuarioActivo->getNombre() << "!\n\n";
        std::cout << "1. Ver mis tareas asignadas\n";
        std::cout << "2. Ver detalles de tarea por ID\n";
        std::cout << "3. Enviar tarea a revision\n";
        std::cout << "4. Ver tablero kanban\n";
        std::cout << "0. Cerrar sesion\n";
        opcion = ValidarEntrada::validarEntradaRango("Seleccione una opcion: ", 0, 4);

        switch (opcion) {
            case 1:
                procesosTareas->verMisTareasAsignadas();
                break;
            case 2:
                procesosTareas->verDetallesTarea();
                break;
            case 3:
                procesosTareas->enviarTareaARevision();
                break;
            case 4:
                procesosTareas->verTableroKanban();
                break;
            case 0:
                std::cout << "Cerrando sesion...\n";
                break;
            default:
                std::cout << "Opcion invalida.\n";
                break;
        }
    } while (opcion != 0);
}
