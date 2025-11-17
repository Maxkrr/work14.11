import random

def total_distance(route, dist_matrix):
    n = len(route)
    return sum(dist_matrix[route[i]][route[(i + 1) % n]] for i in range(n))

def order_crossover(parent1, parent2):
    size = len(parent1)
    a, b = sorted(random.sample(range(size), 2))
    child = [None] * size
    child[a:b+1] = parent1[a:b+1]
    fill_idx = (b + 1) % size
    for city in parent2[b+1:] + parent2[:b+1]:
        if city not in child:
            child[fill_idx] = city
            fill_idx = (fill_idx + 1) % size
    return child

def swap_mutation(route):
    a, b = random.sample(range(len(route)), 2)
    route[a], route[b] = route[b], route[a]

def genetic_algorithm(dist_matrix, pop_size=30, generations=100):
    n = len(dist_matrix)
    population = [random.sample(range(n), n) for _ in range(pop_size)]
    
    for _ in range(generations):
        population.sort(key=lambda x: total_distance(x, dist_matrix))
        new_pop = population[:2]  # Элитизм
        
        while len(new_pop) < pop_size:
            parents = random.sample(population[:10], 2)  # Турнирный отбор
            child = order_crossover(parents[0], parents[1])
            if random.random() < 0.2:  # Вероятность мутации 20%
                swap_mutation(child)
            new_pop.append(child)
        
        population = new_pop
    
    best_route = min(population, key=lambda x: total_distance(x, dist_matrix))
    return best_route, total_distance(best_route, dist_matrix)

# Матрица расстояний между 10 городами
dist_matrix = [
    [0, 29, 82, 46, 68, 52, 72, 42, 51, 55],
    [29, 0, 55, 46, 42, 43, 43, 23, 23, 31],
    [82, 55, 0, 68, 46, 55, 23, 43, 41, 29],
    [46, 46, 68, 0, 82, 15, 72, 31, 62, 42],
    [68, 42, 46, 82, 0, 74, 23, 52, 21, 46],
    [52, 43, 55, 15, 74, 0, 61, 23, 55, 31],
    [72, 43, 23, 72, 23, 61, 0, 42, 23, 31],
    [42, 23, 43, 31, 52, 23, 42, 0, 33, 15],
    [51, 23, 41, 62, 21, 55, 23, 33, 0, 29],
    [55, 31, 29, 42, 46, 31, 31, 15, 29, 0]
]

best_route, cost = genetic_algorithm(dist_matrix)
print(f"Лучший маршрут: {best_route}")
print(f"Стоимость: {cost}")
# Вывод терминала:
Лучший маршрут: [0, 1, 8, 4, 6, 2, 9, 7, 5, 3]
Стоимость: 247
