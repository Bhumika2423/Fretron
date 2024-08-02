import matplotlib.pyplot as plt
import numpy as np
from heapq import heappush, heappop
from shapely.geometry import LineString

# Define the flight paths
flights = {
    "Flight 1": [(1, 1), (2, 2), (3, 3)],
    "Flight 2": [(1, 1), (2, 4), (3, 2)],
    "Flight 3": [(1, 1), (4, 2), (3, 4)]
}

# Heuristic function for A*
def heuristic(a, b):
    return abs(a[0] - b[0]) + abs(a[1] - b[1])

# A* algorithm implementation
def a_star(start, goal, grid):
    neighbors = [(0, 1), (1, 0), (0, -1), (-1, 0)]
    close_set = set()
    came_from = {}
    gscore = {start: 0}
    fscore = {start: heuristic(start, goal)}
    oheap = []

    heappush(oheap, (fscore[start], start))

    while oheap:
        current = heappop(oheap)[1]

        if current == goal:
            data = []
            while current in came_from:
                data.append(current)
                current = came_from[current]
            data.append(start)
            return data[::-1]

        close_set.add(current)
        for i, j in neighbors:
            neighbor = (current[0] + i, current[1] + j)
            tentative_g_score = gscore[current] + 1
            if (0 <= neighbor[0] < grid.shape[0] and
                0 <= neighbor[1] < grid.shape[1] and
                grid[neighbor[0], neighbor[1]] == 0):
                if (neighbor in close_set and tentative_g_score >= gscore.get(neighbor, float('inf'))):
                    continue

                if (tentative_g_score < gscore.get(neighbor, float('inf')) or
                    neighbor not in [i[1] for i in oheap]):
                    came_from[neighbor] = current
                    gscore[neighbor] = tentative_g_score
                    fscore[neighbor] = tentative_g_score + heuristic(neighbor, goal)
                    heappush(oheap, (fscore[neighbor], neighbor))

    return None

def conflict_based_search(flights):
    grid_size = 10
    all_paths = {}

    def replan_all_paths():
        grid = np.zeros((grid_size, grid_size), dtype=int)
        for flight, coords in flights.items():
            path = [coords[0]]
            for i in range(len(coords) - 1):
                start = coords[i]
                goal = coords[i + 1]
                subpath = a_star(start, goal, grid)
                if not subpath:
                    raise ValueError(f"No path found from {start} to {goal}")
                for (x, y) in subpath:
                    grid[x, y] = 1
                path.extend(subpath[1:])
            all_paths[flight] = path

    def detect_conflicts(paths):
        for i, (flight1, path1) in enumerate(paths.items()):
            for flight2, path2 in list(paths.items())[i + 1:]:
                for p1, p2 in zip(path1, path2):
                    if LineString([p1, path1[path1.index(p1) + 1]]).intersects(LineString([p2, path2[path2.index(p2) + 1]])):
                        return True
        return False

    replan_all_paths()

    iterations = 0
    max_iterations = 100
    while iterations < max_iterations:
        if not detect_conflicts(all_paths):
            print("No more conflicts detected.")
            break

        print("Conflict detected. Replanning paths...")
        grid = np.zeros((grid_size, grid_size), dtype=int)
        for flight, path in all_paths.items():
            for x, y in path:
                grid[x, y] = 1

        for flight in all_paths:
            start = all_paths[flight][0]
            goal = all_paths[flight][-1]
            new_path = a_star(start, goal, grid)
            if new_path:
                all_paths[flight] = new_path
            else:
                print(f"Failed to replan path for {flight}")
        
        iterations += 1
        if iterations == max_iterations:
            print("Maximum number of iterations reached. Replanning all paths...")
            replan_all_paths()

    return all_paths

def plot_flight_paths(flights):
    colors = ['blue', 'red', 'yellow']

    plt.figure(figsize=(8, 8))
    for i, (flight, coords) in enumerate(flights.items()):
        xs, ys = zip(*coords)
        plt.plot(xs, ys, marker='o', color=colors[i], label=flight)

    plt.xlabel('X Coordinate')
    plt.ylabel('Y Coordinate')
    plt.legend()
    plt.grid(True)
    plt.title('Flight Paths with Conflict-Based Search')
    plt.show()

try:
    # Run CBS-based multi-agent pathfinding
    final_paths = conflict_based_search(flights)

    # Plot the adjusted flight paths
    plot_flight_paths(final_paths)
except ValueError as e:
    print(e)
