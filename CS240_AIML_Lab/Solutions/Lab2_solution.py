import heapq
import json 
from typing import List,Tuple


def check_valid(state: list, max_missionaries: int, max_cannibals: int) -> bool: # 10 marks 
    """
    Check if a state is valid. State format: [m_left, c_left, boat_position]
    """
    if max_cannibals > max_missionaries:
        return False
    if not (0 <= state[0] <= max_missionaries and 0 <= state[1] <= max_cannibals):
        return False
    if (state[0] > 0 and state[0] < state[1]) or ((max_missionaries-state[0]) > 0 and (max_missionaries-state[0]) < (max_cannibals-state[1])):
        return False
        
    return True

def get_neighbours(state: list, max_missionaries: int, max_cannibals: int) -> List[list]: # 10 marks
    """Generate all valid neighbouring states"""
    moves = [(0,1), (1,0), (1,1), (0,2), (2,0)]
    new_boat_state = 1 - state[-1]
    possible_states = []
    
    for dm, dc in moves:
        new_state = state.copy()
        
        if state[-1] == 1:  # Boat is on left bank
            new_state[0] -= dm
            new_state[1] -= dc
        else:  # Boat is on right bank
            new_state[0] += dm
            new_state[1] += dc
            
        new_state[-1] = new_boat_state
        
        if check_valid(new_state, max_missionaries, max_cannibals):
            possible_states.append(new_state)
    
    return possible_states

def gstar(state : list, new_state : list) -> int : # 5 marks 
    """The value of the edge between state and new_state this is the number of people on the boat""" 
    return abs(state[0]-new_state[0])+abs(state[1]-new_state[1])

def h1(state: list) -> int: # 5 marks
    """
    Number of people on the left bank.
    """
    return (state[0] + state[1])

def h2(state: list) -> int: # 5 marks
    """
    Graded
    Weights missionaries higher than cannibals due to their movement constraints.
    h2 = missionaries_left
    """
    return state[0]

def h3(state: list) -> int: # 5 marks 
    """
    Graded
    Weights missionaries higher than cannibals.
    h3 =  cannibals_left
    """
    return state[1]


def master_astar(init_state: list, final_state: list, max_missionaries: int, 
                 max_cannibals: int,g_star : callable,h : callable) -> Tuple[List[list],bool]:
    """A* search implementation for missionaries and cannibals problem"""
    
    # Since all the 3 heuristic functions satisy monotone restriction property, below algorithm will work. 
    # Recall 2nd question from quiz-1.
    
    closed_list = set()
    parents = {}
    open_list = [(h(init_state), 0, None,init_state)]  # (f_score, g_score, parent,state)
    is_monotone_restricted = True 
    
    while open_list:
        _, g_score, parent,current_state = heapq.heappop(open_list)
        state_str = str(current_state)

        if state_str in closed_list:
            continue
            
        closed_list.add(state_str)
        parents[state_str] = parent

        if current_state == final_state:
            path = []
            while current_state:
                path.append(current_state)
                current_state = parents.get(str(current_state))
            return path[::-1], is_monotone_restricted
        
        for new_state in get_neighbours(current_state, max_missionaries, max_cannibals):
            new_state_str = str(new_state)

            # tentative_g_score is the actual cost from start to new_state through current path
            tentative_g_score = g_score + g_star(current_state,new_state)

            if new_state_str in closed_list:
                continue
            
            f_score = tentative_g_score + h(new_state)
            heapq.heappush(open_list, (f_score, tentative_g_score,current_state,new_state))
    return [],is_monotone_restricted

def astar_h1(init_state: list,final_state: list, max_missionaries: int,max_cannibals: int) -> Tuple[List[list],bool]: # 20 marks
    """
    A* function with h1 heuristic. 
    """
    return master_astar(init_state,final_state,max_missionaries,max_cannibals,gstar,h1) 

def astar_h2(init_state: list,final_state: list, max_missionaries: int,max_cannibals: int) -> Tuple[List[list],bool]: # 20 marks
    """
    A* function with h2 heuristic. 
    """
    return master_astar(init_state,final_state,max_missionaries,max_cannibals,gstar,h2)

def astar_h3(init_state: list,final_state: list, max_missionaries: int,max_cannibals: int) -> Tuple[List[list],bool]: # 20 marks
    """
    A* function with h3 heuristic. 
    """
    return master_astar(init_state,final_state,max_missionaries,max_cannibals,gstar,h3)


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
    if(ism):
        print("-"*10)
        print("|Monotone|")    
        print("-"*10)
    else: 
        print("-"*14)
        print("|Not Monotone|")
        print("-"*14)


def main():
    try:
        testcases = [{"m": 4, "c": 3}]

        for case in testcases:
            max_missionaries = case["m"]
            max_cannibals = case["c"]
            
            init_state = [max_missionaries, max_cannibals, 1]
            final_state = [0, 0, 0]
            
            if not check_valid(init_state, max_missionaries, max_cannibals):
                print(f"Invalid initial state for case: {case}")
                continue
                
            path_h1,ism1 = astar_h1(init_state, final_state, max_missionaries, max_cannibals)
            path_h2,ism2 = astar_h2(init_state, final_state, max_missionaries, max_cannibals)
            path_h3,ism3 = astar_h3(init_state, final_state, max_missionaries, max_cannibals)
            print_solution(path_h1,max_missionaries,max_cannibals)
            print_mon(ism1)
            print("-"*50)
            print_solution(path_h2,max_missionaries,max_cannibals)
            print_mon(ism2)
            print("-"*50)
            print_solution(path_h3,max_missionaries,max_cannibals)
            print_mon(ism3)
            print("="*50)

    except json.JSONDecodeError:
        print("Error reading JSON input file")
    except KeyError as e:
        print(f"Missing required key in test case: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()