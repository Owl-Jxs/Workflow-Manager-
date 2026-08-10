#include "AsignacionDataBase.h"
const std::string AsignacionDataBase::FILENAME_ASIGNACIONES = "asignaciones.csv"; // Nombre del archivo para almacenar las asignaciones
// Formato de guardado ----> idTarea, idUsuario

AsignacionDataBase::AsignacionDataBase() { }

AsignacionDataBase::~AsignacionDataBase() { }

void AsignacionDataBase::guardarAsignacionesEnArchivo(ListaAsignaciones* lista) { //guarda todas las asignaciones de la lista en el archivo
    
    std::ofstream archivoTemporalAsignaciones("temporal.csv");
    if (!archivoTemporalAsignaciones.is_open()) throw std::runtime_error("Error al abrir el archivo temporal de asignaciones.");

// Guardar todas las asignaciones de la lista en el archivo temporal
    NodoAsignacion* actual = lista->getCabeza(); //primer nodo de la lista
    while (actual != nullptr) {
        archivoTemporalAsignaciones << actual->idTarea << "," << actual->idUsuario << std::endl;
        actual = actual->siguiente;
    }
    archivoTemporalAsignaciones.close(); //cerrar el archivo temporal

// Reemplazar el archivo original con el temporal
    if (std::remove (FILENAME_ASIGNACIONES.c_str()) != 0) throw std::runtime_error("Error al eliminar el archivo original de asignaciones.");
    if (std::rename ("temporal.csv", FILENAME_ASIGNACIONES.c_str()) != 0) throw std::runtime_error("Error al renombrar el archivo temporal de asignaciones.");   
}

ListaAsignaciones* AsignacionDataBase::cargarAsignacionesDesdeArchivo () {
// Cargar las asignaciones desde el archivo
    std::ifstream archivoAsignaciones(FILENAME_ASIGNACIONES);
    if (!archivoAsignaciones.is_open()) throw std::runtime_error("Error al abrir el archivo de asignaciones.");

// Crear una nueva lista para cargar las asignaciones desde el archivo
    ListaAsignaciones* nuevaLista = new ListaAsignaciones(); 
    std::string linea;
    int idTarea, idUsuario;

//leer cada línea del archivo y agregar la asignación a la nueva lista
    while (std::getline(archivoAsignaciones, linea)) {
        std::istringstream lineaActual(linea);
        std::string idTareaStr, idUsuarioStr; //variables para los id dentro del archivo

    // Leer idTarea y idUsuario desde la línea
        if (std::getline(lineaActual, idTareaStr, ',') && lineaActual >> idUsuario) { // Leer idUsuario
            idTarea = std::stoi(idTareaStr);
            idUsuario = std::stoi(idUsuarioStr);
        }

        nuevaLista->agregarAsignacion(idTarea, idUsuario); // Agregar la asignación a la nueva lista
    }
    return nuevaLista;
}

void AsignacionDataBase::agregarAsignacion(int idTarea, int idUsuario) { //agrega una asignación a la lista y al archivo

// Guardar la asignación en el archivo
    std::ofstream archivoAsignaciones(FILENAME_ASIGNACIONES, std::ios::app);
    if (!archivoAsignaciones.is_open()) throw std::runtime_error("Error al abrir el archivo de asignaciones.");
    archivoAsignaciones << idTarea << "," << idUsuario << std::endl;
    archivoAsignaciones.close();
} 

void AsignacionDataBase::eliminarAsignacion(int idTarea, int idUsuario) { //elimina una asignación de la lista y del archivo
    
    std::ifstream archivoAsignaciones(FILENAME_ASIGNACIONES); //archivo para leer las asignaciones hasta encontrar la que se desea eliminar
    if (!archivoAsignaciones.is_open()) throw std::runtime_error("Error al abrir el archivo de asignaciones.");

    std::ofstream archivoTemporal("temp.csv");
    if (!archivoTemporal.is_open()) throw std::runtime_error("Error al crear el archivo temporal.");

    std::string linea; //linea para leer el archivo y escribir en el archivo temporal las asignaciones excepto la que se desea eliminar
    while (std::getline(archivoAsignaciones, linea)) {
        
        std::stringstream lineaActual(linea);
        std::string idTareaArchivo, idUsuarioArchivo;
        std::getline(lineaActual, idTareaArchivo, ','); // Leer idTarea
        std::getline(lineaActual, idUsuarioArchivo); // Leer idUsuario

        if (idTareaArchivo != std::to_string(idTarea) || idUsuarioArchivo != std::to_string (idUsuario) ) {
            archivoTemporal << linea << std::endl;
        }
    }
    archivoAsignaciones.close();
    archivoTemporal.close();
// Reemplazar el archivo original con el temporal
    if (std::remove(FILENAME_ASIGNACIONES.c_str()) != 0) throw std::runtime_error("Error al eliminar el archivo original.");
    if (std::rename("temp.csv", FILENAME_ASIGNACIONES.c_str()) != 0) throw std::runtime_error("Error al renombrar el archivo temporal.");
}