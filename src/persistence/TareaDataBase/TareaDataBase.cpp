#include "TareaDataBase.h"
//guias para archivos
const std::string TareaDataBase::FILENAME_TAREAS_REGULARES = "data/tareas_regulares.csv";
const std::string TareaDataBase::FILENAME_TAREAS_URGENTES = "data/tareas_urgentes.csv";
const std::string TareaDataBase::ENUM_PRIORIDAD_TAREA [2] = {"Urgente", "Regular"}; 

//Formato de guardado --> id, descripcion, prioridad, estado, idPadre,cantidadSubTareas

//                                  === === ===     FUNCIONES PRIVATE AUXILIARES    === === ===
std::string TareaDataBase::formularLinea (Tarea* tarea) { // formula la linea 
    std::stringstream linea;
//escribimos la prioridad de la tarea
    std::string perteneceListaUrgente = ((tarea->getPrioridad ()) ?ENUM_PRIORIDAD_TAREA[0] : ENUM_PRIORIDAD_TAREA[1]); 
//definimos la linea con el formato que estara en el archivo
    linea << tarea->getIdTarea () << ",\"" << tarea->getDescripcionTarea () << "\"," << perteneceListaUrgente << ","
    << tarea->getEstado () << ',' << tarea->getPadreId () << ',' << tarea->getCantidadSubTareas ();
     
    return linea.str ();
}

void TareaDataBase::guardarArbol (Tarea* arbolTarea, std::ofstream& archivo) { //guardar la tarea y cada subtarea de 
    if (arbolTarea == nullptr) return; //si no existe subTarea
//lo guardamos en el archivo
    archivo << formularLinea (arbolTarea) << std::endl;

//guardar las subtareas de cada tarea
    guardarArbol (arbolTarea->getPrimerSubTarea (), archivo);
    guardarArbol (arbolTarea->getSiguienteSubTarea (), archivo);
}

void TareaDataBase::guardarLista (ColaFIFO* lista, std::string nombreArchivo) { //guardar una lista dada
    if (lista == nullptr) throw std::invalid_argument ("La lista del archivo: " + nombreArchivo + "Esta vacia");
//Crear archivo de  temporal 
    std::string nombreTemp = "data/Temp.csv";
    std::ofstream archivoTemp (nombreTemp);
    if (!archivoTemp.is_open () ) throw std::runtime_error("Error al abrir el archivo temporal.");

 //guardar cada posicion
    NodoTarea* nodoActual = lista->getFrente ();
    while (nodoActual != nullptr) {
        try{
            guardarArbol (nodoActual->datos, archivoTemp);
        }   catch (const std::exception& e) {
            std::cerr << "Advertencia: Error al guardar tarea en archivo temporal: " << e.what() << std::endl;
         }       
        nodoActual = nodoActual->siguiente;
    }
    archivoTemp.close (); //cerramos el archivo

//cambiamos el archivo de persistencia
    std::remove (nombreArchivo.c_str());
    if (std::rename (nombreTemp.c_str () , nombreArchivo.c_str()) != 0) throw std::runtime_error("Error al renombrar el archivo temporal de tareas.");   
}

void TareaDataBase::guardarNuevaTarea (Tarea* tarea, std::string nombreArchivo) { //guarda una tarea en los archivos de las listas
    std::ofstream archivoListas (nombreArchivo, std::ios::app);
    if (!archivoListas.is_open ()) throw std::runtime_error ("Error al abrir el archivo de la lista de tareas");
    guardarArbol (tarea, archivoListas);
    archivoListas.close ();
}

