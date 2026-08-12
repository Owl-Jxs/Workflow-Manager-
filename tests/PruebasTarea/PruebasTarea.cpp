#include "PruebasTarea.h"

#include "../Controllers/TareaController/TareaController.h"
#include "../models/Tarea/Tarea.h"
#include "../persistence/TareaDataBase/TareaDataBase.h"
#include "../structures/ColaFIFO/ColaFIFO.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
    void verificar(bool condicion, const std::string& mensaje) {
        if (!condicion) {
            throw std::runtime_error("Prueba fallida: " + mensaje);
        }
    }

    template <typename Funcion>
    void verificarQueLanza(Funcion funcion, const std::string& mensaje) {
        try {
            funcion();
        } catch (const std::exception&) {
            return;
        }

        throw std::runtime_error("Prueba fallida: " + mensaje);
    }

    class RespaldoArchivosTareas {
    private:
        struct Archivo {
            std::string ruta;
            bool existia;
            std::string contenido;
        };

        std::vector<Archivo> archivos;

        static std::string leerArchivo(const std::string& ruta) {
            std::ifstream entrada(ruta, std::ios::binary);
            return std::string(
                std::istreambuf_iterator<char>(entrada),
                std::istreambuf_iterator<char>()
            );
        }

        static void escribirArchivo(const std::string& ruta, const std::string& contenido) {
            std::ofstream salida(ruta, std::ios::binary | std::ios::trunc);
            if (!salida.is_open()) {
                throw std::runtime_error("No se pudo escribir el archivo de prueba: " + ruta);
            }
            salida << contenido;
        }

    public:
        RespaldoArchivosTareas() {
            std::filesystem::create_directories("data");

            const std::vector<std::string> rutas = {
                "data/tareas_regulares.csv",
                "data/tareas_urgentes.csv"
            };

            for (const std::string& ruta : rutas) {
                Archivo archivo;
                archivo.ruta = ruta;
                archivo.existia = std::filesystem::exists(ruta);
                archivo.contenido = archivo.existia ? leerArchivo(ruta) : "";
                archivos.push_back(archivo);

                escribirArchivo(ruta, "");
            }
        }

        ~RespaldoArchivosTareas() {
            for (const Archivo& archivo : archivos) {
                try {
                    if (archivo.existia) {
                        escribirArchivo(archivo.ruta, archivo.contenido);
                    } else {
                        std::remove(archivo.ruta.c_str());
                    }
                } catch (...) {
                }
            }
        }
    };

    void imprimirOk(const std::string& nombrePrueba) {
        std::cout << "[OK] " << nombrePrueba << std::endl;
    }
}