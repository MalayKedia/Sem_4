import numpy as np
from collections import deque
import heapq
from typing import List, Tuple, Set, Dict
"""
Do not import any other package unless allowed by te TAs in charge of the lab.
Do not change the name of any of the functions below.
"""

def bfs(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int]:
    """
    Implement Breadth-First Search algorithm to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array.
                            Example: np.array([[1, 2, 3], [4, 0, 5], [6, 7, 8]])
                            where 0 represents the blank space
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array.
                          Example: np.array([[1, 2, 3], [4, 5, 6], [7, 8, 0]])
    
    Returns:
        Tuple[List[str], int]: A tuple containing:
            - List of moves to reach the goal state. Each move is represented as
              'U' (up), 'D' (down), 'L' (left), or 'R' (right), indicating how
              the blank space should move
            - Number of nodes expanded during the search

    Example return value:
        (['R', 'D', 'R'], 12) # Means blank moved right, down, right; 12 nodes were expanded
              
    """
    # TODO: Implement this function
    pass

def dfs(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int]:
    """
    Implement Depth-First Search algorithm to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array
    
    Returns:
        Tuple[List[str], int]: A tuple containing:
            - List of moves to reach the goal state
            - Number of nodes expanded during the search
    """
    # TODO: Implement this function
    pass

def dijkstra(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int, int]:
    """
    Implement Dijkstra's algorithm to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array
    
    Returns:
        Tuple[List[str], int, int]: A tuple containing:
            - List of moves to reach the goal state
            - Number of nodes expanded during the search
            - Total cost of the path for transforming initial into goal configuration
            
    """
    # TODO: Implement this function
    pass

def astar_dt(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int, int]:
    """
    Implement A* Search with Displaced Tiles heuristic to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array
    
    Returns:
        Tuple[List[str], int, int]: A tuple containing:
            - List of moves to reach the goal state
            - Number of nodes expanded during the search
            - Total cost of the path for transforming initial into goal configuration
              
    
    """
    # TODO: Implement this function
    pass

def astar_md(initial: np.ndarray, goal: np.ndarray) -> Tuple[List[str], int, int]:
    """
    Implement A* Search with Manhattan Distance heuristic to solve 8-puzzle problem.
    
    Args:
        initial (np.ndarray): Initial state of the puzzle as a 3x3 numpy array
        goal (np.ndarray): Goal state of the puzzle as a 3x3 numpy array
    
    Returns:
        Tuple[List[str], int, int]: A tuple containing:
            - List of moves to reach the goal state
            - Number of nodes expanded during the search
            - Total cost of the path for transforming initial into goal configuration
    """
    # TODO: Implement this function
    pass

# Example test case to help verify your implementation
if __name__ == "__main__":
    # Example puzzle configuration
    initial_state = np.array([
        [1, 2, 3],
        [4, 0, 5],
        [6, 7, 8]
    ])
    
    goal_state = np.array([
        [1, 2, 3],
        [4, 5, 6],
        [7, 8, 0]
    ])
    
    # Test each algorithm
    print("Testing BFS...")
    bfs_moves, bfs_expanded = bfs(initial_state, goal_state)
    print(f"BFS Solution: {bfs_moves}")
    print(f"Nodes expanded: {bfs_expanded}")
    
    print("\nTesting DFS...")
    dfs_moves, dfs_expanded = dfs(initial_state, goal_state)
    print(f"DFS Solution: {dfs_moves}")
    print(f"Nodes expanded: {dfs_expanded}")
    
    print("\nTesting Dijkstra...")
    dijkstra_moves, dijkstra_expanded, dijkstra_cost = dijkstra(initial_state, goal_state)
    print(f"Dijkstra Solution: {dijkstra_moves}")
    print(f"Nodes expanded: {dijkstra_expanded}")
    print(f"Total cost: {dijkstra_cost}")
    
    print("\nTesting A* with Displaced Tiles...")
    dt_moves, dt_expanded, dt_fscore = astar_dt(initial_state, goal_state)
    print(f"A* (DT) Solution: {dt_moves}")
    print(f"Nodes expanded: {dt_expanded}")
    print(f"Total cost: {dt_fscore}")
    
    print("\nTesting A* with Manhattan Distance...")
    md_moves, md_expanded, md_fscore = astar_md(initial_state, goal_state)
    print(f"A* (MD) Solution: {md_moves}")
    print(f"Nodes expanded: {md_expanded}")
    print(f"Total cost: {md_fscore}")