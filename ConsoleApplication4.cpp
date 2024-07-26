#include <windows.h>
#include <iostream>
#include <vector>
#include <cmath>

struct ThreadData {
    int threadId;
    int numThreads;
    std::vector<std::vector<double>>* matrix;
    HANDLE startEvent;
    HANDLE finishEvent;
    HANDLE* allFinishEvents;
    HANDLE consoleMutex;  // Додавання м'ютекса для синхронізації виводу в консоль
};

DWORD WINAPI GaussianElimination(LPVOID param) {
    ThreadData* data = (ThreadData*)param;
    std::vector<std::vector<double>>& matrix = *data->matrix;
    int numThreads = data->numThreads;
    int threadId = data->threadId;
    int n = matrix.size();

    for (int k = 0; k < n; ++k) {
        // Використання м'ютексу для синхронізованого виводу в консоль
        WaitForSingleObject(data->consoleMutex, INFINITE);
        std::cout << "Thread " << threadId << " starts step " << k << std::endl;
        ReleaseMutex(data->consoleMutex);

        if (k % numThreads == threadId) {
            for (int i = k + 1; i < n; ++i) {
                double factor = matrix[i][k] / matrix[k][k];
                for (int j = k; j <= n; ++j) {
                    matrix[i][j] -= factor * matrix[k][j];
                }
            }
        }

        WaitForSingleObject(data->consoleMutex, INFINITE);
        std::cout << "Thread " << threadId << " finished step " << k << std::endl;
        ReleaseMutex(data->consoleMutex);

        // Сигналізація про завершення обробки кроку
        SetEvent(data->finishEvent);

        WaitForMultipleObjects(numThreads, data->allFinishEvents, TRUE, INFINITE);

        // Перезапуск для наступного кроку
        if (threadId == 0) {
            ResetEvent(data->startEvent);
            for (int i = 0; i < numThreads; ++i) {
                ResetEvent(data->allFinishEvents[i]);
            }
            SetEvent(data->startEvent);
        }

        WaitForSingleObject(data->startEvent, INFINITE);
    }
    return 0;
}

int main() {
    const int numThreads = 4;
    std::vector<std::vector<double>> matrix = { {3, 2, -4, 3},
                                               {2, 3, 3, 15},
                                               {5, -3, 1, 14} };
    HANDLE startEvent = CreateEvent(NULL, TRUE, TRUE, NULL);
    std::vector<HANDLE> threads(numThreads);
    std::vector<HANDLE> finishEvents(numThreads);
    std::vector<ThreadData> threadData(numThreads);
    HANDLE consoleMutex = CreateMutex(NULL, FALSE, NULL); // М'ютекс для виводу в консоль

    for (int i = 0; i < numThreads; ++i) {
        finishEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
    }

    for (int i = 0; i < numThreads; ++i) {
        threadData[i].threadId = i;
        threadData[i].numThreads = numThreads;
        threadData[i].matrix = &matrix;
        threadData[i].startEvent = startEvent;
        threadData[i].finishEvent = finishEvents[i];
        threadData[i].allFinishEvents = &finishEvents[0];
        threadData[i].consoleMutex = consoleMutex; // Передача м'ютексу в структуру даних потоку
        threads[i] = CreateThread(NULL, 0, GaussianElimination, &threadData[i], 0, NULL);
    }

    WaitForMultipleObjects(numThreads, &threads[0], TRUE, INFINITE);

    // Вивід результату
    WaitForSingleObject(consoleMutex, INFINITE);
    std::cout << "----------------------------------------\n";
    std::cout << "Final matrix:" << std::endl;
    for (const auto& row : matrix) {
        for (double val : row) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    ReleaseMutex(consoleMutex);

    // Закриття дескрипторів
    for (HANDLE thread : threads) {
        CloseHandle(thread);
    }
    CloseHandle(startEvent);
    for (HANDLE event : finishEvents) {
        CloseHandle(event);
    }
    CloseHandle(consoleMutex); //закрити м'ютекса

    return 0;
}