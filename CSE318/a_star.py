from queue import PriorityQueue
from math import sqrt

goal_grid = [[1, 2, 3], [4, 5, 6], [7, 8, 0]]
# weight = [1.0, 1.2, 2.0, 5.0]

def get_key(grid):
    return tuple(tuple(row) for row in grid)
 

def dist(p1, p2):
    x1, y1 = p1
    x2, y2 = p2
    return sqrt((x1 - x2)**2 + (y1 - y2)**2)


def get_position(grid) :
    list = [None] * 9
    for i in range(3):
        for j in range(3):
            list[grid[i][j]] = (i,j)
    return list

def wrong_cell_count(grid) :
    count = 0
    for i in range(3):
        for j in range(3):
            if grid[i][j] != 0 and grid[i][j] != (i*3 + j + 1) :
                count += 1
    return count
        
def manhattan_distance(grid) :
    d = 0
    lst = get_position(grid)
    for i in range(3):
        for j in range(3):
            if grid[i][j] != 0 :
                d += abs(i - lst[grid[i][j]][0]) + abs(j - lst[grid[i][j]][1])
    return d

def euclidean_distance(grid) :
    distance = 0
    lst = get_position(grid)
    for i in range(3):
        for j in range(3):
            if grid[i][j] != 0 :
                distance += dist((i,j), lst[grid[i][j]])
    return distance

def linear_conflict(grid) :
    distance = 0
    lst = get_position(grid)
    for i in range(3):
        for j in range(3):
            if grid[i][j] != 0 and grid[i][j] != (i*3 + j + 1) :
                if lst[grid[i][j]][0] == i :
                    for k in range(j+1, 3):
                        if grid[i][k] != 0 and grid[i][k] != (i*3 + k + 1) and lst[grid[i][k]][0] == i :
                            distance += 2
                if lst[grid[i][j]][1] == j :
                    for k in range(i+1, 3):
                        if grid[k][j] != 0 and grid[k][j] != (k*3 + j + 1) and lst[grid[k][j]][1] == j :
                            distance += 2
    return distance


def custom(grid) :
    return manhattan_distance(grid) + linear_conflict(grid)


class Node:
    def __init__(self, grid, parent=None, g=0, h=0, wt = 1.0):
        self.grid = grid
        self.parent = parent
        self.g = g
        self.h = h
        self.weight = wt

    def f(self):
        return self.g + self.weight * self.h
    
    def set_parent(self, parent):
        self.parent = parent
    
    def __lt__(self, other):
        if self.h != other.h:
            return self.h < other.h 
        return self.grid < other.grid    


def get_neighbors(node):
    grid = node.grid
    zero_pos = [(i, row.index(0)) for i, row in enumerate(grid) if 0 in row][0]
    x, y = zero_pos
    
    for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
        xdash, ydash = x+dx, y+dy
        if 0 <= xdash < 3 and 0 <= ydash < 3:
            nw_grid = [row[:] for row in grid]
            nw_grid[x][y], nw_grid[xdash][ydash] = nw_grid[xdash][ydash], nw_grid[x][y]
            yield nw_grid
    
    
def a_star(init_grid, heuristic_fun, weight):
    pq = PriorityQueue()
    init_node = Node(init_grid, None, 0, heuristic_fun(init_grid), weight)
    
    pq.put((init_node.f(), init_node))
    
    closed = set()
    init_node.set_parent(None)
    g_score = {}
    g_score[get_key(init_grid)] = 0
    
    init_node.set_parent(None)
    
    while not pq.empty():
        f, node = pq.get()
        if node in closed:
            continue
        closed.add(get_key(node.grid))
        
        if node.grid == goal_grid:
            path = []
            while node:
                path.append(node.grid)
                node = node.parent
            
            path.reverse()
            return path
        
        for neighbor_grid in get_neighbors(node):
            if get_key(neighbor_grid) in closed:
                continue
            
            nw_g = node.g + 1
            if get_key(neighbor_grid) not in g_score or nw_g < g_score[get_key(neighbor_grid)]:
                g_score[get_key(neighbor_grid)] = nw_g
                h = heuristic_fun(neighbor_grid)
                neighbor_node = Node(neighbor_grid, node, nw_g, h, weight)
                pq.put((neighbor_node.f(), neighbor_node))
                
                
def print_path(path):
    print("Minimum number of moves:", len(path) - 1)
    for grid in path:
        for row in grid:
            print(row)
        print() 

if __name__ == "__main__":
    initial_grid = [[0, 1, 3], [4, 2, 5], [7, 8, 6]]
    heuristic_function = manhattan_distance
    weight_value = 1.0
    
    path = a_star(initial_grid, heuristic_function, weight_value)
    print_path(path)
            
            