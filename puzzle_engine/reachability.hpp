

#ifndef PUZZLEENGINE_REACHABILITY_HPP
#define PUZZLEENGINE_REACHABILITY_HPP

#include <vector>
#include <list>
#include <functional>
#include <iostream>
#include <algorithm>
#include <typeinfo>


enum search_order_t {
    breadth_first, depth_first
};

template<class ActorType, class CostType = std::nullptr_t>
struct parent_state {
    parent_state *parentState;
    ActorType selfState;
    CostType stateCost;
};

template<class ActorType>
std::function<std::list<std::function<void(ActorType &)>>(ActorType &)>
successors(std::list<std::function<void(ActorType &)>> transitions(const ActorType &state)) {
    return transitions;
}

template<class ActorType, class CostType = std::nullptr_t>
class state_space_t {
private:
    ActorType _startState;
    CostType _initialCost;
    std::function<std::list<std::function<void(ActorType &)>>(ActorType &)> _transitionFunctions;
    std::function<bool(const ActorType &)> _invariantFunction;
    std::function<CostType(const ActorType &state, const CostType &cost)> _costFunction;
    bool _isCostEnabled = false;

    template<class ValidationFunction>
    std::list<ActorType> solveOrder(ValidationFunction isGoalState, search_order_t order);

    template<class ValidationFunction>
    std::list<ActorType> solveCost(ValidationFunction isGoalState);

public:
    state_space_t(const ActorType startInputState,
                  std::function<std::list<std::function<void(ActorType &)>>(ActorType &)> transFunctions,
                  bool invariantFunc(const ActorType &) = [](
                          const ActorType &state) { return true; }) : _startState(startInputState),
                                                                      _transitionFunctions(
                                                                              transFunctions),
                                                                      _invariantFunction(invariantFunc) {
    }

    state_space_t(const ActorType startInputState, const CostType costInput,
                  std::function<std::list<std::function<void(ActorType &)>>(ActorType &)> transFunctions,
                  bool invariantFunc(const ActorType &) = [](
                          const ActorType &state) { return true; },
                  std::function<CostType(const ActorType &state, const CostType &cost)> costFunc = [](
                          const ActorType &state, const CostType &cost) { return CostType{0, 0}; }) : _startState(
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
    std::list<ActorType> check(ValidationFunction isGoalState, search_order_t order = search_order_t::breadth_first);
};

template<class ActorType, class CostType>
template<class ValidationFunction>
std::list<ActorType> state_space_t<ActorType, CostType>::check(ValidationFunction isGoalState, search_order_t order) {
    std::list<ActorType> solution;

    if (_isCostEnabled) {
        solution = solveCost(isGoalState);
    } else {
        solution = solveOrder(isGoalState, order);
    }

    return solution;
}

template<class ActorType, class CostType>
template<class ValidationFunction>
std::list<ActorType> state_space_t<ActorType, CostType>::solveCost(ValidationFunction isGoalState) {
    ActorType currentState;
    CostType itCost {_initialCost}, newCost;
    parent_state<ActorType> *traceState;
    std::list<ActorType> passed, solution;
    std::list<parent_state<ActorType> *> waiting;

    waiting.push_back(new parent_state<ActorType>{nullptr, _startState});

    while (!waiting.empty()) {
        currentState = waiting.back()->selfState;
        traceState = waiting.back();
        waiting.pop_back();
        itCost = newCost;

        if (!_invariantFunction(currentState)) {
            continue;
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
                newCost = _costFunction(successor, itCost);
//                std::cout << newCost.depth << ", " << newCost.noise << std::endl;
                waiting.push_back(new parent_state<ActorType>{traceState, successor});
            }
        }
    }

    return solution;
}

template<class ActorType, class CostType>
template<class ValidationFunction>
std::list<ActorType>
state_space_t<ActorType, CostType>::solveOrder(ValidationFunction isGoalState, search_order_t order) {
    ActorType currentState;
    parent_state<ActorType> *traceState;
    std::list<ActorType> passed, solution;
    std::list<parent_state<ActorType> *> waiting;

    waiting.push_back(new parent_state<ActorType>{nullptr, _startState});

    while (!waiting.empty()) {
        switch (order) {
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

                if (!_invariantFunction(successor)) {
                    continue;
                }
                waiting.push_back(new parent_state<ActorType>{traceState, successor});
            }
        }
    }

//    std::list<std::list<ActorType>> returnList;
//    returnList.push_front(solution);
    return solution;
}

void log(std::string message) {
    std::cout << message << std::endl;
}

template<typename StateType, size_t typeSize>
struct std::hash<std::array<StateType, typeSize>> {
    std::size_t operator()(const array<StateType, typeSize> &key) const {
        std::hash<StateType> hashElement;
        size_t result;
        for (int index = 0; index < typeSize; index++) {
            result = (result << 1) ^ hashElement(key[typeSize]);
        }
        return result;
    }
};


#endif //PUZZLEENGINE_REACHABILITY_HPP