import heapq
import json
from typing import List, Tuple


def check_valid(
    state: list, max_missionaries: int, max_cannibals: int
) -> bool:  # 10 marks
    """
    Graded
    Check if a state is valid. State format: [m_left, c_left, boat_position].
    """
    raise ValueError("check_valid not implemented")


def get_neighbours(
    state: list, max_missionaries: int, max_cannibals: int
) -> List[list]:  # 10 marks
    """
    Graded
    Generate all valid neighbouring states.
    """
    raise ValueError("get_neighbours not implemented")


def gstar(state: list, new_state: list) -> int:  # 5 marks
    """
    Graded
    The weight of the edge between state and new_state, this is the number of people on the boat.
    """
    raise ValueError("gstar not implemented")


def h1(state: list) -> int:  # 3 marks
    """
    Graded
    h1 is the number of people on the left bank.
    """
    raise ValueError("h1 not implemented")

def h2(state: list) -> int:  # 3 marks
    """
    Graded
    h2 is the number of missionaries on the left bank. 
    """
    raise ValueError("h2 not implemented")


def h3(state: list) -> int:  # 3 marks
    """
    Graded
    h3 is the number of cannibals on the left bank.
    """
    raise ValueError("h3 not implemented")


def h4(state: list) -> int:  # 3 marks
    """
    Graded
    Weights of missionaries is higher than cannibals.
    h4 = missionaries_left * 1.5 + cannibals_left
    """
    raise ValueError("h4 not implemented")


def h5(state: list) -> int:  # 3 marks
    """
    Graded
    Weights of missionaries is lower than cannibals.
    h5 = missionaries_left + cannibals_left*1.5
    """
    raise ValueError("h5 not implemented")


def astar_h1(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 28 marks
    """
    Graded
    Implement A* with h1 heuristic.
    This function must return path obtained and a boolean which says if the heuristic chosen satisfes Monotone restriction property while exploring or not.
    """
    raise ValueError("astar_h1 not implemented")


def astar_h2(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 8 marks
    """
    Graded
    Implement A* with h2 heuristic.
    """
    raise ValueError("astar_h2 not implemented")


def astar_h3(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 8 marks
    """
    Graded
    Implement A* with h3 heuristic.
    """
    raise ValueError("astar_h3 not implemented")

def astar_h4(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 8 marks
    """
    Graded
    Implement A* with h4 heuristic.
    """
    raise ValueError("astar_h4 not implemented")


def astar_h5(
    init_state: list, final_state: list, max_missionaries: int, max_cannibals: int
) -> Tuple[List[list], bool]:  # 8 marks
    """
    Graded
    Implement A* with h5 heuristic.
    """
    raise ValueError("astar_h5 not implemented")


def print_solution(solution: List[list],max_mis,max_can):
    """
    Prints the solution path. 
    """
    if not solution:
        print("No solution exists for the given parameters.")
        return
        
    print("\nSolution found! Number of steps:", len(solution) - 1)
    print("\nLeft Bank" + " "*20 + "Right Bank")
    print("-" * 50)
    
    for state in solution:
        if state[-1]:
            boat_display = "(B) " + " "*15
        else:
            boat_display = " "*15 + "(B) "
            
        print(f"M: {state[0]}, C: {state[1]}  {boat_display}" 
              f"M: {max_mis-state[0]}, C: {max_can-state[1]}")


def print_mon(ism: bool):
    """
    Prints if the heuristic function is monotone or not.
    """
    if ism:
        print("-" * 10)
        print("|Monotone|")
        print("-" * 10)
    else:
        print("-" * 14)
        print("|Not Monotone|")
        print("-" * 14)


def main():
    try:
        testcases = [{"m": 3, "c": 3}]

        for case in testcases:
            max_missionaries = case["m"]
            max_cannibals = case["c"]
            
            init_state = [max_missionaries, max_cannibals, 1] #initial state 
            final_state = [0, 0, 0] # final state
            
            if not check_valid(init_state, max_missionaries, max_cannibals):
                print(f"Invalid initial state for case: {case}")
                continue
                
            path_h1,ism1 = astar_h1(init_state, final_state, max_missionaries, max_cannibals)
            path_h2,ism2 = astar_h2(init_state, final_state, max_missionaries, max_cannibals)
            path_h3,ism3 = astar_h3(init_state, final_state, max_missionaries, max_cannibals)
            path_h4,ism4 = astar_h4(init_state, final_state, max_missionaries, max_cannibals)
            path_h5,ism5 = astar_h5(init_state, final_state, max_missionaries, max_cannibals)
            print_solution(path_h1,max_missionaries,max_cannibals)
            print_mon(ism1)
            print("-"*50)
            print_solution(path_h2,max_missionaries,max_cannibals)
            print_mon(ism2)
            print("-"*50)
            print_solution(path_h3,max_missionaries,max_cannibals)
            print_mon(ism3)
            print("-"*50)
            print_solution(path_h4,max_missionaries,max_cannibals)
            print_mon(ism4)
            print("-"*50)
            print_solution(path_h5,max_missionaries,max_cannibals)
            print_mon(ism5)
            print("="*50)

    except KeyError as e:
        print(f"Missing required key in test case: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")


if __name__ == "__main__":
    main()
