#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include <vector>
#include <cstddef>
#include <utility>
#include "Icondicion.h"

template <typename T>
class BubbleSort {
private:

    void bubbleSort(std::vector<T>& arreglo, Icondicion<T>* condicion) {

        
        for (std::size_t i = 0; i < arreglo.size() - 1; i++) {

           
            for (std::size_t j = 0; j < arreglo.size() - 1 - i; j++) {

                
                if (!condicion->ordenar(arreglo[j], arreglo[j + 1])) {

                    
                    std::swap(arreglo[j], arreglo[j + 1]);
                }
            }
        }
    }

public:

    void sort(std::vector<T>& arreglo, Icondicion<T>* condicion) {

        
        if (arreglo.empty() || arreglo.size() <= 1 || condicion == nullptr)
            return;

       
        bubbleSort(arreglo, condicion);
    }
};

#endif 