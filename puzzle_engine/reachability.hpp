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
class successors {
private:
    std::function<std::list<std::function<void(ActorType &)>>(ActorType &)> transitionFunctions;

public:
    explicit successors<ActorType>(std::list<std::function<void(ActorType &)>> transitions(const ActorType &state))
            : transitionFunctions(transitions) {
    };

    std::list<std::function<void(ActorType &)>> computeStateSuccessors(ActorType &state) {
        return transitionFunctions(state);
    }
};

template<class ActorType>
class state_space_t {
private:
    ActorType startState;
    successors<ActorType> transitionFunctions;
    std::function<bool(const ActorType &)> invariantFunction;

public:
    state_space_t(const ActorType startInputState, successors<ActorType> transFunctions,
                  bool invariantFunc(const ActorType &) = [](
                          const ActorType &state) { return true; }) : startState(startInputState),
                                                                      transitionFunctions(
                                                                              transFunctions),
                                                                      invariantFunction(invariantFunc) {
    }

    template<class ValidationFunction>
    std::list<ActorType> check(ValidationFunction isGoalState, search_order_t order = search_order_t::breadth_first);
};

template<class ActorType>
template<class ValidationFunction>
std::list<ActorType> state_space_t<ActorType>::check(ValidationFunction isGoalState, search_order_t order) {
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

        if (!invariantFunction(currentState)){
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
            auto transitions = transitionFunctions.computeStateSuccessors(currentState);

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

#endif //PUZZLEENGINE_REACHABILITY_HPP