#ifndef QUICKSORT_H
#define QUICKSORT_H
#include "QuickSort/Condiciones/ICondicion.h"

template <typename T>
class QuickSort {
private:
    static int particionar(T** arreglo, int inicio, int fin, ICondicion<T>* condicion);
    static void ordenarRecursivo(T** arreglo, int inicio, int fin, ICondicion<T>* condicion);

public:
    static void ordenar(T** arreglo, int cantidad, ICondicion<T>* condicion);
};

#endif