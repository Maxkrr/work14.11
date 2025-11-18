#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// Вспомогательные функции для генерации случайных чисел
double random_double(double a, double b) {
    return a + (b - a) * (rand() / (RAND_MAX + 1.0));
}

int random_int(int a, int b) {
    return a + rand() % (b - a + 1);
}

// Функция вычисления суммарного расстояния до ближайших центров
double totalDistance(const vector<pair<double, double>>& clients,
                    const vector<pair<double, double>>& centers) {
    double total = 0.0;
    for (const auto& client : clients) {
        double min_dist = 1e18;
        for (const auto& center : centers) {
            double dx = client.first - center.first;
            double dy = client.second - center.second;
            double dist = sqrt(dx*dx + dy*dy);
            if (dist < min_dist) {
                min_dist = dist;
            }
        }
        total += min_dist;
    }
    return total;
}

vector<pair<double, double>> sa_facility_location(
    const vector<pair<double, double>>& clients,
    int k, double temp, double cooling) {
    
    // Инициализация: случайные позиции k центров
    vector<pair<double, double>> centers(k);
    for (int i = 0; i < k; i++) {
        centers[i] = {
            random_double(-10, 10),
            random_double(-10, 10)
        };
    }
    
    double current_cost = totalDistance(clients, centers);
    
    while (temp > 1e-6) {
        // Создаём кандидата: сдвигаем один случайный центр
        vector<pair<double, double>> new_centers = centers;
        int idx = random_int(0, k - 1);
        new_centers[idx].first += random_double(-1, 1) * temp;
        new_centers[idx].second += random_double(-1, 1) * temp;
        
        double new_cost = totalDistance(clients, new_centers);
        
        // ДОПОЛНЕНИЕ: критерий Метрополиса для принятия/отклонения решения
        if (new_cost < current_cost) {
            // Если новое решение лучше - принимаем
            centers = new_centers;
            current_cost = new_cost;
        } else {
            // Если решение хуже - принимаем с вероятностью exp((current_cost - new_cost)/temp)
            double probability = exp((current_cost - new_cost) / temp);
            if (random_double(0, 1) < probability) {
                centers = new_centers;
                current_cost = new_cost;
            }
        }
        
        temp *= cooling;
    }
    return centers;
}

// Пример использования и тестирования
int main() {
    srand(time(0)); // Инициализация генератора случайных чисел
    
    // Создаем тестовые точки-клиентов
    vector<pair<double, double>> clients = {
        {1.0, 1.0}, {2.0, 2.0}, {3.0, 3.0}, 
        {8.0, 8.0}, {9.0, 9.0}, {10.0, 10.0}
    };
    
    int k = 2; // Количество центров
    double initial_temp = 100.0; // Начальная температура
    double cooling_rate = 0.95; // Коэффициент охлаждения
    
    // Запускаем алгоритм
    auto centers = sa_facility_location(clients, k, initial_temp, cooling_rate);
    
    // Выводим результаты
    cout << "Найденные центры:\n";
    for (size_t i = 0; i < centers.size(); i++) {
        cout << "Центр " << i << ": (" << centers[i].first << ", " << centers[i].second << ")\n";
    }
    
    double final_cost = totalDistance(clients, centers);
    cout << "Финальная стоимость (суммарное расстояние): " << final_cost << endl;
    
    return 0;
}
// Вывод терминала:
//Найденные центры:
//Центр 0: (-22.2628, -99.0718)
//Центр 1: (2.40908, 6.38619)
//Финальная стоимость (суммарное расстояние): 34.7268
