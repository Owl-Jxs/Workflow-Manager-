#include "QuickSort.h"
#include "../models/Tarea/Tarea.h"
#include "../models/Usuario/Usuario.h"

//particionar
template <typename T>
int QuickSort<T>::particionar(T** arreglo, int inicio, int fin, ICondicion<T>* condicion) {
    T* pivote = arreglo[fin];
    int i = inicio - 1;
    for (int j = inicio; j < fin; j++) {
        if (condicion->comparar(*arreglo[j], *pivote)) {
            i++;
            T* temporal = arreglo[i];
            arreglo[i] = arreglo[j];
            arreglo[j] = temporal;
        }
    }
    T* temporal = arreglo[i + 1];
    arreglo[i + 1] = arreglo[fin];
    arreglo[fin] = temporal;
    return i + 1;
}

//ordenamiento recursivo
template <typename T>
void QuickSort<T>::ordenarRecursivo(T** arreglo, int inicio, int fin, ICondicion<T>* condicion) {
    if (inicio < fin) {
        int posicionPivote = particionar(arreglo, inicio, fin, condicion);
        ordenarRecursivo(arreglo, inicio, posicionPivote - 1, condicion);
        ordenarRecursivo(arreglo, posicionPivote + 1, fin, condicion);
    }
}

//ordenar
template <typename T>
void QuickSort<T>::ordenar(T** arreglo, int cantidad, ICondicion<T>* condicion) {
    if (arreglo == nullptr || cantidad <= 1 || condicion == nullptr) {
        return;
    }
    ordenarRecursivo(arreglo, 0, cantidad - 1, condicion);
}


//template para ambos tipos
template class QuickSort<Tarea>;
template class QuickSort<Usuario>;