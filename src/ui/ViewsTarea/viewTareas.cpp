#include "ViewTareas.h"
#include <iostream>
#include <string>

using namespace std;
/*
ViewTareas::ViewTareas(TareaController* tc) {
    this->tc = tc;
}

//BUSCAR TAREA POR ID

// == ============ ================ =============== ELIMINAR::Ya esta en el controlador esta funcionalidad =========== =========== ====== = = == =

Tarea* ViewTareas::buscarTarea(int idTarea) {
    if (tc == nullptr) {
        return nullptr;
    }

    ColaFIFO* listaRegular = tc->getListaTareasRegulares();

    if (listaRegular != nullptr) {
        NodoTarea* actual = listaRegular->getFrente();
        while (actual != nullptr) {
            Tarea* tarea = actual->getDatos();
            if (tarea != nullptr && tarea->getIdTarea() == idTarea) {
                return tarea;
            }
            actual = actual->getSiguiente();
        }
    }

    ColaFIFO* listaUrgente = tc->getListaTareasUrgentes();

    if (listaUrgente != nullptr) {
        nodoTarea* actual = listaUrgente->getFrente();

        while (actual != nullptr) {
            Tarea* tarea = actual->getDatos();
            if (tarea != nullptr && tarea->getIdTarea() == idTarea) {
                return tarea;
            }
            actual = actual->getSiguiente();
        }
    }

    return nullptr;
}

void ViewTareas::mostrarTarea(Tarea* tarea) {
    if (tarea == nullptr) {
        return;
    }

    cout << "\n";
    cout << "---------------------------------------------\n";

    cout << "ID: " << tarea->getIdTarea() << endl;

    cout << "Descripcion: " << tarea->getDescripcionTarea() << endl;

    cout << "Prioridad: ";

    if (tarea->getPrioridad()) {
        cout << "URGENTE";
    }
    else {
        cout << "NORMAL";
    }

    cout << endl;

    cout << "Estado: " << tarea->getEstado() << endl;

    cout << "Cantidad de subtareas: " << tarea->getCantidadSubTareas() << endl;

    cout << "---------------------------------------------\n";

    if (tarea->getPrimerSubTarea() != nullptr) {
        cout << "Subtareas:\n";
        mostrarSubTareas(tarea, 1);
    }
}

void ViewTareas::mostrarSubTareas(Tarea* tarea, int nivel) {
    if (tarea == nullptr) {
        return;
    }

    Tarea* subTarea = tarea->getPrimerSubTarea();

    while (subTarea != nullptr) {
        for (int i = 0; i < nivel; i++) {
            cout << "    ";
        }

        cout << "- ID: " << subTarea->getIdTarea() << " | Descripcion: " << subTarea->getDescripcionTarea() << " | Estado: " << subTarea->getEstado() << endl;

        if (subTarea->getPrimerSubTarea() != nullptr) {
            mostrarSubTareas(subTarea, nivel + 1);
        }
        subTarea = subTarea->getSiguienteSubTarea();
    }
}

void ViewTareas::crearTarea(){
    if (tc == nullptr) {
        cout << "Error: el TareaController no esta disponible.\n";
        return;
    }

    int id;
    string descripcion;
    int prioridad;

    cout << "\n";
    cout << "=============================================\n";
    cout << "              CREAR TAREA\n";
    cout << "=============================================\n";

    cout << "Ingrese el ID de la tarea: ";
    cin >> id;

    cin.ignore();

    if (buscarTarea(id) != nullptr) {
        cout << "Error: ya existe una tarea con ese ID.\n";
        return;
    }

    cout << "Ingrese la descripcion: ";
    getline(cin, descripcion);

    if (descripcion.empty()) {
        cout << "Error: la descripcion no puede estar vacia.\n";
        return;
    }

    cout << "\nSeleccione la prioridad:\n";
    cout << "1. NORMAL\n";
    cout << "2. URGENTE\n";
    cout << "Opcion: ";
    cin >> prioridad;

    if (prioridad != 1 && prioridad != 2) {
        cout << "Error: prioridad invalida.\n";
        return;
    }

    bool urgente = (prioridad == 2);

    string estado = "POR HACER";

    try {
        Tarea* nuevaTarea = new Tarea(id, descripcion, urgente, estado);

        tc->agregarTarea(nuevaTarea, urgente);

        cout << "\nTarea creada correctamente.\n";
        cout << "ID: " << id << endl;
        cout << "Descripcion: " << descripcion << endl;

        if (urgente) {
            cout << "Prioridad: URGENTE\n";
        } else {
            cout << "Prioridad: NORMAL\n";
        }

        cout << "Estado: POR HACER\n";
    }
    catch (const exception& e) {
        cout << "\nError al crear la tarea: " << e.what() << endl;
    }
}

void ViewTareas::crearSubTarea() {
    if (tc == nullptr) {
        cout << "Error: el TareaController no esta disponible.\n";
        return;
    }

    int id;
    int idPadre;
    string descripcion;
    int prioridad;

    cout << "\n";
    cout << "=============================================\n";
    cout << "             CREAR SUBTAREA\n";
    cout << "=============================================\n";

    cout << "Ingrese el ID de la tarea padre: ";
    cin >> idPadre;

    Tarea* padre = buscarTarea(idPadre);

    if (padre == nullptr) {
        cout << "Error: no existe una tarea con ese ID.\n";
        return;
    }

    cout << "Ingrese el ID de la subtarea: ";
    cin >> id;

    if (buscarTarea(id) != nullptr) {
        cout << "Error: ya existe una tarea con ese ID.\n";
        return;
    }

    cin.ignore();

    cout << "Ingrese la descripcion: ";
    getline(cin, descripcion);

    if (descripcion.empty()) {
        cout << "Error: la descripcion no puede estar vacia.\n";
        return;
    }

    cout << "\nSeleccione la prioridad:\n";
    cout << "1. NORMAL\n";
    cout << "2. URGENTE\n";
    cout << "Opcion: ";
    cin >> prioridad;

    if (prioridad != 1 && prioridad != 2) {
        cout << "Error: prioridad invalida.\n";
        return;
    }

    bool urgente = (prioridad == 2);

    string estado = "POR HACER";

    try {
        Tarea* nuevaSubTarea = new Tarea(id, descripcion, urgente, estado);

        tc->agregarSubTarea(nuevaSubTarea, idPadre, urgente);

        cout << "\nSubtarea creada correctamente.\n";
        cout << "ID: " << id << endl;
        cout << "Tarea padre: " << idPadre << endl;
        cout << "Descripcion: " << descripcion << endl;

        if (urgente) {
            cout << "Prioridad: URGENTE\n";
        } else {
            cout << "Prioridad: NORMAL\n";
        }
        cout << "Estado: POR HACER\n";
    }
    catch (const exception& e) {
        cout << "\nError al crear la subtarea: " << e.what() << endl;
    }
}

void ViewTareas::mostrarTableroKanban() {
    if (tc == nullptr) {
        cout << "Error: el TareaController no esta disponible.\n";
        return;
    }

    ColaFIFO* listaRegular = tc->getListaTareasRegulares();

    ColaFIFO* listaUrgente = tc->getListaTareasUrgentes();

    if (listaRegular == nullptr || listaUrgente == nullptr) {
        cout << "Error: no se pudieron obtener las listas de tareas.\n";
        return;
    }

    cout << "\n";
    cout << "====================================================\n";
    cout << "                 TABLERO KANBAN\n";
    cout << "====================================================\n";

    cout << "\n";
    cout << "=================== POR HACER =====================\n";

    bool hayTareas = false;

    nodoTarea* actual = listaRegular->getFrente();

    while (actual != nullptr) {
        Tarea* tarea = actual->getDatos();
        if (tarea != nullptr && tarea->getEstado() == "POR HACER") {
            mostrarTarea(tarea);
            hayTareas = true;
        }
        actual = actual->getSiguiente();
    }

    actual = listaUrgente->getFrente();

    while (actual != nullptr) {
        Tarea* tarea = actual->getDatos();
        if (tarea != nullptr && tarea->getEstado() == "POR HACER") {
            mostrarTarea(tarea);
            hayTareas = true;
        }
        actual = actual->getSiguiente();
    }

    if (!hayTareas) {
        cout << "No hay tareas por hacer.\n";
    }

    cout << "\n";
    cout << "=================== EN PROCESO ====================\n";

    hayTareas = false;

    actual = listaRegular->getFrente();

    while (actual != nullptr) {
        Tarea* tarea = actual->getDatos();
        if (tarea != nullptr && tarea->getEstado() == "EN PROCESO") {
            mostrarTarea(tarea);
            hayTareas = true;
        }
        actual = actual->getSiguiente();
    }

    actual = listaUrgente->getFrente();

    while (actual != nullptr) {
        Tarea* tarea = actual->getDatos();
        if (tarea != nullptr && tarea->getEstado() == "EN PROCESO") {
            mostrarTarea(tarea);
            hayTareas = true;
        }
        actual = actual->getSiguiente();
    }

    if (!hayTareas) {
        cout << "No hay tareas en proceso.\n";
    }

    cout << "\n";
    cout << "=================== COMPLETADAS ===================\n";

    hayTareas = false;

    actual = listaRegular->getFrente();

    while (actual != nullptr) {
        Tarea* tarea = actual->getDatos();
        if (tarea != nullptr && tarea->getEstado() == "COMPLETADA") {
            mostrarTarea(tarea);
            hayTareas = true;
        }
        actual = actual->getSiguiente();
    }

    actual = listaUrgente->getFrente();

    while (actual != nullptr) {
        Tarea* tarea = actual->getDatos();
        if (tarea != nullptr && tarea->getEstado() == "COMPLETADA") {
            mostrarTarea(tarea);
            hayTareas = true;
        }
        actual = actual->getSiguiente();
    }

    if (!hayTareas) {
        cout << "No hay tareas completadas.\n";
    }

    cout << "\n";
    cout << "====================================================\n";
}
*/