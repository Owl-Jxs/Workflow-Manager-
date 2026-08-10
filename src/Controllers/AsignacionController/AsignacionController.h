#ifndef  ASIGNACIONCONTROLLER_H
#define ASIGNACIONCONTROLLER_H

#include "../../persistence/AsignacionDataBase/AsignacionDataBase.h"
#include "../../structures/Asignacion/ListaAsignaciones/ListaAsignaciones.h"

class AsignacionController {
private:
    ListaAsignaciones* listaAsignaciones;
    AsignacionDataBase* archivosAsignaciones; 
public:
//constructores y destructor
    AsignacionController ();
    ~AsignacionController();

//manipular los archivos de datos
    void guardarAsignaciones();
    void cargarAsignaciones();

//manipular asignaciones individuales
    void agregarAsignacion(int idTarea, int idUsuario);
    void eliminarAsignacion(int idTarea, int idUsuario);
    bool buscarAsignacion(int idTarea, int idUsuario);


};

#endif