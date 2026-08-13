#include "AsignacionDataBase.h"
#include <iostream>
const std::string AsignacionDataBase::FILENAME_ASIGNACIONES = "data/asignaciones.csv"; // Nombre del archivo para almacenar las asignaciones
// Formato de guardado ----> idTarea, idUsuario

AsignacionDataBase::AsignacionDataBase() { }

AsignacionDataBase::~AsignacionDataBase() { }

void AsignacionDataBase::guardarAsignacionesEnArchivo(ListaAsignaciones* lista) { //guarda todas las asignaciones de la lista en el archivo
    if (lista == nullptr) throw std::invalid_argument("La lista de asignaciones es nula");

    std::ofstream archivoTemporalAsignaciones("data/asignaciones_temporal.csv");
    if (!archivoTemporalAsignaciones.is_open()) throw std::runtime_error("Error al abrir el archivo temporal de asignaciones.");

// Guardar todas las asignaciones de la lista en el archivo temporal
    NodoAsignacion* actual = lista->getCabeza(); //primer nodo de la lista
    while (actual != nullptr) {
        archivoTemporalAsignaciones << actual->idTarea << "," << actual->idUsuario << std::endl;
        actual = actual->siguiente;
    }
    archivoTemporalAsignaciones.close(); //cerrar el archivo temporal

// Reemplazar el archivo original con el temporal
    std::remove(FILENAME_ASIGNACIONES.c_str()); // si el archivo no existia, no es un error
    if (std::rename("data/asignaciones_temporal.csv", FILENAME_ASIGNACIONES.c_str()) != 0) throw std::runtime_error("Error al renombrar el archivo temporal de asignaciones.");
}

ListaAsignaciones* AsignacionDataBase::cargarAsignacionesDesdeArchivo () {
// Cargar las asignaciones desde el archivo
    std::ifstream archivoAsignaciones(FILENAME_ASIGNACIONES);
    if (!archivoAsignaciones.is_open()) return new ListaAsignaciones(); // archivo no existe, retorna lista vacia

// Crear una nueva lista para cargar las asignaciones desde el archivo
    ListaAsignaciones* nuevaLista = new ListaAsignaciones();
    std::string linea;

//leer cada línea del archivo y agregar la asignación a la nueva lista
    while (std::getline(archivoAsignaciones, linea)) {
        if (linea.empty()) continue;

        try {
            std::istringstream lineaActual(linea);
            std::string idTareaStr, idUsuarioStr;

            if (std::getline(lineaActual, idTareaStr, ',') && std::getline(lineaActual, idUsuarioStr)) {
                int idTarea = std::stoi(idTareaStr);
                int idUsuario = std::stoi(idUsuarioStr);
                nuevaLista->agregarAsignacion(idTarea, idUsuario);
            } else {
                std::cerr << "Advertencia: linea con formato invalido en asignaciones.csv: " << linea << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Advertencia: error al procesar linea de asignaciones.csv: " << e.what() << std::endl;
        }
    }
    archivoAsignaciones.close();
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

    std::ofstream archivoTemporal("data/asignaciones_temporal2.csv");
    if (!archivoTemporal.is_open()) throw std::runtime_error("Error al crear el archivo temporal.");

    std::string linea; //linea para leer el archivo y escribir en el archivo temporal las asignaciones excepto la que se desea eliminar
    bool encontrada = false;
    while (std::getline(archivoAsignaciones, linea)) {
        
        std::stringstream lineaActual(linea);
        std::string idTareaArchivo, idUsuarioArchivo;
        std::getline(lineaActual, idTareaArchivo, ','); // Leer idTarea
        std::getline(lineaActual, idUsuarioArchivo); // Leer idUsuario

        if (idTareaArchivo != std::to_string(idTarea) || idUsuarioArchivo != std::to_string (idUsuario) ) {
            archivoTemporal << linea << std::endl;
        } else {
            encontrada = true;
        }
    }
    archivoAsignaciones.close();
    archivoTemporal.close();

    if (!encontrada) {
        std::remove("data/asignaciones_temporal2.csv");
        throw std::runtime_error("Error: No se encontro la asignación especificada para eliminar.");
    }

// Reemplazar el archivo original con el temporal
    std::remove(FILENAME_ASIGNACIONES.c_str()); // si el archivo no existia, no es un error
    if (std::rename("data/asignaciones_temporal2.csv", FILENAME_ASIGNACIONES.c_str()) != 0) throw std::runtime_error("Error al renombrar el archivo temporal.");
}