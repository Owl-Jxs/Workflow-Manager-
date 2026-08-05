#ifndef TAREA_H
#define TAREA_H

#include <string>

static const std::string PRIORIDAD [2] = {
    "ALTA",
    "NORMAL"
};

static const std::string ESTADO [2] = {
    "PENDIENTE",
    "COMPLETADA"
};

// [Actualizado]
//  - prioridad (ALTA/NORMAL) y estado (PENDIENTE/COMPLETADA)
//  - idUsuarioResponsable (usuario asignado por id)
//  - padre + primerSubTarea + siguienteSubTarea (arbol general de subtareas)
//  - toCSV()/fromCSV() para la conexion con los archivos .csv

class Tarea {
private:
    int idTarea;
    std::string descripcionTarea;
    //ListaUsuarios* Encargados; <- Lista con los encargados de la tarea (Se ancla una lista de usuarios?)
    std::string prioridadTarea; // "ALTA", "NORMAL"
    std::string estadoTarea;    // "PENDIENTE", "COMPLETADA"
    int idUsuarioResponsable;   // conexion con Usuario
    int idPadre;                // id de la tarea padre (0 si es raiz)
    Tarea* padre;               // nullptr si es raiz
    Tarea* primerSubTarea;      // primer hijo
    Tarea* siguienteSubTarea;   // hermano siguiente
    int cantidadSubTareas;

public:
//constructores y destructores
    Tarea(int id, std::string descripcion, std::string prioridad, std::string estado, int idResponsable, int padreId = 0);
    ~Tarea();
//getters and setters
    void setIdTarea(int id);
    int getIdTarea() const;

    void setDescripcionTarea(std::string descripcion);
    std::string getDescripcionTarea() const;

    void setPrioridad(std::string prioridad);
    std::string getPrioridad() const;

    void setEstado(std::string estado);
    std::string getEstado() const;

    void setIdUsuarioResponsable(int idResponsable);
    int getIdUsuarioResponsable() const;

    void setPadre(Tarea* padreTarea);
    Tarea* getPadre() const;
    int getPadreId() const;

    void agregarSubTarea(Tarea* subTarea);
    Tarea* getPrimerSubTarea() const;

    void setSiguienteSubTarea(Tarea* siguiente);
    Tarea* getSiguienteSubTarea() const;

    int getCantidadSubTareas() const;

//conexion con el archivo CSV
    std::string toCSV() const;
    static Tarea fromCSV(const std::string& linea);
};



#endif
