import heapq
import math
import time
import numpy as np
#import seaborn as sns
#import tkinter as tk
import matplotlib.pyplot as plt


from tabulate import tabulate

# Used for states generation (getChildren())
dx = [-1, 1, 0, 0]
dy = [0, 0, 1, -1]

# All global algorithm statistics variables have been removed.
# Search functions now return these statistics in a dictionary.
# The global `end_state` that was set to None is also removed.
# Individual search functions will manage their own end_state via parameters.



# Function to generate all valid children of a certain node for n x n puzzles
def getChildren(state, size):
    #print(state, size)
    children = []
    idx = state.index(0)
    i = idx // size
    j = idx % size
    for x in range(4):
        nx = i + dx[x]
        ny = j + dy[x]
        nwIdx = nx * size + ny
        if checkValid(nx, ny, size):
            listTemp = list(state)
            listTemp[idx], listTemp[nwIdx] = listTemp[nwIdx], listTemp[idx]
            children.append(listTemp)
    return children

# Function to check if the goal state is reached
def goalTest(current_state, the_actual_end_state):
    return current_state == the_actual_end_state


################################################################################################################
# Ways to evaluate if it is solvable
# Formas para evaluar si es resoluble

# Generalized function to check if the start state is solvable for n x n puzzles
#def isSolvable(digit):
#    #print(len(digit))
#    count = 0
#    for i in range(len(digit)):
#        for j in range(i, len(digit)):
#            if digit[i] > digit[j] and digit[i] != 0: # and digit[j] != 0:   # esto ultimo lo he añadido yo
#                count += 1
#        if size<=3:
#            if len(digit)%2 != 0:
#            #print("sumo 1")
#                if digit[i] == 0 and i%2==0:
#                    count += 1
#    #print(count)
#    return count % 2 == 0


################################################################################################################
# Another way
# Otra forma


def count_inversions(arr):
    """Helper function to count inversions in a state"""
    inv_count = 0
    for i in range(len(arr) - 1):
        for j in range(i + 1, len(arr)):
            if arr[i] and arr[j] and arr[i] > arr[j]:  # exclude blank (0)
                inv_count += 1
    return inv_count

