#include "TareaDataBase.h"
//guias para archivos
const std::string TareaDataBase::FILENAME_TAREAS_REGULARES = "data/tareas_regulares.csv";
const std::string TareaDataBase::FILENAME_TAREAS_URGENTES = "data/tareas_urgentes.csv";
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

void TareaDataBase::guardarLista (ColaFIFO* lista, std::string nombreArchivo) { //guardar una lista dada
    if (lista == nullptr) throw std::invalid_argument ("La lista del archivo: " + nombreArchivo + "Esta vacia");
//Crear archivo de  temporal 
    std::string nombreTemp = "data/tareas_temporal.csv";
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
    std::remove (nombreArchivo.c_str ()); // si el archivo no existia, no es un error
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
    if (!lista.is_open () ) return new ColaFIFO ();

    std::unordered_map <int, Tarea*> indiceTareas; //donde se almacenaran las tareas hasta encontrar sus arboles completos
    std::unordered_map <int, int> padresPorId; //guarda el id del padre de cada tarea para reconstruir el arbol
    std::vector <Tarea*> tareasEnlistadas; //donde guardaremos las tareas hasta encontrar sus padres
    std::string linea; // recorre cada linea del archivo

//leemos el archivo
    while (std::getline (lista, linea)) {
        if (linea.empty ()) continue;

        try {
            std::vector<std::string> campos = dividirCamposCSV (linea);
            if (campos.size () < 6) throw std::invalid_argument ("linea incompleta");

            int idTarea = std::stoi (campos[0]);
            int idPadre = std::stoi (campos[4]);
            bool urgente = (campos[2] == ENUM_PRIORIDAD_TAREA[0]);
            // campos[5] = cantidadSubTareas: se recalcula al reconstruir el arbol

            Tarea* nuevaTarea = new Tarea (idTarea, campos[1], urgente, campos[3]);
            if (campos.size () >= 7) {
                nuevaTarea->setCiclosEspera (std::stoi (campos[6]));
            }
            indiceTareas [idTarea] = nuevaTarea; //la ingresamos al "indice rapido"
            padresPorId [idTarea] = idPadre; //guardamos su relacion para adjuntarla despues
            tareasEnlistadas.push_back (nuevaTarea);
        } catch (const std::exception& e) {
            std::cerr << "Advertencia: linea invalida en " << nombreArchivo << ": " << linea
                      << " -> " << e.what () << std::endl;
        }
    }
    lista.close (); //cerramos el archivo de la lista

//creamos la cola
    ColaFIFO* nuevaLista = new ColaFIFO ();
//buscamos los hijos de cada tarea y la ingresamos a la cola
    for (Tarea* tarea: tareasEnlistadas) {
        int idPadre = padresPorId [tarea->getIdTarea ()];

        if (idPadre == Tarea::sinPadre) { //si es una tarea raiz
            try {
                nuevaLista->encolar (tarea);
            } catch (const std::exception& e) {
                std::cerr << "Advertencia: tarea repetida (" << tarea->getIdTarea ()
                          << "); se ignora: " << e.what () << std::endl;
                delete tarea;
            }
        } else if (indiceTareas.find (idPadre) != indiceTareas.end ()) { //buscamos la tarea padre
            try {
                indiceTareas [idPadre]->agregarSubTarea (tarea);
            } catch (const std::exception& e) {
                std::cerr << "Advertencia: no se pudo adjuntar la tarea " << tarea->getIdTarea ()
                          << " a su padre " << idPadre << ": " << e.what () << std::endl;
                delete tarea;
            }
        } else { //el padre no existe en el archivo
            std::cerr << "Advertencia: la tarea " << tarea->getIdTarea ()
                      << " tiene un padre inexistente (" << idPadre << "); se ignora." << std::endl;
            delete tarea;
        }
    }
    return nuevaLista;
}

Tarea* TareaDataBase::buscarSubTarea (Tarea* TareaActual, int idBuscado) { //buscar una subtarea en el arbol
    if (TareaActual == nullptr) return nullptr;
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
    guardarNuevaTarea (tarea, nombreArchivo); //la guardamos en el archivo
}

void TareaDataBase::guardarNuevaSubTareaEnArchivo (Tarea* subTarea, bool perteneceListaUrgente) {
    if (subTarea == nullptr) throw std::invalid_argument ("No se puede agregar una sub tarea nula");
    std::string nombreArchivo = ((perteneceListaUrgente) ? FILENAME_TAREAS_URGENTES : FILENAME_TAREAS_REGULARES); //el nombre del archivo

//guardamos en el archivo
    std::ofstream listaDestino (nombreArchivo, std::ios::app);
    if (!listaDestino.is_open ()) throw std::runtime_error ("Error al abrir el archivo de la lista de tareas");
    listaDestino << formularLinea (subTarea) << std::endl;
    listaDestino.close ();
}
