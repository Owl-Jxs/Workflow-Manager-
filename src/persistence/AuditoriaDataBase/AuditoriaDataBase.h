#ifndef AUDITORIADATABASE_H
#define AUDITORIADATABASE_H

#include <string>
#include <fstream>
#include <stdexcept>

// Bitacora transaccional inmutable (Aclaratoria 3, parte 2).
// Solo se le pueden agregar lineas; nunca borrar ni modificar.
class AuditoriaDataBase {
private:
    static const std::string FILENAME_AUDITORIA; // data/auditoria_log.csv

    // Formato de cada linea:
    // [Fecha y Hora] | [ID Usuario] | [Accion] | [ID Tarea]
    std::string formularLinea(const std::string& fechaHora, int idUsuario,
                              const std::string& accion, int idTarea) const;

public:
    AuditoriaDataBase();
    ~AuditoriaDataBase();

    // Registra una entrada con la fecha/hora actuales del sistema.
    void registrar(int idUsuario, const std::string& accion, int idTarea);

    // Registra una linea ya formada (control total por parte del llamador).
    void registrarLinea(const std::string& linea);

    // Fecha y hora actual en formato legible (YYYY-MM-DD HH:MM:SS).
    static std::string ahora();
};

#endif
