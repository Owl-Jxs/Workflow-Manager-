#include "ProcesosTarea.h"



Tarea* ProcesosTarea::leerNuevaTarea () {
   if (tc == nullptr) {
        std::cout << "Error: el TareaController no esta disponible.\n";
        return nullptr;
    }
    std::string descripcion;

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

void ProcesosTarea::mostrarInformacionTarea (Tarea* tarea, bool mostrarInfoSubTareas, int nivel) {
    if (tarea == nullptr) return;

    std::string sangria(nivel * 3, ' ');

    std::cout << sangria << "|-- ID: " << tarea->getIdTarea()
              << " | " << tarea->getDescripcionTarea()
              << " | " << (tarea->getPrioridad() ? "Urgente" : "Regular") << "\n";

    if (!mostrarInfoSubTareas) return;

    Tarea* subTarea = tarea->getPrimerSubTarea();
    while (subTarea != nullptr) { 
        // Llamada recursiva para bajar de nivel en el árbol
        mostrarInformacionTarea(subTarea, mostrarInfoSubTareas, nivel + 1);
        
        // Moverse horizontalmente a la siguiente subTarea
        subTarea = subTarea->getSiguienteSubTarea (); 
    }
}

//constructor
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

    try {
        agregar = new AgregarTareaComando (tc,nuevaTarea);
        gestorHistorial->ejecutarComando (agregar);
        
    } catch (const std::exception& e) {
        std::cout << "\nError al crear la tarea: " << e.what() << std::endl; delete nuevaTarea;
    }
}

void ProcesosTarea::agregarSubTarea () {
    int idTarea = ValidarEntrada::validarEntradaRango ("Ingrese el id de la tarea padre", 0, tc->getUltimoId ());

    Tarea* tareaBuscada = tc->buscarTareaPorHacer (idTarea);
    if (tareaBuscada == nullptr) tareaBuscada = tc->buscarTareaEnProceso (idTarea);
    if (tareaBuscada == nullptr) { std::cout << "Id inexistente" << std::endl; return; }

    Tarea* nuevaTarea = leerNuevaTarea ();
    nuevaTarea->setIdPadre(idTarea);
    AgregarTareaComando* agregar = nullptr; 
    try {
        agregar = new AgregarTareaComando (tc,nuevaTarea);
        gestorHistorial->ejecutarComando (agregar);
        
    } catch (const std::exception& e) {
        std::cout << "\nError al crear la subtarea: " << e.what() << std::endl; delete nuevaTarea;
    }

}

void ProcesosTarea::ActualizarTarea () {
    int idTarea = ValidarEntrada::validarEntradaRango ("Ingrese el id de la tarea", 0, tc->getUltimoId ());

    Tarea* tareaBuscada = tc->buscarTareaPorHacer (idTarea);
    if (tareaBuscada == nullptr) tareaBuscada= tc->buscarTareaEnProceso (idTarea);
    if (tareaBuscada == nullptr) { std::cout << "Id invalido, no existe tarea con ese id" << std::endl; return; }

    std::string nuevaDescripcion = tareaBuscada ->getDescripcionTarea ();
    std::cout << "Desea cambiar la descripcion de la tarea" << std::endl;
    bool cambiarDescripcion = ValidarEntrada::respuestas_Si_O_No ("Si", "No, mantener actual");
    if (cambiarDescripcion) {
        nuevaDescripcion = ValidarEntrada::ingresarDescripcionCorta ("Ingrese su nueva descripccion", 50);
    }

    bool nuevaPrioridad = tareaBuscada->getPrioridad ();
    std::cout << "Desea cambiar la prioridad de la tarea" << std::endl;
    bool cambiarPrioridad = ValidarEntrada::respuestas_Si_O_No ("Si", "No, mantener actual");
    if (cambiarPrioridad) {
        nuevaPrioridad = ValidarEntrada::respuestas_Si_O_No ("Urgente", "Regular");
    }

    ActualizarTareaComando* actualizar = nullptr;
    try {
        actualizar = new ActualizarTareaComando (tc, idTarea,nuevaDescripcion, nuevaPrioridad);
        gestorHistorial->ejecutarComando(actualizar);
    }catch (std::exception &e) {
        std::cout << "Error al actualizar tarea: " << e.what() << std::endl;
    }

}

