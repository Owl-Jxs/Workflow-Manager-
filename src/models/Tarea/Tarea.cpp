#include "Tarea.h"
///constructors and destructors
Tarea::Tarea (int id, std::string descripcion, bool isUrgente) {
    this->idTarea = id;
    this->descripcionTarea = descripcion;
    this->completada = false;
    this->urgente = isUrgente;
    this->subTarea = nullptr;
    this->siguienteSubTarea = nullptr;
    this-> cantidadSubTareas = 0;
}

Tarea::~Tarea () {
    if (this->subTarea != nullptr)      delete this->subTarea;
    if (this->siguienteSubTarea != nullptr)     delete this->siguienteSubTarea;
    
}

///getters and setters
void Tarea::setIdTarea (int id) {
    this->idTarea = id;
}

int Tarea::getIdTarea () const {
    return this->idTarea;
}

void Tarea::setDescripcionTarea (std::string descripcion) {
    this->descripcionTarea = descripcion;
}

std::string Tarea::getDescripcionTarea () const {
    return this->descripcionTarea;
}

void Tarea::setCompletada (bool completada) {
    this->completada = completada;
}

bool Tarea::isCompletada () const {
    return this->completada;
}

void Tarea::setUrgente (bool urgente) {
    this->urgente = urgente;
}

bool Tarea::isUrgente () const {
    return this->urgente;
}

void Tarea::agregarSubTarea (Tarea* _subTarea) {
    if (this->subTarea == nullptr) {
        this->subTarea = _subTarea;
    } else {
        this->subTarea->setSiguienteSubTarea(_subTarea);
    }
}
Tarea* Tarea::getSubTarea () const {
    return this->subTarea;
}

void Tarea::setSiguienteSubTarea (Tarea* siguienteSubTarea) {
    this->siguienteSubTarea = siguienteSubTarea;
}

Tarea* Tarea::getSiguienteSubTarea () const {
    return this->siguienteSubTarea;
}