def get_blank_row_from_bottom(state, size):
    """Helper function to get row number of blank tile counting from bottom"""
    blank_idx = state.index(0)
    return size - (blank_idx // size)



def isSolvable(initial_state, final_state):
    """
    Determines whether a puzzle is solvable by comparing the initial state against the goal state.
    This implementation works for any puzzle size.

    Determina si un puzzle es resoluble comparando el estado inicial con el estado final.
    Implementación que funciona para cualquier tamaño de puzzle.
    """
    size = int(math.sqrt(len(initial_state)))
    
    # Calcular inversiones
    initial_inversions = count_inversions(initial_state)
    final_inversions = count_inversions(final_state)
    
    # Obtener posición del espacio vacío / empty square (contando desde abajo)
    initial_blank_row = get_blank_row_from_bottom(initial_state, size)
    final_blank_row = get_blank_row_from_bottom(final_state, size)
    
    # Para puzzles de tamaño impar (odd size)
    if size % 2 == 1:
        return initial_inversions % 2 == final_inversions % 2
    # Para puzzles de tamaño par (even size)
    else:
        return (initial_inversions + initial_blank_row) % 2 == (final_inversions + final_blank_row) % 2


################################################################################################################
# Another way
# Otra forma

"""
# function to check if the start state solvable or not ONLY FOR SIZE = 3
def isSolvable3(digit):

    count = 0
    for i in range(0, 9):
        for j in range(i, 9):
            if digit[i] > digit[j] and digit[i] != 0:
                count += 1
        if digit[i] == 0 and i%2==0:
            count += 1

    return count % 2 == 0

def getInvCount(arr, N):

    inv_count = 0
    for i in range(N * N - 1):
        for j in range(i + 1,N * N):
            # count pairs(arr[i], arr[j]) such that
            # i < j and arr[i] > arr[j]
            if (arr[j] and arr[i] and arr[i] > arr[j]):
                inv_count+=1


    return inv_count


# find Position of blank from bottom
def findXPosition(puzzle, N):
    matriz = [puzzle[i:i+N] for i in range(0, len(puzzle), N)]
    # start from bottom-right corner of matrix
    for i in range(N - 1,-1,-1):
        for j in range(N - 1,-1,-1):
            if (matriz[i][j] == 0):
                return N - i


# This function returns true if given
# instance of N*N - 1 puzzle is solvable
def isSolvable(puzzle, puzzle2):
    N=int(math.sqrt(len(puzzle)))

    if N == 3 and is_spiral_state(puzzle): return isSolvable3(puzzle)   #caso particular de 3x3 en espiral

    # Count inversions in given puzzle
    invCount = getInvCount(puzzle, N)

    # If grid is odd, return true if inversion
    # count is even.
    if (N & 1):
        return ~(invCount & 1)

    else:    # grid is even
        pos = findXPosition(puzzle, N)
        if (pos & 1):
            return ~(invCount & 1)
        else:
            return invCount & 1

"""


# Function to check if the indices are within the puzzle bounds
def checkValid(i, j, size):
    return 0 <= i < size and 0 <= j < size



# Generalized graphSearch function to work with n x n puzzles
def graphSearch(inputState, end_state_input, function_g, function_h, size, maximum_depth=-1):
    start_time = time.time()
    integer_state = tuple(inputState)
    actual_end_state = end_state_input  # Use local variable for end_state

    heap = []
    explored = {}
    parent = {}
    cost_map = {}

    # Localize variables
    local_path = []
    local_cost = 0
    local_depth = 0
    local_time_elapsed = 0.0
    local_generated_nodes = 1  # Start with the initial state
    local_expanded_nodes = 0
    # local_max_nodes_in_memory initialized after initial heap push
    local_open_list_unique_count = 0 # Tracks unique states in heap_map > 0
    
    # Variables for internal tracking, might not all be part of the final returned dict
    # but are needed for calculations that were previously global.
    # local_explored_count = 0 # Number of states moved to explored set
    # local_heap_count = 0 # Number of items currently in heap (approximated)
    # local_open_counter = 0 # Number of nodes in open list (heap) - if different from local_heap_count

    # Initialize heap with the start state
    # Pass actual_end_state to heuristic if it needs it
    # Siempre llama a la heurística con dos argumentos
    initial_h_cost = function_h(inputState, actual_end_state)
    heapq.heappush(heap, (initial_h_cost, integer_state))
    cost_map[integer_state] = initial_h_cost
    heap_map = {}  # To track items in heap for efficient updates/checks
    
    # Initial state processing for heap_map and unique count
    heap_map[integer_state] = 1
    local_open_list_unique_count = 1
    local_max_nodes_in_memory = 1 # Initially 1 (start node) + 0 (explored)
    
    depth_map = {}
    depth_map[integer_state] = 0

    while heap:
        node = heapq.heappop(heap)
        state = node[1] # This is a tuple
        string_state = list(state) # Convert to list for functions expecting list

        # Safely decrement heap_map count for the popped state
        if state in heap_map: # Should always be true if came from heap
            heap_map[state] -= 1
            if heap_map[state] == 0:
                local_open_list_unique_count -= 1
                # del heap_map[state] # Optional: clean up map if count is zero

        # If already explored, skip (continue to the next node in heap)
        if state in explored:
            continue

        # Determine parent_cost. If function_h needs end_state, pass it.
        current_h_cost = function_h(string_state, actual_end_state)
        parent_cost = node[0] - current_h_cost # g(parent)

        # Mark as explored and update counts
        explored[state] = 1
        local_expanded_nodes = len(explored)
        # local_depth is updated when children are processed or goal is found
        
        # Update max_nodes_in_memory
        local_max_nodes_in_memory = max(local_max_nodes_in_memory, len(explored) + local_open_list_unique_count)

        if goalTest(string_state, actual_end_state):
            local_path = getPath(parent, tuple(inputState), actual_end_state)
            local_cost = len(local_path) - 1
            local_time_elapsed = float(time.time() - start_time)
            current_goal_depth = depth_map.get(state, 0) # Get depth of the goal state
            
            return {
                'path': local_path,
                'cost': local_cost,
                'depth': current_goal_depth,
                'time': local_time_elapsed,
                'nodes_generated': local_generated_nodes,
                'nodes_expanded': local_expanded_nodes,
                'max_nodes': local_max_nodes_in_memory
            }

        if (maximum_depth == -1) or (depth_map[state] < maximum_depth):
            children = getChildren(state, size)
            for child_list in children: # getChildren returns list of lists
                child_tuple = tuple(child_list) # Convert to tuple for dictionary keys/set elements
                local_generated_nodes += 1
                
                # Pass actual_end_state to heuristic if it needs it
                child_h_cost = function_h(child_list, actual_end_state)
                
                # Cost to reach child: g(parent) + cost(parent, child) + h(child)
                # Assuming function_g(0) is cost(parent,child) = 1, or similar logic
                new_total_cost = parent_cost + function_g(0) + child_h_cost

                # NEW-1A: Child is not in explored and not effectively in heap (heap_map count is 0)
                if child_tuple not in explored and heap_map.get(child_tuple, 0) == 0:
                    heapq.heappush(heap, (new_total_cost, child_tuple))
                    # Transition from 0 to 1 in heap_map means new unique item for open list
                    local_open_list_unique_count += 1
                    heap_map[child_tuple] = 1
                    cost_map[child_tuple] = new_total_cost
                    parent[child_tuple] = state
                    depth_map[child_tuple] = depth_map[state] + 1
                    local_depth = max(depth_map[child_tuple], local_depth) # Tracks max depth reached

                # NEW-2: Child is not in explored, but is in heap (heap_map count > 0), and new path is better
                elif child_tuple not in explored and new_total_cost < cost_map[child_tuple]:
                    heapq.heappush(heap, (new_total_cost, child_tuple))
                    heap_map[child_tuple] += 1 # Increment count, unique count unchanged
                    cost_map[child_tuple] = new_total_cost
                    parent[child_tuple] = state
                    depth_map[child_tuple] = depth_map[state] + 1
                    local_depth = max(depth_map[child_tuple], local_depth)

                # NEW-3: Child is in explored, but new path is better (re-opening)
                elif child_tuple in explored and new_total_cost < cost_map[child_tuple]:
                    del explored[child_tuple] # Remove from explored
                    # No change to local_expanded_nodes here, it's len(explored) after processing current node
                    
                    heapq.heappush(heap, (new_total_cost, child_tuple))
                    if heap_map.get(child_tuple, 0) == 0:
                        local_open_list_unique_count += 1 # Was not in open list, now it is
                    heap_map[child_tuple] = heap_map.get(child_tuple, 0) + 1
                    
                    cost_map[child_tuple] = new_total_cost
                    parent[child_tuple] = state
                    depth_map[child_tuple] = depth_map[state] + 1
                    local_depth = max(depth_map[child_tuple], local_depth)
                
                # After any child processing that modifies heap or explored counts
                local_max_nodes_in_memory = max(local_max_nodes_in_memory, len(explored) + local_open_list_unique_count)

    # Goal not found
    local_time_elapsed = float(time.time() - start_time)
    final_depth_val = 0
    if explored: # Calculate max depth from explored nodes if any
        final_depth_val = max((depth_map[s] for s in explored if s in depth_map), default=0)
    
    return {
        'path': [],
        'cost': 0,
        'depth': final_depth_val,
        'time': local_time_elapsed,
        'nodes_generated': local_generated_nodes,
        'nodes_expanded': local_expanded_nodes, # len(explored) at this point
        'max_nodes': local_max_nodes_in_memory
    }



# DFS_B_ (Inner Recursive Function)
def DFS_B_(state, depth, maximum_depth, actual_end_state, current_lim_ida, function_h, size, explored_set):
    stats = {
        'found_goal': False,
        'path_segment': [],
        'nodes_expanded_subtree': 1,  # Current node is expanded
        'max_depth_subtree': depth,
        'nodes_generated_subtree': 0,
        'max_len_explored_subtree': len(explored_set),
        'min_next_lim_sig_subtree': float('inf')
    }

    # Check goal state
    if goalTest(state, actual_end_state):
        stats['found_goal'] = True
        stats['max_depth_subtree'] = depth
        # Path segment is built on return from recursion
        return stats

    # IDA* specific logic
    if current_lim_ida != -1:
        # Heuristic function call needs to handle actual_end_state if it's designed for it
        h_cost = function_h(tuple(state), actual_end_state)
        f_value = depth + h_cost
        if f_value > current_lim_ida:
            stats['min_next_lim_sig_subtree'] = min(stats['min_next_lim_sig_subtree'], f_value)
            if tuple(state) in explored_set: # only remove if it was added
                explored_set.remove(tuple(state))
            return stats

    # Depth-limited DFS check
    if maximum_depth != -1 and depth == maximum_depth: # maximum_depth is -1 for pure IDA*
        if tuple(state) in explored_set: # only remove if it was added
             explored_set.remove(tuple(state))
        return stats

    # Expand children
    children = getChildren(state, size)
    stats['nodes_generated_subtree'] += len(children)

    for child in children:
        child_tuple = tuple(child)
        if child_tuple not in explored_set:
            explored_set.add(child_tuple)
            stats['max_len_explored_subtree'] = max(stats['max_len_explored_subtree'], len(explored_set))

            recursive_stats = DFS_B_(child, depth + 1, maximum_depth, actual_end_state, current_lim_ida, function_h, size, explored_set)

            stats['nodes_expanded_subtree'] += recursive_stats['nodes_expanded_subtree']
            stats['nodes_generated_subtree'] += recursive_stats['nodes_generated_subtree']
            stats['max_depth_subtree'] = max(stats['max_depth_subtree'], recursive_stats['max_depth_subtree'])
            stats['max_len_explored_subtree'] = max(stats['max_len_explored_subtree'], recursive_stats['max_len_explored_subtree'])
            stats['min_next_lim_sig_subtree'] = min(stats['min_next_lim_sig_subtree'], recursive_stats['min_next_lim_sig_subtree'])

            if recursive_stats['found_goal']:
                stats['found_goal'] = True
                stats['path_segment'] = [child_tuple] + recursive_stats['path_segment']
                return stats  # Propagate solution upwards immediately

    # If loop finishes and goal not found, backtrack
    if not stats['found_goal']:
        if tuple(state) in explored_set: # only remove if it was added
            explored_set.remove(tuple(state))
    return stats

# DFS_B (Outer Function)
def DFS_B(inputState, end_state_input, size, maximum_depth=-1, lim_ida=-1, function_h=None):
    start_time = time.time()
    actual_end_state = end_state_input

    local_path = []
    local_cost = 0
    # nodes_expanded is from DFS_B_ result
    local_max_depth = 0
    local_nodes_generated = 1  # For the initial state
    local_max_nodes_stored = 1 # For the initial state in explored set
    local_lim_sig = float('inf')

    explored = set()
    explored.add(tuple(inputState))

    # Note: DFS_B_ manages explored set additions/removals for its subtree
    dfs_results = DFS_B_(inputState, 0, maximum_depth, actual_end_state, lim_ida, function_h, size, explored)

    local_nodes_generated += dfs_results['nodes_generated_subtree']
    local_nodes_expanded = dfs_results['nodes_expanded_subtree']
    local_max_depth = dfs_results['max_depth_subtree']
    # Max nodes stored: initial (1), max during recursion, or final size of explored (if goal found at root, explored might be small)
    local_max_nodes_stored = max(local_max_nodes_stored, dfs_results['max_len_explored_subtree'], len(explored))


    if lim_ida != -1: # This is for IDA*
        local_lim_sig = dfs_results['min_next_lim_sig_subtree']

    if dfs_results['found_goal']:
        local_path = [tuple(inputState)] + dfs_results['path_segment']
        local_cost = len(local_path) - 1
    
    local_time_elapsed = float(time.time() - start_time)

    results_dict = {
        'path': local_path,
        'cost': local_cost,
        'depth': local_max_depth,
        'time': local_time_elapsed,
        'nodes_generated': local_nodes_generated,
        'nodes_expanded': local_nodes_expanded,
        'max_nodes': local_max_nodes_stored
    }
    
    # The original DFS_B returned 0 or 1 for success/failure, or a tuple for IDA*
    # Now, we consistently return the results_dict and local_lim_sig.
    # Callers will check len(results_dict['path']) > 0 to determine if a solution was found.
    return (results_dict, local_lim_sig)


def ID_B(inputState, end_state_input, size):
    actual_end_state = end_state_input  # Use actual_end_state instead of global end_state

    total_nodes_expanded = 0
    total_nodes_generated = 0 # Accumulates from each DFS_B call, plus 1 for inputState if not counted in DFS_B
    final_path = []
    final_cost = 0
    final_depth = 0 # This will be the depth at which solution is found
    final_max_nodes_stored = 0 # Max nodes stored across all DFS_B iterations
    
    start_time_id = time.time()
    solution_reached_flag = False
    current_max_depth_limit = 1

    # Initial state is considered generated
    # total_nodes_generated = 1 # If DFS_B doesn't count the root, uncomment. But our DFS_B counts its root.

    while not solution_reached_flag:
        # Call the refactored DFS_B, ignoring lim_sig as it's not used by ID_B
        dfs_b_results_dict, _ = DFS_B(inputState, actual_end_state, size, current_max_depth_limit)

        total_nodes_generated += dfs_b_results_dict['nodes_generated']
        total_nodes_expanded += dfs_b_results_dict['nodes_expanded']
        final_max_nodes_stored = max(final_max_nodes_stored, dfs_b_results_dict['max_nodes'])
        # The depth reported by DFS_B is the actual depth of nodes visited.
        # For ID_B, the 'final_depth' is the depth at which the solution is found.

        if len(dfs_b_results_dict['path']) > 0:  # Solution found
            solution_reached_flag = True
            final_path = dfs_b_results_dict['path']
            final_cost = dfs_b_results_dict['cost']
            final_depth = dfs_b_results_dict['depth'] # This is the depth of the goal node
            # print(f'ID: Sol reached at depth {current_max_depth_limit}') # Optional print
        
        current_max_depth_limit += 1

    total_time_elapsed = float(time.time() - start_time_id)

    return {
        'path': final_path,
        'cost': final_cost,
        'depth': final_depth,
        'time': total_time_elapsed,
        'nodes_generated': total_nodes_generated,
        'nodes_expanded': total_nodes_expanded,
        'max_nodes': final_max_nodes_stored
    }

def IDA_B(inputState, end_state_input, size, function_h):
    actual_end_state = end_state_input # Use actual_end_state

    total_nodes_expanded = 0
    total_nodes_generated = 0
    final_path = []
    final_cost = 0
    final_depth = 0
    final_max_nodes_stored = 0
    
    start_time_id = time.time()
    solution_reached_flag = False
    
    # Initialize current_f_limit using function_h, passing actual_end_state
    current_f_limit = function_h(inputState, actual_end_state)

    while not solution_reached_flag:
        # Call the refactored DFS_B
        # Pass -1 for maximum_depth, current_f_limit as lim_ida for DFS_B
        dfs_b_results_dict, next_f_limit = DFS_B(inputState, actual_end_state, size, -1, current_f_limit, function_h)

        total_nodes_generated += dfs_b_results_dict['nodes_generated']
        total_nodes_expanded += dfs_b_results_dict['nodes_expanded']
        final_max_nodes_stored = max(final_max_nodes_stored, dfs_b_results_dict['max_nodes'])

        if len(dfs_b_results_dict['path']) > 0:  # Solution found
            solution_reached_flag = True
            final_path = dfs_b_results_dict['path']
            final_cost = dfs_b_results_dict['cost']
            final_depth = dfs_b_results_dict['depth']
        else: # Solution not found in this iteration
            if next_f_limit == float('inf'):
                # No solution possible with increasing f-limits, or all nodes explored
                break # Exit loop, will return current stats (empty path)
        
        current_f_limit = next_f_limit

    total_time_elapsed = float(time.time() - start_time_id)

    return {
        'path': final_path,
        'cost': final_cost,
        'depth': final_depth,
        'time': total_time_elapsed,
        'nodes_generated': total_nodes_generated,
        'nodes_expanded': total_nodes_expanded,
        'max_nodes': final_max_nodes_stored
    }



def getPath(parentMap, inputState, actual_end_state):
    path = []
    temp = tuple(actual_end_state)
    while temp != inputState:
        path.append(temp)
        temp = parentMap[temp]
    path.append(inputState)
    path.reverse()
    return path





# Heuristics
# Heurísticas



def getLinear_conflict(state, end_state):
    """
    Computes the number of linear conflicts in rows and columns.
    
    Calcula la cantidad de conflictos lineales en filas y columnas.
    """
    size = int(len(state) ** 0.5)
    conflicts = 0

    goal = end_state

    # Contar conflictos en filas (rows)
    for row in range(size):
        row_values = state[row * size:(row + 1) * size]
        goal_row = goal[row * size:(row + 1) * size]
        conflicts += count_row_conflicts(row_values, goal_row)

    # Contar conflictos en columnas (columns)
    for col in range(size):
        col_values = state[col::size]
        goal_col = goal[col::size]
        conflicts += count_column_conflicts(col_values, goal_col)

    return conflicts

def count_row_conflicts(row, goal_row):
    conflicts = 0
    # Crear una lista con los valores que están en su fila objetivo (values in goal row)
    values = [tile for tile in row if tile in goal_row and tile != 0]
    for i in range(len(values)):
        for j in range(i + 1, len(values)):
            # Verifica si están en orden inverso al del objetivo (reverse order from the goal)
            if goal_row.index(values[i]) > goal_row.index(values[j]):
                conflicts += 1  # cuenta el par conflictivo (counts the conflicting pair)
    return conflicts * 2  # cada conflicto aumenta la heurística en 2

def count_column_conflicts(col, goal_col):
    conflicts = 0
    values = [tile for tile in col if tile in goal_col and tile != 0]
    for i in range(len(values)):
        for j in range(i + 1, len(values)):
            if goal_col.index(values[i]) > goal_col.index(values[j]):
                conflicts += 1
    return conflicts * 2

def get_md_plus_linear_conflict(state, end_state):
    """
    Computes the heuristic Manhattan Distance + Linear Conflict.
    
    Calcula la heurística Manhattan Distance + Linear Conflict.
    """
    return getManhattanDistance(state, end_state) + getLinear_conflict(state, end_state)
        

def getManhattanDistance(state, end_state):
    tot = 0
    size=int(math.sqrt(len(state)))

    for i in range(1, len(state)):
        goal = end_state.index(i)
        goalX = goal // size
        goalY = goal % size
        idx = state.index(i)
        itemX = idx // size
        itemY = idx % size
        tot += abs(goalX - itemX) + abs(goalY - itemY)
    return tot


    

# Utilities
# Utilidades
import random


def infer_movement(current_state, next_state, size):
    """
    Infers which tile moved onto the empty square between two consecutive states.
    
    Infiere qué ficha se movió al espacio vacío entre dos estados consecutivos.
    """
    current_zero_idx = current_state.index(0)
    next_zero_idx = next_state.index(0)

    # La ficha que se mueve al espacio vacío es la que ocupa la posición del '0' en el siguiente estado
    moved_tile = next_state[current_zero_idx]

    # Convertir los índices unidimensionales en coordenadas 2D (opcional, si quieres añadir coordenadas)
    current_row, current_col = divmod(current_zero_idx, size)
    next_row, next_col = divmod(next_zero_idx, size)

    return moved_tile


def visualize_state(state):
    # Imprimir el estado como una matriz
    size = int(math.sqrt(len(state)))
    for i in range(size):
        row = state[i * size:(i + 1) * size]  # Obtener la fila correspondiente
        print(" ".join(f"{num:2}" if num != 0 else "  " for num in row))  # Formateo de ancho fijo


def visualize_solution_text(path, size):
    """
    Visualizes the solution in text format, showing each step as a matrix
    and adding information on the movement performed.
    
    Visualiza la solución en formato de texto, mostrando cada paso como una matriz
    y añadiendo información sobre el movimiento realizado.
    """
    for step_num, state in enumerate(path):
        # Imprimir el número del paso (step number)
        print(f"Level {step_num}\n")

        # Imprimir el estado como una matriz (state as a matrix)
        for i in range(size):
            row = state[i * size:(i + 1) * size]  # Obtener la fila correspondiente
            print(" ".join(f"{num:2}" if num != 0 else "  " for num in row))  # Formateo de ancho fijo
        #plt = draw_state( state, size ) 


        # Separador entre pasos (separator between steps)
        print("\n" + "-" * 20)
                # Si no es el último estado, inferir el movimiento
        if step_num != len(path)-1:
            moved_tile = infer_movement(path[step_num], path[step_num + 1], size)
            # Imprimir el número del paso y la ficha que se movió
            print(f"(Move tile: {moved_tile})\n")
        else: print("Solution found\n")



def draw_state( state, size, plt = plt ):

    ### Ocultar los ejes (hide axes)
    plt.axis('off')  
    
    plt.rcParams["figure.figsize"] = (1,1)
    
    ### Tamaño original de la figura (original size of the plot)
    ###plt.rcParams["figure.figsize"] = plt.rcParamsDefault["figure.figsize"]
    
    # Calcula la matriz de correlación
    np_state = np.array( state ).reshape(size, size)
    
    # Crear heatmap
    sns.heatmap(np_state, cmap='crest', annot=True, fmt='.0f', cbar=False)
    
    # Muestra el gráfico
    plt.show()
    ###plt.plot()
    
    return plt



def draw_path_for_algorithm (result, algorithm_name, size): 
    plt = draw_state ( initial_state, size )
    for step_num, state in enumerate( result[algorithm_name]['path'] ):
        if (step_num > 0):
            plt = draw_state( state, size, plt ) 


# Función para mostrar el path de un algoritmo específico
def show_path_for_algorithm(results, algorithm_name, size):
    if algorithm_name in results:
        result = results[algorithm_name]
        path_length = len(result['path']) - 1   # Longitud del path
        print(f"Path for {algorithm_name} (Length: {path_length} steps):")
        visualize_solution_text(result['path'], size)
    else:
        print(f"No results found for algorithm {algorithm_name}")




def plot_algorithm_comparison(results):
    # List of metrics to plot, adding a new metric 'cost/time' for the ratio
    metrics = ['cost', 'time', 'nodes_generated', 'nodes_expanded', 'max_nodes', 'cost/time']

    # Create a figure with subplots, one for each metric
    fig, axes = plt.subplots(nrows=2, ncols=3, figsize=(20, 8))
    axes = axes.flatten()  # Flatten to easily iterate

    # Loop over each metric and create a subplot
    for i, metric in enumerate(metrics):
        ax = axes[i]
        algorithms = list(results.keys())

        # Calculate values for each metric
        if metric == 'cost/time':
            values = [results[algo]['cost']  / results[algo]['time'] if results[algo]['time'] != 0 else 0 for algo in algorithms]
        else:
            values = [results[algo][metric] for algo in algorithms]

        ax.bar(algorithms, values)
        ax.set_title(f'{metric.capitalize()} Comparison')
        ax.set_ylabel(metric)
        ax.set_xticks(range(len(algorithms)))
        ax.set_xticklabels(algorithms, rotation=45, ha='right')

    # Adjust layout so everything fits without overlap
    plt.tight_layout()
    plt.show()




def show_results(results):
    """
    Displays the results of the algorithms in table format.
    Args:
        results: Dictionary with the results of the algorithms executed

    Muestra los resultados de los algoritmos en formato tabla.
    Args:
        results: Diccionario con los resultados de los algoritmos ejecutados
    """
    table_data = []
    headers = ["Algorithm", "Cost", "Depth", "Time (s)", "Nodes Generated", "Nodes Expanded", "Max Nodes Stored"]

    for algorithm_name, result in results.items():
        table_data.append([
            algorithm_name,
            result['cost'],
            result['depth'],
            result['time'],
            result['nodes_generated'],
            result['nodes_expanded'],
            result['max_nodes']
        ])

    # Mostrar los resultados en formato de tabla usando tabulate
    print(tabulate(table_data, headers, tablefmt="grid"))
    print("\n")




def generate_random_state(size, end_state=None):
    """
    Generate a random state for a square puzzle of given size.
    If end_state is provided, ensures the random state is solvable relative to it.
    """
    import random

    def count_inversions(state):
        """Helper function to count inversions in a state"""
        inversions = 0
        for i in range(len(state)):
            for j in range(i + 1, len(state)):
                if state[i] != 0 and state[j] != 0 and state[i] > state[j]:
                    inversions += 1
        return inversions

    def get_blank_row_from_bottom(state, size):
        """Helper function to get row number of blank tile from bottom"""
        blank_index = state.index(0)
        return size - (blank_index // size)

    def is_solvable(state, goal, size):
        """Helper function to check if a state is solvable"""
        state_inversions = count_inversions(state)
        goal_inversions = count_inversions(goal)
        
        state_blank_row = get_blank_row_from_bottom(state, size)
        goal_blank_row = get_blank_row_from_bottom(goal, size)
        
        if size % 2 == 1:
            return state_inversions % 2 == goal_inversions % 2
        else:
            return (state_inversions + state_blank_row) % 2 == (goal_inversions + goal_blank_row) % 2

    # Create a list of numbers from 0 to size*size-1
    numbers = list(range(size * size))
    random_state = numbers[:]
    
    if end_state is None:
        # If no end_state provided, just return any random permutation
        random.shuffle(random_state)
        return random_state
    
    # end_state is already a 1D list, so we don't need to flatten it

    # Keep generating random states until we find a solvable one
    max_attempts = 1000
    attempts = 0
    
    while attempts < max_attempts:
        random.shuffle(random_state)
        if is_solvable(random_state, end_state, size):
            return random_state
        attempts += 1
    
    # If we couldn't find a solvable state after max_attempts,
    # just return the goal state with a few valid moves applied
    state = end_state[:]
    steps = random.randint(20, 50)
    
    for _ in range(steps):
        blank_index = state.index(0)
        i = blank_index // size
        j = blank_index % size
        
        # Get valid moves
        valid_moves = []
        for x, y in [(-1,0), (1,0), (0,-1), (0,1)]:  # up, down, left, right
            new_i, new_j = i + x, j + y
            if 0 <= new_i < size and 0 <= new_j < size:
                valid_moves.append((new_i, new_j))
        
        if valid_moves:
            new_i, new_j = random.choice(valid_moves)
            new_index = new_i * size + new_j
            state[blank_index], state[new_index] = state[new_index], state[blank_index]
    
    return state


def generate_ordered_state(n):
    """
    Generate an ordered state for a square puzzle of given size.
    Returns a 1D list with numbers 1 to n^2-1 followed by 0.
    """
    return list(range(1, n*n)) + [0]  # create a list from 1 to n^2-1 and append 0


def generate_spiral_state(n):
    """
    Generate a spiral pattern state for a square puzzle of given size.
    Returns a 1D list with numbers arranged in a clockwise spiral pattern,
    with 0 as the last number placed (near or at the center).
    """
    matrix = [[0] * n for _ in range(n)]  # Initialize nxn matrix with zeros
    numbers = list(range(1, n*n)) + [0]  # List from 1 to n^2-1 with 0 at the end

    left, right = 0, n - 1
    top, bottom = 0, n - 1
    index = 0

    while left <= right and top <= bottom:
        # Fill top row
        for i in range(left, right + 1):
            matrix[top][i] = numbers[index]
            index += 1
        top += 1

        # Fill right column
        for i in range(top, bottom + 1):
            matrix[i][right] = numbers[index]
            index += 1
        right -= 1

        # Fill bottom row
        if top <= bottom:
            for i in range(right, left - 1, -1):
                matrix[bottom][i] = numbers[index]
                index += 1
            bottom -= 1

        # Fill left column
        if left <= right:
            for i in range(bottom, top - 1, -1):
                matrix[i][left] = numbers[index]
                index += 1
            left += 1
    
    # Convert matrix to 1D list in row-major order
    result = []
    for i in range(n):
        for j in range(n):
            result.append(matrix[i][j])
    
    return result


def is_spiral_state(state):
    """
    Verifies whether the puzzle state is arranged as a spiral.
    Args:
        state: 1D list representing the puzzle state
    Returns:
        bool: True if the state is a spiral, False otherwise

    Verifica si un estado del puzzle está ordenado en espiral.
    Args:
        state: Lista 1D representando el estado del puzzle
    Returns:
        bool: True si el estado está en espiral, False en caso contrario
    """
    n = int(math.sqrt(len(state)))  # Tamaño del puzzle (puzzle size)
    
    # Generar el estado espiral esperado
    expected_state = generate_spiral_state(n)
    
    # Comparar directamente el estado con el estado espiral esperado
    return state == expected_state
    
    # Verificar que el siguiente número sea 0
    found_zero = False
    # Verificar fila superior restante
    for i in range(left, right + 1):
        if matrix[top][i] == 0:
            found_zero = True
            break
        elif matrix[top][i] != expected:
            return False
            
    if not found_zero:
        # Verificar columna derecha restante
        for i in range(top, bottom + 1):
            if matrix[i][right] == 0:
                found_zero = True
                break
            elif matrix[i][right] != expected:
                return False
                
    if not found_zero:
        # Verificar fila inferior restante
        for i in range(right, left - 1, -1):
            if matrix[bottom][i] == 0:
                found_zero = True
                break
            elif matrix[bottom][i] != expected:
                return False
                
    if not found_zero:
        # Verificar columna izquierda restante
        for i in range(bottom, top - 1, -1):
            if matrix[i][left] == 0:
                found_zero = True
                break
            elif matrix[i][left] != expected:
                return False
    
    return found_zero
