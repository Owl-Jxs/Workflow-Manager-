#include "TareaDataBase.h"
//guias para archivos
const std::string TareaDataBase::FILENAME_TAREAS_ACTIVAS = "data/Tareas_Data.csv";
const std::string TareaDataBase::FILENAME_TAREAS_COMPLETADAS = "data/tareas_completadas.csv";
const std::string TareaDataBase::ENUM_PRIORIDAD_TAREA [2] = {"Urgente", "Regular"}; 

//Formato de guardado --> id, descripcion, prioridad, estado, idPadre, cantidadSubTareas, ciclosEspera

//                                  === === ===     FUNCIONES PRIVATE AUXILIARES    === === ===

namespace {
    // Escapa las comillas dentro de un texto para poder guardarlo como campo CSV.
    std::string escaparCSV (const std::string& texto) {
        std::string resultado;
        for (char c : texto) {
            if (c == '"') {
                resultado += "\"\"";
            } else {
                resultado += c;
            }
        }
        return resultado;
    }

    // Divide una linea CSV en campos, respetando comillas dobles que envuelven
    // los campos. Una comilla doble dentro de un campo se escribe como "".
    std::vector<std::string> dividirCamposCSV (const std::string& linea) {
        std::vector<std::string> campos;
        std::string actual;
        bool dentroDeComillas = false;

        for (size_t i = 0; i < linea.size (); ++i) {
            char c = linea[i];
            if (c == '"') {
                if (dentroDeComillas && i + 1 < linea.size () && linea[i + 1] == '"') {
                    actual += '"';
                    ++i;
                } else {
                    dentroDeComillas = !dentroDeComillas;
                }
            } else if (c == ',' && !dentroDeComillas) {
                campos.push_back (actual);
                actual.clear ();
            } else {
                actual += c;
            }
        }

        campos.push_back (actual);
        return campos;
    }
}

std::string TareaDataBase::formularLinea (Tarea* tarea) { // formula la linea 
    std::stringstream linea;
//escribimos la prioridad de la tarea
    std::string perteneceListaUrgente = ((tarea->getPrioridad ()) ?ENUM_PRIORIDAD_TAREA[0] : ENUM_PRIORIDAD_TAREA[1]); 
//definimos la linea con el formato que estara en el archivo
    linea << tarea->getIdTarea () << ",\"" << escaparCSV (tarea->getDescripcionTarea ()) << "\"," << perteneceListaUrgente << ","
    << tarea->getEstado () << ',' << tarea->getIdPadre () << ',' << tarea->getCantidadSubTareas ()
    << ',' << tarea->getCiclosEspera ();
     
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

void TareaDataBase::guardarLista (Cola* lista, std::ofstream& archivo) { //guardar una lista dada
    if (lista == nullptr) return;
 //guardar cada posicion
    NodoTarea* nodoActual = lista->getFrente ();
    while (nodoActual != nullptr) {
        try{
            guardarArbol (nodoActual->datos, archivo);
        }   catch (const std::exception& e) {
            std::cerr << "Advertencia: Error al guardar tarea en archivo temporal: " << e.what() << std::endl;
         }       
        nodoActual = nodoActual->siguiente;
    }

}

void TareaDataBase::guardarVector (const std::vector<Tarea*>& enProceso, std::ofstream& archivo){
    for (Tarea* t : enProceso) {
        archivo << formularLinea (t) << std::endl;
    }
}

void TareaDataBase::guardarNuevaTarea (Tarea* tarea, std::ofstream& archivo) { //guarda una tarea en los archivos de las listas
   guardarArbol (tarea, archivo);
}

void TareaDataBase::eliminarTarea (std::vector<int> idArbolTarea, std::string nombreArchivo) {
    std::ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) throw std::runtime_error("Error al abrir el archivo");

    std::string nombreTemporal = "Temp.csv";
    std::ofstream archivoTemp(nombreTemporal);

    if (!archivoTemp.is_open()) {
        archivo.close();
        throw std::runtime_error("Error al crear archivo temporal");
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::vector <std::string> lineaActual = dividirCamposCSV (linea);
        try {
            bool escribir = true;
            for (int i : idArbolTarea) {

                if (i < 0) throw std::invalid_argument ("Id invalido");
                if (lineaActual[0] == std::to_string (i) ) {
                    escribir = false; break;
                }
            }  

            if (escribir) {// Si NO es la tarea que queremos eliminar,la copiamos al archivo temporal
                    archivoTemp << linea << std::endl;
                }
        } catch (std::exception &e) {
            std::cout << e.what (); continue;
        }
        
        
    }
    archivo.close();    archivoTemp.close();
// Eliminar el archivo original
    std::remove(nombreArchivo.c_str());
// Renombrar el temporal con el nombre original
    if (std::rename(nombreTemporal.c_str(), nombreArchivo.c_str()) != 0)throw std::runtime_error("Error al reemplazar el archivo");    
}

