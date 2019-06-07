//
// Created by Alberg on 30-05-2019.
//

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

template<class ActorType>
struct parent_state {
    parent_state *parentState;
    ActorType selfState;
};

template<class ActorType>
std::function<std::list<std::function<void(ActorType &)>>(ActorType &)>
successors(std::list<std::function<void(ActorType &)>> transitions(const ActorType &state)) {
    return transitions;
}

template<class ActorType, class CostType = std::nullptr_t>
class state_space_t {
private:
    ActorType startState;
    std::function<std::list<std::function<void(ActorType &)>>(ActorType &)> transitionFunctions;
    std::function<bool(const ActorType &)> invariantFunction;

public:
    state_space_t(const ActorType startInputState,
                  std::function<std::list<std::function<void(ActorType &)>>(ActorType &)> transFunctions,
                  bool invariantFunc(const ActorType &) = [](
                          const ActorType &state) { return true; }) : startState(startInputState),
                                                                      transitionFunctions(
                                                                              transFunctions),
                                                                      invariantFunction(invariantFunc) {
    }

    state_space_t(const ActorType startInputState, const CostType costInput,
                  std::function<std::list<std::function<void(ActorType &)>>(ActorType &)> transFunctions,
                  bool invariantFunc(const ActorType &) = [](
                          const ActorType &state) { return true; },
                  std::function<CostType(const ActorType &state, const CostType &cost)> costFunc = [](
                          const ActorType &state, const CostType &cost) { return CostType{0, 0}; }) : startState(
            startInputState),
                                                                                                      transitionFunctions(
                                                                                                              transFunctions),
                                                                                                      invariantFunction(
                                                                                                              invariantFunc) {
    }

    template<class ValidationFunction>
    std::list<ActorType> check(ValidationFunction isGoalState, search_order_t order = search_order_t::breadth_first);
};

template<class ActorType, class CostType>
template<class ValidationFunction>
std::list<ActorType> state_space_t<ActorType, CostType>::check(ValidationFunction isGoalState, search_order_t order) {
    ActorType currentState;
    parent_state<ActorType> *traceState;
    std::list<ActorType> passed, solution;
    std::list<parent_state<ActorType> *> waiting;

    waiting.push_back(new parent_state<ActorType>{nullptr, startState});

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

        if (!invariantFunction(currentState)) {
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
            auto transitions = transitionFunctions(currentState);

            for (auto transition: transitions) {
                auto successor{currentState};
                transition(successor);

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

namespace std {
    template <typename T, size_t i>
    struct hash<array<T, i>> {
        std::size_t operator() (const array<T, i>& key) const {
            hash<T> ElementHasher;
            size_t res;
            for (int j = 0; j<i;j++)
            {
                res = (res << 1) ^ ElementHasher(key[i]);
            }
            return res;
        }
    };
}

#endif //PUZZLEENGINE_REACHABILITY_HPP