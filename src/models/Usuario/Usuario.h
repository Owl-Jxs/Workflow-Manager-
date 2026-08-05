#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class Usuario {
private:
    int idUsuario;
    std::string nombreUsuario;
    std::string rolUsuario;

public:
    // Constantes de rol integradas
    static inline const std::string ROL_ADMIN = "ADMIN";
    static inline const std::string ROL_USER = "USER";

    // Constructores y destructor
    Usuario();
    Usuario(int id, const std::string& nombre, const std::string& rol);
    ~Usuario();

    // Getters y Setters
    void setId(int id);
    int getId() const;

    void setNombre(const std::string& nombre);
    std::string getNombre() const;

    void setRol(const std::string& rol);
    std::string getRol() const;
};

#endif
