#ifndef USUARIO_H
#define USUARIO_H

#include <string>

static const std::string ROLES [2] = {
    "ADMIN",
    "USER"
};

class Usuario {
private:
    int idUsuario;
    std::string nombreUsuario;
    std::string rolUsuario; // "ADMIN", "USER"

public:
	//constructores y destructores
    Usuario();
    Usuario(int id, std::string nombre, std::string rol);
    ~Usuario();
//getters and setters
    void setId(int id);
    int getId() const;

    void setNombre(std::string nombre);
    std::string getNombre() const;

    void setRol(std::string rol);
    std::string getRol() const;

//conexion con el archivo CSV
    std::string toCSV() const;
    static Usuario fromCSV(const std::string& linea);
};



#endif
