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


void ValidarEntrada::convertirMayusculas (std::string &palabra) {
    bool ultimoCharEspacio = false; //para cuando pasamos un espacio

    palabra[0] = std::toupper (palabra[0]);
    for (char c : palabra) {
        if (c == ' ') {
            ultimoCharEspacio = true;
        }
        if (ultimoCharEspacio) {
            c = std::toupper (c); ultimoCharEspacio = false;
        }
    }
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
    if (!contieneSoloCaracteres (entrada, 1) ) {
        throw std::invalid_argument ("--- La entrada debe contener caracteres y " + std::to_string(cantidadEspacios) + " espacio maximo ---");
    }
}

//               === === === === === === === === === === ===  PUBLIC  === === === === === === === === === === === === === === 
int ValidarEntrada::validarEntradaRango (const std::string& mensaje,int inicio, int fin) {
    std::string entradaTemporal;
    bool entradaCorrecta = false;
    int entradavalidada;

    while (!entradaCorrecta) {
        int entradaConvertida; //la entrada pasada de string a int
    //leemos la entrada
        std::cout << mensaje << std::endl;    std::getline (std::cin,  entradaTemporal);
    
    //la procesamos
        try {
            entradaConvertida = leerEntradaNumerica (entradaTemporal, ( (inicio < 0) ? true : false) );
        } catch (std::exception &e) {
            e.what (); std::system ("cls");    continue;
        }

    //validamos que este en el rango correcto
        if (entradaConvertida < inicio || entradaConvertida > fin) { 
            std::cout << "\n--- Ingrese una entrada dentro del rango ---" << std::endl;     std::system ("cls");    continue;
        }
        
    //le entregamos la entrada correcta a la que hara el return y salimos del while
        entradaCorrecta = entradaConvertida;      entradaCorrecta = true;
    }
    return entradaCorrecta;
}

int ValidarEntrada::validarCodigoNumerico (const std::string& mensaje, int cantidadDigitos) {
    std::string entradaTemporal;
    bool entradaCorrecta = false;
    int entradavalidada;

    while (!entradaCorrecta) {
        int entradaConvertida; //la entrada pasada de string a int    
    //leemos la entrada
        std::cout << mensaje << std::endl;    std::getline (std::cin,  entradaTemporal);
   
       if (entradaTemporal.size () != cantidadDigitos) { 
        std::cout << "Ingrese la cantidad de digitos correctos [" << cantidadDigitos << "]" <<std::endl; 
        std::system ("cls"); continue;
       } 

        try {    //la procesamos
            entradaConvertida = leerEntradaNumerica (entradaTemporal, false);
        } catch (std::exception &e) {
            e.what (); std::system ("cls");    continue;
        }

    //le entregamos la entrada correcta a la que hara el return y salimos del while
        entradaCorrecta = entradaConvertida;      entradaCorrecta = true;
    }
    return entradaCorrecta;
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
    bool contrasenaValida = false;

    while (!contrasenaValida) {
        std::cout << mensaje << std::endl; 
        bool seguirEscribiendo = true;

        while (seguirEscribiendo) {
            char c = _getch ();

            if (c == '\r') {
                seguirEscribiendo = true; //tecla enter
            } else if (c == '\b' || c == 127) { //si el usuario borra un caracter
                if (!entrada.empty () ) {
                    entrada.pop_back ();    std::cout << "\b \b";
                }
            } else {
                entrada.push_back (c);
                std::cout << mascara;
            }   
        }
         contrasenaValida = true;
    }
    return entrada;
}
