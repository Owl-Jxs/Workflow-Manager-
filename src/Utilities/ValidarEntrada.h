#ifndef VALIDARENTRADA_H
#define VALIDARENTRADA_H
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <conio.h>

class ValidarEntrada {
private: 
    static bool contieneSoloCaracteres (std::string entrada, int cantidadEspacios);
    static bool contieneSoloDigitos (std::string entrada, bool permitirNegativos);
    static void convertirMayusculas (std::string &palabra);

    static int leerEntradaNumerica (const std::string& entrada, bool permitirNegativos);
    static void leerEntradaTextual (const std::string& entrada, int cantidadEspacios);

public: 
    ValidarEntrada () {}
    ~ValidarEntrada () {}
    static int validarEntradaRango (const std::string& mensaje, int inicio, int fin);
    static int validarCodigoNumerico (const std::string& mensaje, int cantidadDigitos);
    static std::string validarNombreCompleto ();
    static std::string validarContrasena (const std::string& mensaje, char mascara);
    static bool respuestas_Si_O_No (std::string mensajeSi, std::string mensajeNo);
    static std::string ingresarDescripcionCorta (const std::string& mensaje, int maxCaracteres);

};

#endif