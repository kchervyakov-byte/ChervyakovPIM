#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <omp.h>

using namespace std;

// --- Функция для Задания 3: Расчет характеристик на заданном количестве потоков ---
void calculateStatsOMP(const vector<double>& data, int num_threads, double& mean, double& std_dev, double& exec_time) {
    int n = (int)data.size();

    // Устанавливаем нужное количество потоков для этого расчета
    omp_set_num_threads(num_threads);

    double start_time = omp_get_wtime();

    // 1. Параллельный расчет суммы (для среднего арифметического)
    double sum = 0.0;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        sum += data[i];
    }
    mean = sum / n;

    // 2. Параллельный расчет суммы квадратов разностей (для среднеквадратического отклонения)
    double variance_sum = 0.0;
#pragma omp parallel for reduction(+:variance_sum)
    for (int i = 0; i < n; i++) {
        double diff = data[i] - mean;
        variance_sum += diff * diff; // Умножение работает быстрее и стабильнее pow() в OMP
    }
    std_dev = sqrt(variance_sum / n);

    double end_time = omp_get_wtime();
    exec_time = end_time - start_time;
}

int main() {
    setlocale(LC_ALL, "Russian");

    int userChoice = 0;
    cout << "Выберите задание:\n";
    cout << "1 - Задание 2: Сортировка массива\n";
    cout << "2 - Задание 3: Статистика из файла (сравнение 1, 2, 4 потоков OMP)\n";
    cout << "Ваш выбор: ";
    cin >> userChoice;

    if (userChoice == 1) {
        // ================= ЗАДАНИЕ 2: СОРТИРОВКА =================
        int size;
        cout << "Введите размер массива: ";
        cin >> size;

        if (size <= 0) {
            cout << "Некорректный размер!" << endl;
            return 1;
        }

        vector<double> arr(size);
        cout << "Введите элементы массива (" << size << " шт.): ";
        for (int i = 0; i < size; i++) {
            cin >> arr[i];
        }

        // Сортировка пузырьком (Bubble Sort)
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    double temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }

        cout << "\nОтсортированный массив: ";
        for (int i = 0; i < size; i++) {
            cout << arr[i] << " ";
        }
        cout << endl;

    }
    else if (userChoice == 2) {
        // ================= ЗАДАНИЕ 3: СТАТИСТИКА + OMP =================

        // --- АВТОГЕНЕРАЦИЯ ФАЙЛА НА 5 МЛН ЧИСЕЛ ---
        int gen_size = 5000000;
        ofstream genFile("inputt.txt");
        if (genFile.is_open()) {
            genFile << gen_size << "\n";
            for (int i = 0; i < gen_size; i++) {
                genFile << (rand() % 1000) / 10.0 << " ";
            }
            genFile.close();
        }
        // ------------------------------------------

        ifstream inFile("inputt.txt");
        if (!inFile.is_open()) {
            cout << "Ошибка! Не удалось открыть файл inputt.txt" << endl;
            return 1;
        }

        int size;
        if (!(inFile >> size) || size <= 0) {
            cout << "Ошибка чтения количества элементов из файла!" << endl;
            return 1;
        }

        vector<double> data(size);
        for (int i = 0; i < size; i++) {
            if (!(inFile >> data[i])) {
                cout << "Ошибка: в файле меньше элементов, чем указано в размере!" << endl;
                return 1;
            }
        }
        inFile.close();

        cout << "Данные из файла прочитаны. Количество элементов: " << size << "\n\n";

        // Прогрев потоков OpenMP перед началом замеров
#pragma omp parallel num_threads(4)
        {
        }

        // Массивы для фиксации результатов тестов производительности
        int thread_modes[3] = { 1, 2, 4 };
        double means[3], std_devs[3], times[3];

        // Запуск вычислений последовательно для 1, 2 и 4 потоков
        for (int i = 0; i < 3; i++) {
            calculateStatsOMP(data, thread_modes[i], means[i], std_devs[i], times[i]);
            cout << "Потоков: " << thread_modes[i]
                << " | Среднее: " << means[i]
                << " | Отклонение: " << std_devs[i]
                << " | Время: " << times[i] << " сек.\n";
        }

        // Запись итогового отчета в файл outputt.txt
        ofstream outFile("outputt.txt");
        if (!outFile.is_open()) {
            cout << "Ошибка создания файла outputt.txt!" << endl;
            return 1;
        }

        outFile << "--- РЕЗУЛЬТАТЫ ВЫЧИСЛЕНИЙ (ЗАДАНИЕ 3) ---\n";
        outFile << "Размер выборки: " << size << "\n\n";

        outFile << "Среднее арифметическое: " << means[0] << "\n";
        outFile << "Среднеквадратическое отклонение: " << std_devs[0] << "\n\n";

        outFile << "--- СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ---\n";
        for (int i = 0; i < 3; i++) {
            outFile << "Потоков: " << thread_modes[i] << " -> Время расчета: " << times[i] << " сек.\n";
        }
        outFile.close();

        cout << "\nРезультаты и замеры времени успешно сохранены в файл: outputt.txt" << endl;
    }
    else {
        cout << "Неверный выбор!" << endl;
    }

    return 0;
}
