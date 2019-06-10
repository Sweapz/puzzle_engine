/**
 * The reachability header file library.
 * Author: Christopher Hansen Nielsen
 * Mail: chni15@student.aau.dk, Student Number: 20154154
 *
 * Written and compiled on a windows machine.
 * g++ -std=c++17 -pedantic -Wall -DNDEBUG -O3 -o frogs frogs.cpp
 * g++ -std=c++17 -pedantic -Wall -DNDEBUG -O3 -o crossing crossing.cpp
 * g++ -std=c++17 -pedantic -Wall -DNDEBUG -O3 -o family family.cpp
 */

#ifndef PUZZLEENGINE_REACHABILITY_HPP
#define PUZZLEENGINE_REACHABILITY_HPP

#include <vector>
#include <list>
#include <functional>
#include <iostream>
#include <algorithm>
#include <typeinfo>

// This enum is used to handle the support for different search orders except for cost order. It is implemented
// as part of requirement 5.
enum search_order_t {
    breadth_first, depth_first
};

// This struct is the basis for keeping track of the solution when traversing the states. When it is used, it holds a
// pointer to the parent node as well as a copy of the state.
template<class StateTypeT>
struct trace_node {
    trace_node *parentState;
    StateTypeT selfState;
};

// This function is used to pass on the transition generator function from the respective puzzles. It is implemented
// as part of requirement 2.
template<class StateTypeT>
std::function<std::list<std::function<void(StateTypeT &)>>(StateTypeT &)>
successors(std::list<std::function<void(StateTypeT &)>> transitions(const StateTypeT &state)) {
    return transitions;
}

// This class holds all the information about a given state space. It utilizes two template types StateTypeT and
// CostTypeT. These are the basis of the generic implementation as part of requirements 8 and 9.
template<class StateTypeT, class CostTypeT = std::nullptr_t>
class state_space_t {
private:
    StateTypeT _startState;
    CostTypeT _initialCost;
    std::function<std::list<std::function<void(StateTypeT &)>>(StateTypeT &)> _transitionFunctions;
    std::function<bool(const StateTypeT &)> _invariantFunction;
    std::function<CostTypeT(const StateTypeT &state, const CostTypeT &cost)> _costFunction;
    bool _isCostEnabled; // used explicitly to determine whether or not a cost have been specified.

    template<class ValidationFunction>
    std::list<StateTypeT> solveOrder(ValidationFunction isGoalState, search_order_t order);

    template<class ValidationFunction>
    std::list<StateTypeT> solveCost(ValidationFunction isGoalState);

public:
    // This is the first constructor for the class, which handles calls from the frogs.cpp
    // and crossing.cpp instantiation.
    state_space_t(const StateTypeT startInputState,
                  std::function<std::list<std::function<void(StateTypeT &)>>(StateTypeT &)> transFunctions,
                  bool invariantFunc(const StateTypeT &) = [](
                          const StateTypeT &state) { return true; }) : _startState(startInputState),
                                                                      _transitionFunctions(
                                                                              transFunctions),
                                                                      _invariantFunction(invariantFunc),
                                                                      _isCostEnabled(false){
    }

    // This the second and overloaded constructor for the class. This handles calls from the family.cpp.
    state_space_t(const StateTypeT startInputState, const CostTypeT costInput,
                  std::function<std::list<std::function<void(StateTypeT &)>>(StateTypeT &)> transFunctions,
                  bool invariantFunc(const StateTypeT &) = [](
                          const StateTypeT &state) { return true; },
                  std::function<CostTypeT(const StateTypeT &state, const CostTypeT &cost)> costFunc = [](
                          const StateTypeT &state, const CostTypeT &cost)
                                  { return CostTypeT{0, 0}; }) : _startState(
                                                                      startInputState),
                                                                  _initialCost(
                                                                          costInput),
                                                                  _transitionFunctions(
                                                                          transFunctions),
                                                                  _invariantFunction(
                                                                          invariantFunc),
                                                                  _costFunction(
                                                                          costFunc),
                                                                  _isCostEnabled(
                                                                          true) {
    }

    template<class ValidationFunction>
    std::list<StateTypeT> check(ValidationFunction isGoalState, search_order_t order = search_order_t::breadth_first);
};

// This function is called from the different puzzle files and returns a solution if found. It introduces a new template
// ValidationFunction that handles the goal predicate function. It also takes an order, which is defaulted to
// breadth first if nothing else is specified.
// It returns a list of states.
template<class StateTypeT, class CostTypeT>
template<class ValidationFunction>
std::list<StateTypeT> state_space_t<StateTypeT, CostTypeT>::check(ValidationFunction isGoalState, search_order_t order) {
    std::list<StateTypeT> solution;

    if (_isCostEnabled) { // Here we check if the cost method is specified, and calls the solveCost if true.
        solution = solveCost(isGoalState);
    } else { // Otherwise we call the solveOrder method with the order provided.
        solution = solveOrder(isGoalState, order);
    }

    // Returns the list of states. Implemented as part of requirement 4.
    return solution;
}

