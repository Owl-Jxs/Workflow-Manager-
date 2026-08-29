#include "ValidarEntrada.h"
bool ValidarEntrada::contieneSoloCaracteres (std::string entrada, int cantidadEspacios) {
    int cantidadActualEspacios = 0; 
    for (char c : entrada) {
        if (c == ' ' && cantidadActualEspacios < cantidadEspacios) {
            cantidadActualEspacios++; continue;
        }
        if (!isalpha (c)) return false;
    }
    return true;
}

bool ValidarEntrada::contieneSoloDigitos (std::string entrada, bool permitirNegativos) {
    bool primeraLetra = true;
    for (char c : entrada) {
        if (permitirNegativos && primeraLetra && c == '-') {
            primeraLetra = false;   continue; 
        }
        if (!isdigit (c) ) return false;
    }
    return true;
}


int ValidarEntrada::leerEntradaNumerica (const std::string& entrada, bool permitirNegativos) {
//validamos que no este vacia ni contenga caracteres
    if (entrada.empty () || entrada ==  " ") throw std::invalid_argument ("---La entrada no puede estar vacia ---"); 

    if (!contieneSoloDigitos (entrada, permitirNegativos)) { 
        throw std::invalid_argument  ("--- La entrada debe contener solo digitos ---");    
    }   
    return std::stoi (entrada);
}

void ValidarEntrada::leerEntradaTextual (const std::string& entrada,int cantidadEspacios) {
//validamos que no este vacia y que solo contenga letras
    if (entrada.empty () || entrada ==  " ")  throw std::invalid_argument ("--- Ingrese una entrada no vacia ---");
    if (!contieneSoloCaracteres (entrada, cantidadEspacios) ) {
        throw std::invalid_argument ("--- La entrada debe contener caracteres y " + std::to_string(cantidadEspacios) + " espacio maximo ---");
    }
}

//               === === === === === === === === === === ===  PUBLIC  === === === === === === === === === === === === === === 
int ValidarEntrada::validarEntradaRango (const std::string& mensaje,int inicio, int fin) {
    std::string entradaTemporal;

    while (true) {
    //leemos la entrada
        std::cout << mensaje << std::endl;    std::getline (std::cin,  entradaTemporal);

    //la procesamos
        int entradaConvertida; //la entrada pasada de string a int
        try {
            entradaConvertida = leerEntradaNumerica (entradaTemporal, ( (inicio < 0) ? true : false) );
        } catch (std::exception&) {
            std::system ("cls");    continue;
        }

    //validamos que este en el rango correcto
        if (entradaConvertida < inicio || entradaConvertida > fin) { 
            std::cout << "\n--- Ingrese una entrada dentro del rango ---" << std::endl;     std::system ("cls");    continue;
        }

    //devolvemos la entrada ya validada
        std::system ("cls"); return entradaConvertida;
    }
}

int ValidarEntrada::validarCodigoNumerico (const std::string& mensaje, int cantidadDigitos) {
    std::string entradaTemporal;

    while (true) {
    //leemos la entrada
        std::cout << mensaje << std::endl;    std::getline (std::cin,  entradaTemporal);

    //verificamos la cantidad de digitos
       if (entradaTemporal.size () != cantidadDigitos) { 
        std::cout << "Ingrese la cantidad de digitos correctos [" << cantidadDigitos << "]" <<std::endl; 
        std::system ("cls"); continue;
       } 

    //la procesamos
        try {
            return leerEntradaNumerica (entradaTemporal, false);
        } catch (std::exception&) {
            std::system ("cls");    continue;
        }
    }
}

std::string ValidarEntrada::validarNombreCompleto () {
    std::stringstream nombreCompleto;
    bool nombreValidado = false;
    while (!nombreValidado) {
        try {
            std::string nombre, apellidos;
            std::cout << "Ingrese sus apellidos" << std::endl; std::getline (std::cin, apellidos);
            leerEntradaTextual (apellidos, 1); //verificamos entrada 

            std::cout << "Ingrese su nombre/nombres" << std::endl; std::getline (std::cin, nombre);
            leerEntradaTextual (nombre, 1);

            nombreCompleto << nombre << " " << apellidos;
            nombreValidado = true;
        } catch (std::exception &e) {
            e.what (); continue;
        }
    }
    return nombreCompleto.str ();
}

std::string ValidarEntrada::validarContrasena (const std::string& mensaje, char mascara) {
    std::string entrada = "";
    std::cout << mensaje << std::endl;
    while (true) { //miientras se quiera escribir un caracter 
        char c = _getch ();
        if (c == '\r') {
            break;
        } else if (c == '\b' || c == 127) {
            if (!entrada.empty ()) {
                entrada.pop_back ();
                std::cout << "\b \b";
            }
        } else {
            entrada.push_back (c);
            std::cout << mascara;
        }   
    }
    std::cout << "\n";
    return entrada;
}

bool ValidarEntrada::respuestas_Si_O_No (std::string mensajeSi, std::string mensajeNo) {
std::cout << "1." << mensajeSi << std::endl
    << "2." << mensajeNo << std::endl << std::endl;

int opcion =validarEntradaRango ("ingrese su opcion", 1,2);

if (opcion == 1) return true;
return false;
}

// Retorna el string validado usando 'int' para el máximo de caracteres
std::string ValidarEntrada::ingresarDescripcionCorta(const std::string& mensaje, int maxCaracteres) {
    std::string entrada;
    bool esValida = false;

    do {
        std::cout << mensaje;
        std::getline(std::cin, entrada);

        // entrada.length() se convierte explícitamente a int para comparar
        int longitudActual = static_cast<int>(entrada.length());

        if (entrada.empty()) {
            std::cout << "Error: La descripción no puede estar vacía. Intente de nuevo.\n";  std::system ("pause");
        } else if (longitudActual > maxCaracteres) {
            std::cout << "Error: Excede el límite de " << maxCaracteres << " caracteres (longitud actual: " 
                      << longitudActual << "). Intente de nuevo.\n";  std::system ("pause");
        } else {
            esValida = true;
        }
        std::system ("cls");
    } while (!esValida);

    return entrada;
}