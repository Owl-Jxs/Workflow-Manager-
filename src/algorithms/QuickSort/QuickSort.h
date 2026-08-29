#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "../Condiciones/ICondicion.h"
#include <vector>

template <typename T>
class QuickSort {

private:

    static int particionar (std::vector<T>& arreglo, int inicio, int fin, Icondicion<T>* condicion) {
        T pivote = arreglo[fin];
        int i = inicio - 1;

        for (int j = inicio; j < fin; j++) {
            if (condicion-> ordenar (arreglo[j], pivote)) {
                i++;
                T temporal = arreglo[i];
                arreglo[i] = arreglo[j];
                arreglo[j] = temporal;
            }
        }

        T temporal = arreglo[i + 1];
        arreglo[i + 1] = arreglo[fin];
        arreglo[fin] = temporal;
        return i + 1;
    }


    static void ordenarRecursivo(std::vector<T>& arreglo, int inicio, int fin, Icondicion<T>* condicion) {
        if (inicio < fin) {
            int posicionPivote = particionar(arreglo, inicio, fin, condicion);
            
            ordenarRecursivo(arreglo, inicio, posicionPivote - 1, condicion);
            ordenarRecursivo(arreglo, posicionPivote + 1,  fin, condicion);
        }
    }


public:

    static void ordenar (std::vector<T>& arreglo, Icondicion<T>* condicion) {
        if (arreglo.empty() || condicion == nullptr) return;
        
        ordenarRecursivo(arreglo, 0, arreglo.size () - 1, condicion);
    }
};

#endif