// The method is used when solving the state space based on a given cost. It takes in isGoalState which is a predicate
// that is used to determine whether a solution have been found.
// It returns a list of states. It is implemented as part of requirement 7.
template<class StateTypeT, class CostTypeT>
template<class ValidationFunction>
std::list<StateTypeT> state_space_t<StateTypeT, CostTypeT>::solveCost(ValidationFunction isGoalState) {
    StateTypeT currentState;
    CostTypeT itCost{_initialCost}, newCost;
    trace_node<StateTypeT> *traceState {};
    std::list<StateTypeT> passed, solution;
    std::list<std::pair<CostTypeT, trace_node<StateTypeT> *>> waiting;

    // The method utilizes a waiting list that contains a pair of cost and a trace_node. The cost is used to determine
    // which element should be popped from the list next.
    waiting.push_back(std::make_pair(itCost, new trace_node<StateTypeT>{nullptr, _startState}));

    while (!waiting.empty()) {
        // Here we take the first element from the list and then pop it.
        currentState = waiting.front().second->selfState;
        traceState = waiting.front().second;
        itCost = waiting.front().first;
        waiting.pop_front();

        // Here we check if the goal state has been reached. This is implemented as part of requirement 3.
        if (isGoalState(currentState)) {
            // If a goal is found, we use the traceState to traverse back up through the tree of trace_nodes
            // until the parentState is NULL. For each node we push the state to the solution list.
            while (traceState->parentState != NULL) {
                solution.push_front(traceState->selfState);
                traceState = traceState->parentState;
            }

            solution.push_front(traceState->selfState); // Adds the start state to the solution trace.
            return solution;
        }

        // Here we check if the currentState is part of the passed list.
        if (!(std::find(passed.begin(), passed.end(), currentState) != passed.end())) {
            // If it is not, we push it to the list, and generate the transitions via the _transitionFunctions which
            // is a member of the state_space_t class.
            passed.push_back(currentState);
            auto transitions = _transitionFunctions(currentState);

            for (auto transition: transitions) {
                // For each transition, we then generate the successor
                auto successor{currentState};
                transition(successor);

                // Prevents invalid states being added to waiting via an invariant predicate. This is implemented as
                // part of requirement 6.
                if (!_invariantFunction(successor)) {
                    continue;
                }
                newCost = _costFunction(successor, itCost);
                waiting.push_back(std::make_pair(newCost, new trace_node<StateTypeT>{traceState, successor}));
            }
            if (!transitions.empty()) { // Prevents sorting if no new transitions have been found.
                // Here we sort the waiting list based on the cost for each pair in the list.
                waiting.sort([](const std::pair<CostTypeT, trace_node<StateTypeT> *> &a,
                                std::pair<CostTypeT, trace_node<StateTypeT> *> &b) { return a.first < b.first; });
            }
        }
    }

    return solution;
}

// The method is used when solving the state space based on a given order. It takes in isGoalState which is a predicate
// that is used to determine whether a solution have been found. It also takes an order, which specifies how the
// solution should be found. The method is very similar to solveCost in functionality.
// It returns a list of states.
template<class StateTypeT, class CostTypeT>
template<class ValidationFunction>
std::list<StateTypeT>
state_space_t<StateTypeT, CostTypeT>::solveOrder(ValidationFunction isGoalState, search_order_t order) {
    StateTypeT currentState;
    trace_node<StateTypeT> *traceState {};
    std::list<StateTypeT> passed, solution;
    std::list<trace_node<StateTypeT> *> waiting;

    // As solveOrder does not utilize a cost, waiting is just a list of trace_nodes.
    waiting.push_back(new trace_node<StateTypeT>{nullptr, _startState});

    while (!waiting.empty()) {
        switch (order) { // We switch on the order to determine what element should be accessed and popped from waiting.
            case breadth_first:
                currentState = waiting.front()->selfState;
                traceState = waiting.front();
                waiting.pop_front();
                break;
            case depth_first:
                currentState = waiting.back()->selfState;
                traceState = waiting.back();
                waiting.pop_back();
                break;
            default:
                std::cout << "Order not supported" << std::endl;
                break;
        }
        if (isGoalState(currentState)) {
            while (traceState->parentState != NULL) {
                solution.push_front(traceState->selfState);
                traceState = traceState->parentState;
            }

            solution.push_front(traceState->selfState); // Adds the start state to the solution trace.
            return solution;
        }
        if (!(std::find(passed.begin(), passed.end(), currentState) != passed.end())) {
            passed.push_back(currentState);
            auto transitions = _transitionFunctions(currentState);

            for (auto transition: transitions) {
                auto successor{currentState};
                transition(successor);

                if (!_invariantFunction(successor)) { // Prevents invalid states being added to waiting.
                    continue;
                }
                waiting.push_back(new trace_node<StateTypeT>{traceState, successor});
            }
        }
    }

    return solution;
}

// The following hash override is implemented in order to compile family.cpp. It is, however, never used and should
// not be used at all. Gives a warning when compiling the program.
template<class StateType, size_t typeSize>
struct std::hash<std::array<StateType, typeSize>> {
    std::size_t operator()(const array<StateType, typeSize> &key) const {
        return NULL;
    }
};

#endif //PUZZLEENGINE_REACHABILITY_HPP