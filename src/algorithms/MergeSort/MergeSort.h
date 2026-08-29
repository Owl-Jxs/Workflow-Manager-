#ifndef MERGESORT_H
#define MERGESORT_H
#include <vector>
#include "../Condiciones/Icondicion.h"
#include <cstddef> // Para size_t

template <typename T>
class MergeSort {
private:
    void merge(std::vector<T>& vec, size_t left, size_t mid, size_t right, Icondicion<T>* condicion) {
        size_t n1 = mid - left + 1;
        size_t n2 = right - mid;

        std::vector<T> leftArr(n1);
        std::vector<T> rightArr(n2);

        for (size_t i = 0; i < n1; i++)
            leftArr[i] = vec[left + i];
        for (size_t j = 0; j < n2; j++)
            rightArr[j] = vec[mid + 1 + j];

        size_t i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {
            if (condicion->ordenar(leftArr[i], rightArr[j])) {
                vec[k] = leftArr[i];
                i++;
            } else {
                vec[k] = rightArr[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            vec[k] = leftArr[i];
            i++;
            k++;
        }

        while (j < n2) {
            vec[k] = rightArr[j];
            j++;
            k++;
        }
    }

    void mergeSort(std::vector<T>& vec, size_t left, size_t right, Icondicion<T>* condicion) {
        if (left >= right)
            return;

        size_t mid = left + (right - left) / 2;

        mergeSort(vec, left, mid, condicion);
        mergeSort(vec, mid + 1, right, condicion);
        merge(vec, left, mid, right, condicion);
    }

public:
    void sort(std::vector<T>& vector, Icondicion<T>* condicion) {
    // Protección contra vector vacío o puntero nulo
        if (vector.size() <= 1 || !condicion) return;
        
        mergeSort(vector, 0, vector.size() - 1, condicion);
    }
};


#endif