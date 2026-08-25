#include "ProcesosTarea.h"

void ProcesosTarea::asignarResponsable (int idTarea) {
    bool seguirBuscando = true, usuarioEncontrado = false;
    Usuario* responsable;

    do {
        int idResponsable = ValidarEntrada::validarCodigoNumerico ("Ingrese el id del responsable de esta tarea",  9);
        responsable = uc->buscarUsuarioPorId (idResponsable);

        if (responsable == nullptr ) {
            std::cout << " usuario no encontrado, desea seguir buscando? " << std::endl;
            seguirBuscando  = ValidarEntrada::respuestas_Si_O_No ("Si", "No");
        } else {
            usuarioEncontrado = true;
        }

    } while (seguirBuscando && !usuarioEncontrado);

    if (!seguirBuscando) throw std::runtime_error ("Tarea cancelada");

    AgregarAsignacionComando* nuevaAsignacion;
    try {
        nuevaAsignacion = new AgregarAsignacionComando (ac,idTarea, responsable->getId ());
        gestorHistorial->ejecutarComando (nuevaAsignacion);
    } catch (std::exception &e) {
        delete nuevaAsignacion; throw;
    }
}

Tarea* ProcesosTarea::leerNuevaTarea () {
   if (tc == nullptr) {
        std::cout << "Error: el TareaController no esta disponible.\n";
        return;
    }
    std::string descripcion;
    int prioridad;

    std::cout << "\n"
     << "=============================================\n"
     << "              CREAR TAREA\n"
     << "=============================================\n";

    descripcion = ValidarEntrada::ingresarDescripcionCorta ("Ingrese la descripcion: ", 50);

    std::cout << "\nSeleccione la prioridad:\n";
    bool urgente = ValidarEntrada::respuestas_Si_O_No ("URGENTE", "NORMAL");

    std::string estado = "POR HACER";
    Tarea* nuevaTarea = new Tarea (0, descripcion, urgente, estado);
    return nuevaTarea;
}

ProcesosTarea::ProcesosTarea (UsuarioController* _uc,  TareaController* _tc, 
AsignacionController* _ac, Usuario* _uA, GestorHistorial* _gH) {
    this-> uc = _uc;
    this-> tc = _tc;
    this-> ac = _ac;
    this-> usuarioActivo = _uA;
    this-> gestorHistorial = _gH;
   
 }


void ProcesosTarea::agregarTarea () {
    Tarea* nuevaTarea = leerNuevaTarea ();
    AgregarTareaComando* agregar = nullptr; 
    AgregarAsignacionComando* AgregarAsignacion = nullptr;

    try {
        agregar = new AgregarTareaComando (tc,nuevaTarea, nuevaTarea->getPrioridad ());
        gestorHistorial->ejecutarComando (agregar);
        asignarResponsable (nuevaTarea->getIdTarea ());

    } catch (const std::exception& e) {
        std::cout << "\nError al crear la tarea: " << e.what() << std::endl;
        delete nuevaTarea; delete agregar; throw;
    }
}

void ProcesosTarea::ActualizarTarea () {
    int idTarea = ValidarEntrada::validarEntradaRango ("Ingrese el id de la tarea", 0, tc->getUltimoId ());

    Tarea* tareaBuscada = tc->buscarTarea (idTarea);
    if (tareaBuscada == nullptr) {
        std::cout << "Id invalido, no existe tarea con ese id" << std::endl; return;
    }

    std::string nuevaDescripcion;
    std::cout << "Desea cambiar la descripcion de la tarea" << std::endl;
    bool cambiarDescripcion = ValidarEntrada::respuestas_Si_O_No ("Si", "No, mantener actual");
    if (cambiarDescripcion) {
        nuevaDescripcion = ValidarEntrada::ingresarDescripcionCorta ("Ingrese su nueva descripccion", 50);
    }

    bool nuevaPrioridad;
    std::cout << "Desea cambiar la prioridad de la tarea" << std::endl;
    bool cambiarDescripcion = ValidarEntrada::respuestas_Si_O_No ("Si", "No, mantener actual");
    if (cambiarDescripcion) {
        nuevaPrioridad = ValidarEntrada::respuestas_Si_O_No ("Urgente", "Regular");
    }

    std::string estado;
    std::cout << "Desea cambiar el estado de la tarea" << std::endl;
    bool cambiarDescripcion = ValidarEntrada::respuestas_Si_O_No ("Si", "No, mantener actual");
    if (cambiarDescripcion) {
        std::cout << "ESTADOS" << std::endl
         << "1. " << Tarea::ESTADO[0] << std::endl
         << "2. " << Tarea::ESTADO[1] << std::endl
         << "3. " << Tarea::ESTADO[2] << std::endl;
        int opcion = ValidarEntrada::validarEntradaRango ("Ingrese su opcion",1,3);
        switch (opcion) {
            case 1:
                estado = Tarea::ESTADO[0]; break;
            case 2:
                estado = Tarea::ESTADO[0]; break;
            case 3: 
                estado = Tarea::ESTADO[0]; break;
            default: 
                break;
        };
    }

    ActualizarTareaComando* actualizar;
    try {
        actualizar = new ActualizarTareaComando (tc, idTarea,nuevaDescripcion, nuevaPrioridad, estado);
        gestorHistorial->ejecutarComando(actualizar);
    }catch (std::exception &e) {
        std::cout << "Error al actualizar tarea" << std::endl;
        delete actualizar;  throw;
    }

}

