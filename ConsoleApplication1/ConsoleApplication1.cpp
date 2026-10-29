#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <omp.h> // Поддержка OpenMP

using namespace std;

// Функция для обработки массива (Задание 1 и 2)
void processArrayParallel(const vector<double>& A, const vector<double>& B, vector<double>& C, int size) {
    // Вычисления разделяются между потоками процессора
#pragma omp parallel for
    for (int i = 0; i < size; i++) {
        C[i] = sqrt(abs(A[i])) + pow(B[i], 2); // Пример математической обработки
    }
}

int main() {
    // Настройка русского языка в консоли
    setlocale(LC_ALL, "Russian");

    int userChoice = 0;
    cout << "Выберите режим работы:\n";
    cout << "1 - Консоль (Ввод данных с клавиатуры)\n";
    cout << "2 - Файл (Чтение из inputt.txt)\n";
    cout << "Ваш выбор: ";
    cin >> userChoice;

    if (userChoice == 1) {
        // ================= ЗАДАНИЕ 1: КОНСОЛЬНЫЙ ВВОД =================
        int size;
        cout << "Введите размер массивов: ";
        cin >> size;

        if (size <= 0) {
            cout << "Некорректный размер!" << endl;
            return 1;
        }

        vector<double> A(size);
        vector<double> B(size);
        vector<double> C(size);

        cout << "Введите элементы массива A (" << size << " шт.): ";
        for (int i = 0; i < size; i++) {
            cin >> A[i];
        }

        cout << "Введите элементы массива B (" << size << " шт.): ";
        for (int i = 0; i < size; i++) {
            cin >> B[i];
        }

        double startTime = omp_get_wtime();
        processArrayParallel(A, B, C, size);
        double endTime = omp_get_wtime();

        cout << "\n--- Результаты (Консоль) ---" << endl;
        cout << "Массив C (Результат): ";
        for (int i = 0; i < size; i++) {
            cout << C[i] << " ";
        }
        cout << "\nВремя параллельного расчета: " << (endTime - startTime) << " сек." << endl;
        cout << "Использовано потоков: " << omp_get_max_threads() << endl;

    }
    else if (userChoice == 2) {
        // ================= ЗАДАНИЕ 2: РАБОТА С ФАЙЛОМ =================
        ifstream inFile("inputt.txt");
        if (!inFile.is_open()) {
            cout << "Ошибка! Не удалось открыть файл inputt.txt" << endl;
            cout << "Создайте файл inputt.txt в папке с проектом (там где ConsoleApplication1.cpp)." << endl;
            return 1;
        }

        int size;
        if (!(inFile >> size) || size <= 0) {
            cout << "Ошибка чтения размера массива из файла!" << endl;
            return 1;
        }

        vector<double> A(size);
        vector<double> B(size);
        vector<double> C(size);

        // Читаем данные для массива A
        for (int i = 0; i < size; i++) {
            if (!(inFile >> A[i])) {
                cout << "Ошибка чтения элементов массива A!" << endl;
                return 1;
            }
        }

        // Читаем данные для массива B
        for (int i = 0; i < size; i++) {
            if (!(inFile >> B[i])) {
                cout << "Ошибка чтения элементов массива B!" << endl;
                return 1;
            }
        }
        inFile.close();

        cout << "Данные из inputt.txt успешно прочитаны. Размер: " << size << endl;

        double startTime = omp_get_wtime();
        processArrayParallel(A, B, C, size);
        double endTime = omp_get_wtime();

        // Записываем результат в выходной файл
        ofstream outFile("outputt.txt");
        if (!outFile.is_open()) {
            cout << "Ошибка создания файла output.txt!" << endl;
            return 1;
        }

        outFile << "Размер: " << size << "\n";
        outFile << "Массив C (Результат параллельного расчета):\n";
        for (int i = 0; i < size; i++) {
            outFile << C[i] << " ";
        }
        outFile << "\n\nВремя расчета: " << (endTime - startTime) << " сек.\n";
        outFile << "Потоков процессора: " << omp_get_max_threads() << "\n";
        outFile.close();

        cout << "\n--- Результаты (Файл) ---" << endl;
        cout << "Параллельный расчет окончен." << endl;
        cout << "Результаты успешно сохранены в файл: outputt.txt" << endl;
        cout << "Время параллельного расчета: " << (endTime - startTime) << " сек." << endl;
    }
    else {
        cout << "Неверный режим работы!" << endl;
    }

    return 0;
}
