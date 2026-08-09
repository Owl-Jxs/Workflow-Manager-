#include "TareaDataBase.h"

const std::string TareaDataBase::FILENAME_TAREAS_REGULARES = "tareas_regulares.csv";
const std::string TareaDataBase::FILENAME_TAREAS_URGENTES = "tareas_urgentes.csv";
//Formato de guardado --> id, descripcion,estado, idPadre,cantidadSubTareas

TareaDataBase::TareaDataBase () {  //Constructror
    this-> listaTareasRegulares = new ColaFIFO ();
    this-> listaTareasUrgentes = new ColaFIFO ();
}

TareaDataBase::~TareaDataBase () { //Destructor
    if (listaTareasRegulares != nullptr) delete listaTareasRegulares;
    if (listaTareasUrgentes != nullptr) delete listaTareasUrgentes;
}
//Funciones auxiliares private 
void TareaDataBase::guardarArbol (Tarea* arbolTarea, std::ofstream& archivo) {//guardar la tarea y cada subtarea de 
    if (arbolTarea == nullptr) return; //si no existe subTarea
//atributos de tarea que se guardaran
    int idTarea, idPadre, cantidadSubTareas;
    std::string descripcion, estado;

//lo guardamos en el archivo
    archivo << arbolTarea->getIdTarea () << ",\"" << arbolTarea->getDescripcionTarea () << "\"," << arbolTarea->getEstado () << ',' 
            << arbolTarea->getPadreId () << ',' << arbolTarea->getCantidadSubTareas () << std::endl;

//guardar las subtareas de cada tarea
    guardarArbol (arbolTarea->getPrimerSubTarea (), archivo);
    guardarArbol (arbolTarea->getSiguienteSubTarea (), archivo);
}

void TareaDataBase::guardarLista (ColaFIFO* lista, std::string nombreArchivo) { //guardar una lista dada
    if (lista == nullptr) return;
//Crear archivo de  temporal 
    std::string nombreTemp = "Temp.csv";
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

ColaFIFO* cargarLista (std::string nombreArchivo, bool prioridad) { //carga una lista de los archivos
//abrimos el archivo de la lista
    std::ifstream lista (nombreArchivo);
    if (!lista.is_open () ) throw std::runtime_error ("Error al abrir el arvchivo con la lista");

    std::unordered_map <int, Tarea*> indiceTareas; //donde se almacenaran las tareas hasta encontrar sus arboles completos
    std::vector <Tarea*> tareasEnlistadas; //donde guardaremos las tareas hasta encontrar sus padres
    std::string linea;
    while (std::getline (lista, linea)) { //leeemos la linea
        std::stringstream lineaActual (linea);
        std::string idTarea, idPadre, cantidadSubTareas, descripcion, estado;
    //leemos la linea
        if (std::getline (lineaActual, idTarea, ',') && std::getline (lineaActual, descripcion, ',')
        && std::getline (lineaActual, estado, ',') && std::getline (lineaActual, idPadre, ',') && std::getline (lineaActual, idTarea) ) {
        //creamos la nueva tarea
            int tareaId = std::stoi (idTarea), tareaPadreId = std::stoi (idPadre);
            Tarea* nuevaTarea = new Tarea (tareaId,descripcion, prioridad, estado);
            nuevaTarea->setIdPadre (tareaPadreId);
            indiceTareas [tareaId] = nuevaTarea; //la ingresamos al "indice rapido"
            tareasEnlistadas.push_back (nuevaTarea);
        }
    }
    lista.close (); //cerramos el archivo de la lista
//creamos la cola
    ColaFIFO* nuevaLista = new ColaFIFO ();
//buscamos los hijoss de cada tarea y la ingresamos a la cola
    for (Tarea* tarea: tareasEnlistadas) {

        if (tarea->getIdTarea () == Tarea::sinPadre) { //si es una tarea raiz
            nuevaLista->encolar (tarea);
        } else{ //si es una subTarea
            int idPadre = tarea->getIdPadre ();
            if (indiceTareas.find (idPadre) != indiceTareas.end () ) { //buscamos la tarea padre
                indiceTareas [idPadre]->agregarSubTarea(tarea);
            }
        }
        
    }

  

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
                                            //=== === === metodos  public === === === 

void TareaDataBase::guardarBaseDeDatos () { //guardar la base de datos en ambas listas
        guardarLista (listaTareasRegulares, FILENAME_TAREAS_REGULARES); //Guardamos la lista regular
        guardarLista (listaTareasUrgentes, FILENAME_TAREAS_URGENTES); //guardamos la lista de urgentes
}


void TareaDataBase::cargarBaseDeDatos () {
    if (listaTareasRegulares != nullptr) delete listaTareasRegulares;
        this->listaTareasRegulares = cargarLista(FILENAME_TAREAS_REGULARES, false);
     if (listaTareasUrgentes != nullptr) delete listaTareasUrgentes;
        this->listaTareasUrgentes= cargarLista(FILENAME_TAREAS_URGENTES, true);          
}




void TareaDataBase::agregarTarea (Tarea* tarea) { //agrega una nueva tarea al sistema y a los archivos
    if (tarea == nullptr) throw std::invalid_argument ("No se puede agregar una tarea nula");
    std::string nombreLista = ""; //variable para el nombre del archivo de la lista
//la ingresamos a la lista que le corresponde
    if (tarea->getPrioridad ()){
       listaTareasUrgentes->encolar (tarea);
       nombreLista = FILENAME_TAREAS_URGENTES;
    } else {
        listaTareasRegulares->encolar (tarea);
       nombreLista = FILENAME_TAREAS_REGULARES;
    }
//la metemos a la DB
    guardarNuevaTarea (tarea, nombreLista);
}

void TareaDataBase::agregarSubTarea (Tarea* subTarea, int idTareaPadre, bool perteneceListaUrgente) {
    if (subTarea == nullptr) throw std::invalid_argument ("No se puede agregar una sub tarea nula");

    std::string nombreLista = ((perteneceListaUrgente) ? FILENAME_TAREAS_URGENTES : FILENAME_TAREAS_REGULARES); //el nombre del archivo
    ColaFIFO* colaBuscada = ((perteneceListaUrgente) ? listaTareasUrgentes : listaTareasRegulares); //la lista que buscamos
    bool tareaPadreEncontrada = false;

//buscamos en la lista la tarea Padre
    NodoTarea* nodoActual = colaBuscada->getFrente ();
    while (nodoActual != nullptr) {
        Tarea* tareaActual = nodoActual->datos;
        if (tareaActual->getPadreId () != idTareaPadre) { // si no coincide
            Tarea* subTarea= buscarSubTarea (tareaActual, idTareaPadre); //buscamos en sus hijos
       
            if (subTarea != nullptr) { //si se encuentra la tarea padre en alguno de sus hijos
                subTarea->agregarSubTarea (subTarea);
                tareaPadreEncontrada = true;
                break;
            }

        } else { //si se encuentra la subtarea buscada
            tareaActual->agregarSubTarea (subTarea);
            tareaPadreEncontrada = true;
            break;
        }
        nodoActual = nodoActual->siguiente;
    }
//si no se encuentra la subtarea
    if (!tareaPadreEncontrada) throw std::invalid_argument ("Tarea padre no encontrada");

//guardamos en el archivo
    std::ofstream listaDestinoSobreLectura (nombreLista, std::ios::app);
    listaDestinoSobreLectura << subTarea->getIdTarea () << ",\"" << subTarea->getDescripcionTarea () << "\"," << subTarea->getEstado () << ',' 
            << subTarea->getPadreId () << ',' << subTarea->getCantidadSubTareas () << std::endl;
}