void ProcesosTarea::eliminarTarea () {
    int idTarea = ValidarEntrada::validarEntradaRango ("Ingrese el id de la tarea a eliminar",0, tc->getUltimoId ());
    Tarea* tareaAeliminar = tc->buscarTarea (idTarea);

    if (tareaAeliminar == nullptr){
        std::cout << "Tarea no encontrada" << std::endl; return;
    }

    EliminarTareaComando* eliminar;
    std::vector <int> usuariosAsignados = ac->getAsignacionesResponsablesDeTarea (idTarea);
    //EliminarAsignacionComando* eliminarAsignacion = new EliminarAsignacionComando ()
    try {
        eliminar = new EliminarTareaComando (tc, idTarea, tareaAeliminar->getPrioridad ());
        gestorHistorial->ejecutarComando (eliminar);
        //gestorHistorial-> ejecutarComando (eliminarAsignacion);
    } catch (std::exception &e) {
        delete eliminar;
    }
}

void ProcesosTarea::listarTareas () {
    //vedtores de las tareas originales
    std::vector <Tarea*> listaUrgente = tc->listarTareasRegulares ();
    std::vector <Tarea*> listaRegulares = tc->listarTareasRegulares ();

//VECTORES PARA CLASIFICAR LAS TAREAS
    std::vector<Tarea*> porHacer;
    std::vector<Tarea*> enProceso;
    std::vector<Tarea*> completadas;

    for (Tarea* tarea : listaUrgente) { //lista de urgentes

         if (tarea->getEstado() == "POR HACER") {
            porHacer.push_back(tarea);
        } else if (tarea->getEstado() == "EN PROCESO") {
            enProceso.push_back(tarea);
        } else if (tarea->getEstado() == "COMPLETADA") {
            completadas.push_back(tarea);
        }
    }

     for (Tarea* tarea : listaRegulares) { //lista de regular

         if (tarea->getEstado() == "POR HACER") {
            porHacer.push_back(tarea);
        } else if (tarea->getEstado() == "EN PROCESO") {
            enProceso.push_back(tarea);
        } else if (tarea->getEstado() == "COMPLETADA") {
            completadas.push_back(tarea);
        }
    }

    std::cout << "====================================================\n"
     << "                  TABLERO KANBAN\n"
     << "====================================================\n"
     << "\n"
     << "=================== POR HACER ======================\n";

    if (porHacer.empty()) {
        std::cout << "\nNo hay tareas por hacer.\n";

    } else {
        for (Tarea* tarea : porHacer) {
            std::cout << tarea->getIdTarea () << "| " <<tarea->getDescripcionTarea  () << "| " << ((tarea->getPrioridad ()) ? "Urgente" : "Regular");  
        }
    }
    std::cout << "\n=================== EN PROCESO ====================\n";

    if (enProceso.empty()) {
        std::cout << "\nNo hay tareas en proceso.\n";
    } else {
        for (Tarea* tarea : enProceso) {
           std::cout << tarea->getIdTarea () << "| " <<tarea->getDescripcionTarea  () << "| " << ((tarea->getPrioridad ()) ? "Urgente" : "Regular");  
        }
    }

    std::cout << "\n=================== COMPLETADAS ===================\n";

    if (completadas.empty()) {
        std::cout << "\nNo hay tareas completadas.\n";
    } else {
        for (Tarea* tarea : completadas) {
           std::cout << tarea->getIdTarea () << "| " <<tarea->getDescripcionTarea  () << "| " << ((tarea->getPrioridad ()) ? "Urgente" : "Regular");  
        }
    }
    std::cout << "\n====================================================\n";
}

void ProcesosTarea::listarArbolTarea () {
    int idTarea = ValidarEntrada::validarEntradaRango ("Ingrse el id de la tarea", 0 , tc->getUltimoId () );
    Tarea* tarea = tc->buscarTarea (idTarea);
    if (tarea == nullptr) {
        std::cout << "La Tarea buscada no existe " << std::endl; return;
    }
    Tarea* subTarea = tarea->getPrimerSubTarea();

    if (subTarea == nullptr) {
        std::cout << "\nEsta tarea no tiene subtareas.\n";
        return;
    }


    std::cout << "\n============== SUBTAREAS ==============\n\n";
    while (subTarea != nullptr) {
        std::cout << subTarea->getIdTarea () << "| " <<tarea->getDescripcionTarea  () << "| " << ((tarea->getPrioridad ()) ? "Urgente" : "Regular");  
        std::cout << "\n----------------------------------------\n\n";
        subTarea = subTarea->getSiguienteSubTarea();
    }

    std::cout << "\n========================================\n";
}