ColaFIFO* TareaDataBase::cargarLista (std::string nombreArchivo) { //carga una lista de los archivos
//abrimos el archivo de la lista
    std::ifstream lista (nombreArchivo);
    if (!lista.is_open () ) throw std::runtime_error ("Error al abrir el arvchivo con la lista");

    std::unordered_map <int, Tarea*> indiceTareas; //donde se almacenaran las tareas hasta encontrar sus arboles completos
    std::vector <Tarea*> tareasEnlistadas; //donde guardaremos las tareas hasta encontrar sus padres
    std::string linea; // recorre cada linea del archivo

//leemos el archivo
    while (std::getline (lista, linea)) { 
        std::stringstream lineaActual (linea);
        std::string idTareaTXT, idPadreTXT, cantidadSubTareasTXT, descripcionTXT, estadoTXT, prioridadTXT;
    //leemos la linea
        if (std::getline (lineaActual, idTareaTXT, ',') && std::getline (lineaActual, descripcionTXT, ',') 
        && std::getline (lineaActual, prioridadTXT, ',')&& std::getline (lineaActual, estadoTXT, ',') 
        && std::getline (lineaActual, idPadreTXT, ',') && std::getline (lineaActual, idTareaTXT) ) {

        //creamos la nueva tarea
            int idTarea = std::stoi (idTareaTXT), idPadre = std::stoi (idPadreTXT);
            bool urgente= ( (prioridadTXT == ENUM_PRIORIDAD_TAREA [0]) ? true : false);

            Tarea* nuevaTarea = new Tarea (idTarea,descripcionTXT, urgente, estadoTXT);
            nuevaTarea->setIdPadre (idPadre);
            indiceTareas [idTarea] = nuevaTarea; //la ingresamos al "indice rapido"
            tareasEnlistadas.push_back (nuevaTarea);
        }
    }
    lista.close (); //cerramos el archivo de la lista

//creamos la cola
    ColaFIFO* nuevaLista = new ColaFIFO ();
//buscamos los hijoss de cada tarea y la ingresamos a la cola
    for (Tarea* tarea: tareasEnlistadas) {

        if (tarea->getIdPadre () == Tarea::sinPadre) { //si es una tarea raiz 
            nuevaLista->encolar (tarea);
        } else{ //si es una subTarea
            int idPadre = tarea->getIdPadre ();
            if (indiceTareas.find (idPadre) != indiceTareas.end () ) { //buscamos la tarea padre
                indiceTareas [idPadre]->agregarSubTarea(tarea);
            }
        }
        
    }
    return nuevaLista;
}

Tarea* TareaDataBase::buscarSubTarea (Tarea* TareaActual, int idBuscado) { //buscar una subtarea en el arbol
    if (TareaActual->getIdTarea () == idBuscado) {
        return TareaActual;
    } else {
        Tarea* primerSubTarea = buscarSubTarea (TareaActual->getPrimerSubTarea (), idBuscado);
        if (primerSubTarea != nullptr) return primerSubTarea;
        Tarea* siguienteSubTarea = buscarSubTarea (TareaActual->getSiguienteSubTarea (), idBuscado);
        if (siguienteSubTarea != nullptr) return siguienteSubTarea;
    } 
    return nullptr;
}


//                                  === === ===     CONSTRUCTOR Y DESTRUCTOR    === === ===
TareaDataBase::TareaDataBase () { }
TareaDataBase::~TareaDataBase () { }

//                                  === === ===     METODOS PUBLICOS    === === ===

void TareaDataBase::guardarListaEnArchivo (ColaFIFO* listaCompleta, bool perteneceListaUrgente) { //guardar la base de datos en ambas listas
    std::string nombreArchivo = ((perteneceListaUrgente) ? FILENAME_TAREAS_URGENTES : FILENAME_TAREAS_REGULARES);
    guardarLista (listaCompleta, nombreArchivo); //Guardamos la lista
}

ColaFIFO* TareaDataBase:: cargarListaDelArchivo (bool perteneceListaUrgente) {
     std::string nombreArchivo = ((perteneceListaUrgente) ? FILENAME_TAREAS_URGENTES : FILENAME_TAREAS_REGULARES);
    return cargarLista (nombreArchivo);
}

void TareaDataBase::guardarNuevaTareaEnArchivo (Tarea* tarea, bool perteneceListaUrgente) { //agrega una nueva tarea al sistema y a los archivos
    if (tarea == nullptr) throw std::invalid_argument ("No se puede agregar una tarea nula");
    std::string nombreArchivo = ((perteneceListaUrgente) ? FILENAME_TAREAS_URGENTES : FILENAME_TAREAS_REGULARES);
    guardarNuevaTarea (tarea, nombreArchivo); //la guaardamos en el archivo
}

void TareaDataBase::guardarguardarNuevaSubTareaEnArchivo (Tarea* subTarea, bool perteneceListaUrgente) {
    if (subTarea == nullptr) throw std::invalid_argument ("No se puede agregar una sub tarea nula");
    std::string nombreArchivo = ((perteneceListaUrgente) ? FILENAME_TAREAS_URGENTES : FILENAME_TAREAS_REGULARES); //el nombre del archivo

//guardamos en el archivo
    std::ofstream listaDestinoSobreLectura (nombreArchivo, std::ios::app);
    listaDestinoSobreLectura << formularLinea (subTarea) << std::endl;
}
