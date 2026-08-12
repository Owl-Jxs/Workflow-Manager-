#ifndef TAREA_H
#define TAREA_H
#include <string>
#include <stdexcept>

class Tarea {
private:
    int idTarea;
    std::string descripcionTarea;
    bool prioridadTarea;      // true = ALTA, false = NORMAL
    std::string estadoTarea;    // "POR HACER", "EN PROCESO", "COMPLETADA"

//atributos relacionados con las subtareas
    int idTareaPadre;             // id del padre
    Tarea* primerSubTarea;      // primer hijo
    Tarea* siguienteSubTarea;   // hermano siguiente
    int cantidadSubTareas;
    Tarea* TareaPadre; // puntero al padre
//funciones privadas de validar parametros
    void validarId (int id);
    void validarDescripcion (std::string descripcion);
    void validarEstado (std::string estado);
   void esAncestro (const Tarea* buscado) const; // Verifica si 'buscado' es un ancestro de 'this'
public:
//constructores y destructores
    Tarea (int id, std::string descripcion, bool prioridad, std::string estado);
    ~Tarea ();

//variables static
    static const std::string ESTADO [3];
    static const int sinPadre;

//getters and setters
    void setIdTarea (int id);
    int getIdTarea () const;

    void setDescripcionTarea (std::string descripcion);
    std::string getDescripcionTarea () const;

    void setPrioridad (bool prioridad);
    bool getPrioridad () const;

    void setEstado (std::string estado);
    std::string getEstado () const;

    void agregarSubTarea (Tarea* subTarea);
    Tarea* getPrimerSubTarea () const;

    void setSiguienteSubTarea (Tarea* siguiente);
    Tarea* getSiguienteSubTarea () const;
    
    void setIdPadre (int id);
    int getIdPadre () const;
    
    void setTareaPadre (Tarea* padre);
    Tarea* getTareaPadre () const;
    int getCantidadSubTareas () const;

// busca una subTarea
    Tarea* buscarSubTarea (int idTarea);
};



#endif