void ProcesosTarea::asignarResponsable () {
    int id = ValidarEntrada::validarCodigoNumerico ("ingrese el id del usuario que sera responsable", 9);
    Usuario* usuarioResponsable = uc->buscarUsuarioPorId (id);

    if (usuarioResponsable != nullptr) {
        AsignarResponsableComando* nuevoResponsable = nullptr;

        try {
            nuevoResponsable = new AsignarResponsableComando (tc, ac, id);
            gestorHistorial->ejecutarComando (nuevoResponsable);
        } catch (std::exception &e){
            std::cout << e.what () << std::endl; delete nuevoResponsable;
        }

    }

}

void ProcesosTarea::verificarTareaEnColaRevision () {
    Tarea* frente = tc->getSiguienteTareaProcesable ();
    if (frente == nullptr ) {
        std::cout << "No hay tareas en la cola de revision " << std::endl; return;
    }
    mostrarInformacionTarea (frente, true, 1);
    std::cout << "\n\nLa tarea cumple los requisitos para clasificarla como completada? " << std::endl;
    bool validarTarea = ValidarEntrada::respuestas_Si_O_No  ("Si, Validar", "No, Regresar a En proceso");

    if (validarTarea) {
        validarTareaEnRevisionComando* validar = nullptr;
        try {
            validar = new validarTareaEnRevisionComando (tc, ac, frente);
            gestorHistorial->ejecutarComando  (validar);
        } catch (std::exception &e) {
            delete validar;
        }
    } else{
        RechazarTareaEnRevisionComando* rechazar = nullptr;
        try {
            rechazar = new RechazarTareaEnRevisionComando (tc, ac, frente);
            gestorHistorial->ejecutarComando  (rechazar);
        } catch (std::exception &e) {
            delete rechazar;
        }
    }
}

void ProcesosTarea::eliminarTarea () {
    int idTarea = ValidarEntrada::validarEntradaRango ("Ingrese el id de la tarea a eliminar",0, tc->getUltimoId ());
    
    Tarea* tareaAeliminar = tc->buscarTareaPorHacer (idTarea);
    if (tareaAeliminar == nullptr) tareaAeliminar = tc->buscarTareaEnProceso (idTarea);
    if (tareaAeliminar == nullptr){ std::cout << "Tarea no encontrada" << std::endl; return; }

    EliminarTareaComando* eliminar = nullptr;
    try {
        eliminar = new EliminarTareaComando (tc, ac, idTarea);
        gestorHistorial->ejecutarComando (eliminar);
    } catch (std::exception &e) {
        delete eliminar;
    }
   
}