//                                  === === ===     CONSTRUCTOR Y DESTRUCTOR    === === ===
TareaDataBase::TareaDataBase () { }
TareaDataBase::~TareaDataBase () { }

//                                  === === ===     METODOS PUBLICOS    === === ===
void TareaDataBase::cargarTareasActivas (Cola* regulares, Cola* urgentes, std::vector<Tarea*>& enProceso,
 Cola* enRevision, int &ultimoId) {

    std::ifstream archivoLectura (FILENAME_TAREAS_ACTIVAS);// cargamos el archivo con los datos
    if (!archivoLectura.is_open()) throw std::runtime_error ("error al cargar los archivos");;

    std::unordered_map <int, Tarea*> indiceTareas; // guarda todas las tareas principales de los archivos
    std::unordered_map   <int, int> indicePadres; // guarda el indice de los padres para colocar las subtareas con sus padres
    std::vector <Tarea*> tareasEnlistadas; //donde guardaremos todas las tareas
    std::string linea;
    int idMax = 0;

    while (std::getline (archivoLectura, linea) ) { //cargamos TODAS las tareas y las ordenamos
        if (linea.empty ()) continue;
        try {
            std::vector < std::string> camposPorLinea = dividirCamposCSV (linea);
            if (camposPorLinea.size () < 7) throw std::invalid_argument("linea incompleta"); //si no tiene todos los campos
        //Formato de guardado --> id, descripcion, prioridad, estado, idPadre, cantidadSubTareas, ciclosEspera

            int idTarea = std::stoi (camposPorLinea[0]);
            int idPadre = std::stoi (camposPorLinea[4]);

            if (idPadre >= idMax && idPadre >= idTarea) {
                idMax = idPadre;
            } else if (idTarea >= idMax && idTarea >= idPadre) {
                idMax = idTarea;
            }


            bool urgente = ((ENUM_PRIORIDAD_TAREA[0] == camposPorLinea[2]) ? true : false);

            Tarea* nuevaTarea = new Tarea (idTarea, camposPorLinea[1], urgente, camposPorLinea[3]); //creamos la tarea
            if (camposPorLinea.size() >= 7) nuevaTarea->setCiclosEspera(std::stoi(camposPorLinea[6]));
            
            indiceTareas [idTarea] = nuevaTarea;
            indicePadres [idTarea] = idPadre;
            tareasEnlistadas.push_back (nuevaTarea);

        } catch (std::exception &e) {
            std::cerr << "Advertencia: linea invalida: " << linea << " -> " << e.what() << std::endl;   
        }
    }
    archivoLectura.close ();

    for (Tarea* t: tareasEnlistadas) { // construimos todas las listas
        if (t->getIdPadre () == Tarea::sinPadre){ // si es una  tarea "Raiz"
            try {
                if (t->getEstado() == Tarea::ESTADO [0] && t->getPrioridad ()) urgentes->encolar (t); //si es una tarea urgente y esta en por hacer
                else if (t->getEstado() == Tarea::ESTADO [0] && !(t->getPrioridad () ) ) regulares->encolar (t); // si es una tarea regular y esta en por hacer
                else if (t->getEstado () == Tarea::ESTADO[1]) enProceso.push_back (t); 
                else if (t->getEstado () == Tarea::ESTADO[2]) enRevision->encolar (t);
            } catch (std::exception &e) {
                delete t; std::cout << e.what ();
            }
    
        } else if (indicePadres.find (t->getIdPadre ()) != indicePadres.end ()) { // si es una subTarea con un padre
            try {
                indiceTareas[t->getIdPadre ()]->agregarSubTarea(t);
            } catch (const std::exception& e) {
                std::cerr << "Advertencia: fallo al adjuntar subtarea " << t->getIdTarea() << std::endl;    delete t;
            }
        } else {
            std::cout << "Tarea con padre Huerfano, se porcedera a ignorar" <<std::endl; delete t;
        }
       
    }
    ultimoId = idMax;
}

