#include "ProcesosTareaNormal.h"

ProcesosTareaNormal::ProcesosTareaNormal(TareaController* _tc, AsignacionController* _ac, Usuario* _usuario, GestorHistorial* _gH) {
    this->tc = _tc;
    this->ac = _ac;
    this->usuarioActivo = _usuario;
    this->gestorHistorial = _gH;
}

void ProcesosTareaNormal::mostrarInformacionTarea(Tarea* tarea, bool mostrarSubTareas, int nivel) {
    if (tarea == nullptr) return;

    std::string sangria(nivel * 3, ' ');

    std::cout << sangria << "|-- ID: " << tarea->getIdTarea()
              << " | " << tarea->getDescripcionTarea()
              << " | " << (tarea->getPrioridad() ? "Urgente" : "Regular")
              << " | " << tarea->getEstado() << "\n";

    if (!mostrarSubTareas) return;

    Tarea* subTarea = tarea->getPrimerSubTarea();
    while (subTarea != nullptr) {
        mostrarInformacionTarea(subTarea, mostrarSubTareas, nivel + 1);
        subTarea = subTarea->getSiguienteSubTarea();
    }
}

void ProcesosTareaNormal::verMisTareasAsignadas() {
    std::vector<int> ids = {usuarioActivo->getId()};
    std::vector<std::pair<int, int>> asignaciones = ac->getAsignacionesTareasPorUsuario(ids);

    if (asignaciones.empty()) {
        std::cout << "\nNo tiene tareas asignadas.\n";
        return;
    }

    std::cout << "\n========== MIS TAREAS ASIGNADAS ==========\n";

    for (std::pair<int, int> par : asignaciones) {
        int idTarea = par.first;
        Tarea* tarea = tc->buscarTareaPorHacer(idTarea);
        if (tarea == nullptr) tarea = tc->buscarTareaEnProceso(idTarea);
        if (tarea == nullptr) tarea = tc->buscarTareaEnRevision(idTarea);
        if (tarea == nullptr) tarea = tc->buscarTareaCompletada(idTarea);

        if (tarea != nullptr) {
            mostrarInformacionTarea(tarea, false, 1);
            std::cout << "--------------------------------------------------------------------------------\n";
        }
    }
}

void ProcesosTareaNormal::verDetallesTarea() {
    int idTarea = ValidarEntrada::validarEntradaRango("Ingrese el id de la tarea", 0, tc->getUltimoId());

    Tarea* tarea = tc->buscarTareaPorHacer(idTarea);
    if (tarea == nullptr) tarea = tc->buscarTareaEnProceso(idTarea);
    if (tarea == nullptr) tarea = tc->buscarTareaEnRevision(idTarea);
    if (tarea == nullptr) tarea = tc->buscarTareaCompletada(idTarea);

    if (tarea == nullptr) {
        std::cout << "La tarea buscada no existe.\n";
        return;
    }

    mostrarInformacionTarea(tarea, true, 1);
    std::system("pause");
    std::system("cls");
}

void ProcesosTareaNormal::enviarTareaARevision() {
    int idTarea = ValidarEntrada::validarEntradaRango("Ingrese el id de la tarea a enviar a revision", 0, tc->getUltimoId());

    Tarea* tarea = tc->buscarTareaEnProceso(idTarea);
    if (tarea == nullptr) {
        std::cout << "La tarea no existe o no esta en estado EN PROCESO.\n";
        return;
    }

    if (!ac->buscarAsignacion(idTarea, usuarioActivo->getId())) {
        std::cout << "Esta tarea no esta asignada a su usuario.\n";
        return;
    }

    std::cout << "\nTarea a enviar a revision:\n";
    mostrarInformacionTarea(tarea, false, 1);

    bool confirmar = ValidarEntrada::respuestas_Si_O_No("Si, enviar", "No, cancelar");
    if (!confirmar) {
        std::cout << "Operacion cancelada.\n";
        return;
    }

    try {
        MandarARevisionComando* comando = new MandarARevisionComando(tc, idTarea);
        gestorHistorial->ejecutarComando(comando);
        std::cout << "Tarea enviada a revision correctamente.\n";
    } catch (std::exception& e) {
        std::cout << "Error al enviar a revision: " << e.what() << "\n";
    }
}

void ProcesosTareaNormal::verTableroKanban() {
    std::vector<Tarea*> listaUrgente = tc->listarTareasUrgentes();
    std::vector<Tarea*> listaRegulares = tc->listarTareasRegulares();
    std::vector<Tarea*> enProceso = tc->listarTareasEnProceso();
    std::vector<Tarea*> enRevision = tc->listarTareasEnRevision();
    std::vector<Tarea*> completadas = tc->listarTareasCompletadas();

    if (listaUrgente.empty() && listaRegulares.empty() && enProceso.empty() && enRevision.empty() && completadas.empty()) {
        std::cout << "No hay registros de tareas.\n";
        return;
    }

    std::cout << "====================================================\n"
              << "                  TABLERO KANBAN\n"
              << "====================================================\n"
              << "\n"
              << "=================== POR HACER ======================\n";

    if (listaUrgente.empty() && listaRegulares.empty()) {
        std::cout << "\nNo hay tareas por hacer.\n";
    } else {
        for (Tarea* tarea : listaUrgente) { mostrarInformacionTarea(tarea, false, 1); }
        for (Tarea* tarea : listaRegulares) { mostrarInformacionTarea(tarea, false, 1); }
    }

    std::cout << "\n=================== EN PROCESO ====================\n";
    if (enProceso.empty()) {
        std::cout << "\nNo hay tareas en proceso.\n";
    } else {
        for (Tarea* tarea : enProceso) { mostrarInformacionTarea(tarea, false, 1); }
    }

    std::cout << "\n=================== EN REVISION ===================\n";
    if (enRevision.empty()) {
        std::cout << "\nNo hay tareas en revision.\n";
    } else {
        for (Tarea* tarea : enRevision) { mostrarInformacionTarea(tarea, false, 1); }
    }

    std::cout << "\n=================== COMPLETADAS ===================\n";
    if (completadas.empty()) {
        std::cout << "\nNo hay tareas completadas.\n";
    } else {
        for (Tarea* tarea : completadas) { mostrarInformacionTarea(tarea, false, 1); }
    }

    std::cout << "\n====================================================\n";
}
