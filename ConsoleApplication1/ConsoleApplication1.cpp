#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_VECTORS 6
#define DIM 3

// 1. Функция вычисления скалярного произведения двух векторов
double dot_product(const double a[], const double b[], int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += a[i] * b[i];
    }
    return sum;
}

int main(void) {
    // Инициализация генератора случайных чисел текущим временем
    srand((unsigned int)time(NULL));

    double vectors[NUM_VECTORS][DIM];

    // 2. Генерация 6 случайных векторов (координаты от -10.0 до 10.0)
    for (int i = 0; i < NUM_VECTORS; i++) {
        for (int j = 0; j < DIM; j++) {
            vectors[i][j] = ((double)rand() / RAND_MAX) * 20.0 - 10.0;
        }
    }

    // 3. Открытие файла для записи результата
    FILE* file = fopen("result.txt", "w");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        return 1;
    }

    // Записываем сгенерированные векторы в файл для наглядности
    fprintf(file, "Сгенерированные векторы:\n");
    for (int i = 0; i < NUM_VECTORS; i++) {
        fprintf(file, "Вектор %d: (", i + 1);
        for (int j = 0; j < DIM; j++) {
            fprintf(file, "%.2f%s", vectors[i][j], (j < DIM - 1) ? ", " : "");
        }
        fprintf(file, ")\n");
    }

    fprintf(file, "\nПопарные скалярные произведения:\n");

    // Перебор всех уникальных пар векторов (i, j), где j > i
    for (int i = 0; i < NUM_VECTORS; i++) {
        for (int j = i + 1; j < NUM_VECTORS; j++) {
            double prod = dot_product(vectors[i], vectors[j], DIM);
            fprintf(file, "Вектор %d * Вектор %d = %.2f\n", i + 1, j + 1, prod);
        }
    }

    // Закрытие файла и освобождение ресурсов
    fclose(file);
    printf("Готово! Результаты сохранены в файл result.txt\n");

    return 0;
}