void ProcesosTarea::mostrarTableroKanban () {
    //vedtores de las tareas originales
    std::vector <Tarea*> listaUrgente = tc->listarTareasUrgentes ();
    std::vector <Tarea*> listaRegulares = tc->listarTareasRegulares ();
//VECTORES PARA CLASIFICAR LAS TAREAS
    std::vector<Tarea*> enProceso = tc->listarTareasEnProceso ();
    std::vector<Tarea*> enRevision = tc->listarTareasEnRevision ();
    std::vector<Tarea*> completadas = tc->listarTareasCompletadas ();

    if (listaUrgente.empty () && listaRegulares.empty () && enProceso.empty () && enRevision.empty () && completadas.empty ()) {
        std::cout << "No hay registros de tareas" << std::endl; return;
    }

    std::cout << "====================================================\n"
     << "                  TABLERO KANBAN\n"
     << "====================================================\n"
     << "\n"
     << "=================== POR HACER ======================\n";

    if (listaUrgente.empty() && listaRegulares.empty () )  {
        std::cout << "\nNo hay tareas por hacer.\n";

    } else {
        for (Tarea* tarea : listaUrgente) { mostrarInformacionTarea (tarea, false, 1); }
        for (Tarea* tarea : listaRegulares) { mostrarInformacionTarea (tarea, false, 1); }

    }
    std::cout << "\n=================== EN PROCESO ====================\n";

    if (enProceso.empty()) {
        std::cout << "\nNo hay tareas en proceso.\n";
    } else {
        for (Tarea* tarea : enProceso) { mostrarInformacionTarea(tarea, false, 1);  }
    }

     std::cout << "\n=================== EN REVISION ===================\n";
    if (enRevision.empty()) {  
        std::cout << "\nNo hay tareas en revision.\n";
    } else {
        for (Tarea* tarea : enRevision) { mostrarInformacionTarea  (tarea, false, 1);    }
    }
    
    std::cout << "\n=================== COMPLETADAS ===================\n";
    if (completadas.empty()) {
        std::cout << "\nNo hay tareas completadas.\n";
    } else {
        for (Tarea* tarea : completadas) { mostrarInformacionTarea  (tarea, false, 1);    }
    }
    std::cout << "\n====================================================\n";
}

void ProcesosTarea::mostrarTareaPorId () {
    int idTarea = ValidarEntrada::validarEntradaRango ("Ingrese el id de la tarea", 0 , tc->getUltimoId () );

    Tarea* tarea = tc->buscarTareaPorHacer (idTarea);
    if (tarea == nullptr) tarea = tc->buscarTareaEnProceso (idTarea);
    if (tarea == nullptr) tarea = tc->buscarTareaEnRevision (idTarea);
    if (tarea == nullptr) tarea = tc->buscarTareaCompletada (idTarea);
    if (tarea == nullptr) {std::cout << "La Tarea buscada no existe " << std::endl; return;}
    
    mostrarInformacionTarea (tarea, true, 1); std::system ("pause"); std::system ("cls");
}

void ProcesosTarea::ordenarLista () {
    std::vector <Tarea*> vectorOrdenado = tc->listarTodasLasTareasActivas (); 
     
    if (vectorOrdenado.empty ()){
        std::cout << "No hay registros de tareas" << std::endl; return;
    }

    std::cout << "Que algoritmo de ordenamiento prefiere usar? " << std::endl
     << "1. QuickSort" << std::endl
     << "2. MergeSort" << std::endl 
     << "3. BubbleSort" << std::endl;
    int opcion = ValidarEntrada::validarEntradaRango ("Ingrese su opcion", 1,3);   
    
    std::cout << "Desea usar un ordenamiento ascendente o descendente? " << std::endl
     << "1. Ascendente" << std::endl
     << "2. Descendente" << std::endl; 
    int opcionOrden = ValidarEntrada::validarEntradaRango ("Ingrese su opcion", 1,2);   
    Icondicion<Tarea*>* condicion;

    if (opcionOrden == 1){
        condicion = new ordenarTareaPorIdAscedente ();
    } else {
        condicion = new ordenarTareaPorIdDescendente ();
    }

    switch (opcion) {
    case 1:{    
        QuickSort<Tarea*>* algoritmo = new QuickSort<Tarea*> ();    algoritmo->ordenar (vectorOrdenado, condicion);
        delete algoritmo;
        break;
    }
    case 2:{
        MergeSort<Tarea*>* algoritmo = new MergeSort<Tarea*> ();     algoritmo->sort (vectorOrdenado, condicion);
        delete algoritmo;
        break;
    }
    case 3:{
        BubbleSort<Tarea*>* algoritmo = new BubbleSort<Tarea*> ();    algoritmo->sort (vectorOrdenado, condicion);
        delete algoritmo;
        break;
    }
    default:
        break;
    }
    
    delete condicion;
    for (Tarea* t : vectorOrdenado) {
        std:: cout << "--------------------------------------------------------------------------------" << std::endl;
            mostrarInformacionTarea (t, false, 1);
    }
}
