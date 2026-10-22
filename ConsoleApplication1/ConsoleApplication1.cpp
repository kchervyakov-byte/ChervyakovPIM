#define _CRT_SECURE_NO_WARNINGS // Разрешаем использовать scanf и fopen в Visual Studio
#include <stdlib.h> // Нужна для работы функции system
#include <stdio.h>
#include <math.h>

/*
 * Функция для вычисления корней квадратного уравнения.
 * Принимает коэффициенты уравнения и выводит результат на экран.
 */
void solveEquation(double a, double b, double c) {
    // Вычисляем дискриминант по формуле: D = b^2 - 4ac
    double discriminant = (b * b) - (4 * a * c);

    // Условный оператор для проверки знака дискриминанта
    if (discriminant > 0) {
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double x2 = (-b - sqrt(discriminant)) / (2 * a);
        printf("Два корня: x1 = %.2f, x2 = %.2f\n", x1, x2);
    }
    else if (discriminant == 0) {
        double x = -b / (2 * a);
        printf("Один корень: x = %.2f\n", x);
    }
    else {
        printf("Вещественных корней нет (дискриминант меньше нуля).\n");
    }
}

/*
 * Функция для варианта Б: чтение из файла "input.txt" и запись в "output.txt"
 */
void solveFromFile() {
    // 1. Открываем файл input.txt для чтения ("r" - read)
    FILE* fileIn = fopen("input.txt", "r");
    if (fileIn == NULL) {
        printf("Ошибка: не удалось открыть файл input.txt\n");
        return;
    }

    double a = 0, b = 0, c = 0;
    // Считываем три числа из файла с помощью fscanf (работает как scanf, но для файлов)
    fscanf(fileIn, "%lf %lf %lf", &a, &b, &c);
    fclose(fileIn); // Закрываем файл после чтения

    // Повторяем математику дискриминанта
    double discriminant = (b * b) - (4 * a * c);

    // 2. Открываем (или создаем) файл output.txt для записи ("w" - write)
    FILE* fileOut = fopen("output.txt", "w");
    if (fileOut == NULL) {
        printf("Ошибка: не удалось создать файл output.txt\n");
        return;
    }

    // Записываем результат в файл с помощью fprintf (работает как printf, но пишет в файл)
    if (discriminant > 0) {
        double x1 = (-b + sqrt(discriminant)) / (2 * a);
        double x2 = (-b - sqrt(discriminant)) / (2 * a);
        fprintf(fileOut, "x1 = %.2f, x2 = %.2f\n", x1, x2);
    }
    else if (discriminant == 0) {
        double x = -b / (2 * a);
        fprintf(fileOut, "x = %.2f\n", x);
    }
    else {
        fprintf(fileOut, "Вещественных корней нет.\n");
    }

    fclose(fileOut); // Закрываем файл после записи
    printf("Готово! Результат сохранен в файл output.txt\n");
}

/*
 * Главная функция программы
 */
int main() {
    system("chcp 1251 > nul"); // Настройка русского языка

    int userChoice = 0;
    printf("Выберите режим работы:\n");
    printf("1 - Консоль (Ввод с клавиатуры)\n");
    printf("2 - Файлы (Чтение из input.txt)\n");
    printf("Ваш выбор: ");
    scanf("%d", &userChoice);

    if (userChoice == 1) {
        double coefA = 0, coefB = 0, coefC = 0;
        printf("\nВведите коэффициенты a, b и c через пробел: ");
        scanf("%lf %lf %lf", &coefA, &coefB, &coefC);
        solveEquation(coefA, coefB, coefC);
    }
    else if (userChoice == 2) {
        solveFromFile();
    }
    else {
        printf("Неверный режим работы.\n");
    }

    return 0;
}
