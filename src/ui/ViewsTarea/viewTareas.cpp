#include "ViewTareas.h"
#include <iostream>
#include <vector>

using namespace std;

ViewTareas::ViewTareas() {
}

//MOSTRAR INFORMACIÓN DE UNA TAREA
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
    } else {
        cout << "NORMAL";
    }

    cout << endl;

    cout << "Estado: " << tarea->getEstado() << endl;

    cout << "Cantidad de subtareas: " << tarea->getCantidadSubTareas() << endl;

    cout << "---------------------------------------------\n";
}

//MOSTRAR SUBTAREAS
void ViewTareas::mostrarSubTareas(Tarea* tarea) {
    if (tarea == nullptr) {
        return;
    }

    Tarea* subTarea = tarea->getPrimerSubTarea();

    if (subTarea == nullptr) {
        cout << "\nEsta tarea no tiene subtareas.\n";
        return;
    }

    cout << "\n";
    cout << "============== SUBTAREAS ==============\n";

    while (subTarea != nullptr) {
        cout << "\n";
        cout << "ID: " << subTarea->getIdTarea() << endl;

        cout << "Descripcion: " << subTarea->getDescripcionTarea() << endl;

        cout << "Prioridad: ";

        if (subTarea->getPrioridad()) {
            cout << "URGENTE";
        } else {
            cout << "NORMAL";
        }

        cout << endl;

        cout << "Estado: " << subTarea->getEstado() << endl;

        cout << "Cantidad de subtareas: " << subTarea->getCantidadSubTareas() << endl;

        subTarea = subTarea->getSiguienteSubTarea();
    }

    cout << "\n========================================\n";
}

//MOSTRAR TABLERO KANBAN
void ViewTareas::mostrarTableroKanban(ColaFIFO* listaUrgente, ColaFIFO* listaRegular) {

    //VECTORES PARA CLASIFICAR LAS TAREAS
    vector<Tarea*> porHacer;
    vector<Tarea*> enProceso;
    vector<Tarea*> completadas;

    //RECORRER LISTA URGENTE
    NodoTarea* actual = listaUrgente->getFrente();

    while (actual != nullptr) {
        Tarea* tarea = actual->datos;

        if (tarea->getEstado() == "POR HACER") {
            porHacer.push_back(tarea);
        } else if (tarea->getEstado() == "EN PROCESO") {
            enProceso.push_back(tarea);
        } else if (tarea->getEstado() == "COMPLETADA") {
            completadas.push_back(tarea);
        }

        actual = actual->siguiente;
    }

    //RECORRER LISTA REGULAR
    actual = listaRegular->getFrente();

    while (actual != nullptr) {
        Tarea* tarea = actual->datos;

        if (tarea->getEstado() == "POR HACER") {
            porHacer.push_back(tarea);
        } else if (tarea->getEstado() == "EN PROCESO") {
            enProceso.push_back(tarea);
        } else if (tarea->getEstado() == "COMPLETADA") {
            completadas.push_back(tarea);
        }

        actual = actual->siguiente;
    }

    cout << "\n";
    cout << "====================================================\n";
    cout << "                  TABLERO KANBAN\n";
    cout << "====================================================\n";

    cout << "\n";
    cout << "=================== POR HACER ======================\n";

    if (porHacer.empty()) {
        cout << "\nNo hay tareas por hacer.\n";

    } else {
        for (Tarea* tarea : porHacer) {
            mostrarTarea(tarea);
        }
    }

    cout << "\n";
    cout << "=================== EN PROCESO ====================\n";

    if (enProceso.empty()) {
        cout << "\nNo hay tareas en proceso.\n";
    } else {
        for (Tarea* tarea : enProceso) {
            mostrarTarea(tarea);
        }
    }

    cout << "\n";
    cout << "=================== COMPLETADAS ===================\n";

    if (completadas.empty()) {
        cout << "\nNo hay tareas completadas.\n";
    } else {
        for (Tarea* tarea : completadas) {
            mostrarTarea(tarea);
        }
    }

    cout << "\n";
    cout << "====================================================\n";
}

/*
SE PUEDEN UTILIZAR PARA MENU
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
*/
*/