void TareaDataBase::guardarTareasActivas (Cola* regulares, Cola* urgentes, const std::vector<Tarea*>& enProceso, Cola* enRevision){ 
    std::ofstream archivo(FILENAME_TAREAS_ACTIVAS, std::ios::trunc);
    if (!archivo.is_open()) throw std::runtime_error ("Error al abrir el archivo de guardado");

// Guardar las colas usando la función auxiliar private
    guardarLista (regulares, archivo);
    guardarLista (urgentes, archivo);
    guardarLista (enRevision, archivo);
// Guardar el vector
    guardarVector (enProceso, archivo);
    archivo.close();
}

void TareaDataBase::guardarTareasCompletadas (Cola* listaTareasCompletadas) {
    std::ofstream archivo (FILENAME_TAREAS_COMPLETADAS, std::ios::trunc);
    if (!archivo.is_open()) throw std::runtime_error ("Error al abrir el archivos");

// Guardar las colas usando la función auxiliar private   
    guardarLista (listaTareasCompletadas, archivo);
    archivo.close ();
}
   
void TareaDataBase::guardarNuevaTareaEnArchivo (Tarea* tarea) { //agrega una nueva tarea al sistema y a los archivos
    if (tarea == nullptr) throw std::invalid_argument ("No se puede agregar una tarea nula");
    std::ofstream archivo (FILENAME_TAREAS_ACTIVAS, std::ios::app);
    if (!archivo.is_open()) throw std::runtime_error("Error al abrir el archivo");

    guardarNuevaTarea (tarea, archivo); //la guardamos en el archivo
    archivo.close();
}

void TareaDataBase::actualizarEstadoTarea (Tarea* tarea) {
    if (tarea == nullptr) throw std::invalid_argument("tarea nula");
    if (tarea->getIdTarea () < 0) throw std::invalid_argument("ID de tarea invalido");
    
    std::ifstream archivo(FILENAME_TAREAS_ACTIVAS);
    if (!archivo.is_open()) throw std::runtime_error("Error al abrir el archivo");

    std::string nombreTemporal = "Temp.csv";
    std::ofstream archivoTemp(nombreTemporal);
    if (!archivoTemp.is_open()) {
        archivo.close();
        throw std::runtime_error("Error al crear archivo temporal");
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::vector <std::string> lineaActual = dividirCamposCSV (linea);

        if (lineaActual[0] != std::to_string (tarea->getIdTarea ()) ) {// Si NO es la tarea que queremos eliminar,la copiamos al archivo temporal
                archivoTemp << linea << '\n';
        } else {
            archivoTemp << formularLinea (tarea) << std::endl;
        }
    }
    archivo.close();    archivoTemp.close();
// Eliminar el archivo original
    std::remove(FILENAME_TAREAS_ACTIVAS.c_str());
// Renombrar el temporal con el nombre original
    if (std::rename(nombreTemporal.c_str(), FILENAME_TAREAS_ACTIVAS.c_str()) != 0)throw std::runtime_error("Error al reemplazar el archivo"); 
}

void TareaDataBase::eliminarTareaActiva (std::vector<int> idArbolTarea){
    eliminarTarea (idArbolTarea, FILENAME_TAREAS_ACTIVAS);
}

void TareaDataBase::registrarTareaCompletada (Tarea* tarea) {  
    if (tarea == nullptr)   throw std::invalid_argument ("No se completar una tarea nula");
    std::ofstream archivoGuardado (FILENAME_TAREAS_COMPLETADAS, std::ios::app);
    if (!archivoGuardado.is_open ()) throw std::runtime_error ("Error al abrir el archivo de la lista de tareas");
    
    guardarNuevaTarea (tarea, archivoGuardado);
    archivoGuardado.close ();
}

void TareaDataBase::eliminarRegistroTareaCompletada (std::vector<int> idArbolTarea) {
    eliminarTarea (idArbolTarea, FILENAME_TAREAS_COMPLETADAS);
}