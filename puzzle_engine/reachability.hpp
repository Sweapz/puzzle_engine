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
    parent_state* parentState;
    ActorType selfState;
//    bool compare_parent(ActorType expectedParent);
};

//template<class ActorType>
//bool parent_state<ActorType>::compare_parent(ActorType expectedParent) {
//    if(selfState == expectedParent){
//        return true;
//    } else{
//        return false;
//    }
//}

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

    template<class ValidationFunction>
    std::list<ActorType> findSolutionBreadthFirst(ValidationFunction isGoalState);

    template<class ValidationFunction>
    std::list<ActorType> findSolutionDepthFirst(ValidationFunction isGoalState);

public:
    state_space_t(const ActorType startInputState, successors<ActorType> transFunctions) : startState(startInputState),
                                                                                           transitionFunctions(
                                                                                                   transFunctions) {
    }

    template<class ValidationFunction>
    std::list<ActorType> check(ValidationFunction isGoalState, search_order_t order = search_order_t::breadth_first) {
        std::list<ActorType> solutionTrace;
        switch (order) {
            case breadth_first:
                solutionTrace = findSolutionBreadthFirst(isGoalState);
                break;
            case depth_first:
                solutionTrace = findSolutionDepthFirst(isGoalState);
                break;
        }

        return solutionTrace;
    }
};

template<class ActorType>
template<class ValidationFunction>
std::list<ActorType> state_space_t<ActorType>::findSolutionDepthFirst(ValidationFunction isGoalState) {
    ActorType currentState;
    std::list<ActorType> passed, solution;
    std::list<parent_state<ActorType>*> waiting;

    waiting.push_back(new parent_state<ActorType>{nullptr, startState});

    while (!waiting.empty()) {
        currentState = waiting.front()->selfState;
        auto traceState {waiting.front()};
        waiting.pop_front();

        if (isGoalState(currentState)) {
            while (traceState->parentState != NULL){
                solution.push_front(traceState->selfState);
                traceState = traceState->parentState;
            }

            return solution;
        }
        if (!(std::find(passed.begin(), passed.end(), currentState) != passed.end())) {
            passed.push_back(currentState);
            auto transitions = transitionFunctions.computeStateSuccessors(currentState);

            for (auto transition: transitions) {
                auto successor {currentState};
                transition(successor);

                waiting.push_back(new parent_state<ActorType>{traceState, successor});
            }
        }
    }

    return solution;

    return solution;
}

template<class ActorType>
template<class ValidationFunction>
std::list<ActorType> state_space_t<ActorType>::findSolutionBreadthFirst(ValidationFunction isGoalState) {
    ActorType currentState;
    std::list<ActorType> passed, solution;
    std::list<parent_state<ActorType>*> waiting;

    waiting.push_back(new parent_state<ActorType>{nullptr, startState});

    while (!waiting.empty()) {
        currentState = waiting.front()->selfState;
        auto traceState {waiting.front()};
        waiting.pop_front();

        if (isGoalState(currentState)) {
            while (traceState->parentState != NULL){
                solution.push_front(traceState->selfState);
                traceState = traceState->parentState;
            }

            return solution;
        }
        if (!(std::find(passed.begin(), passed.end(), currentState) != passed.end())) {
            passed.push_back(currentState);
            auto transitions = transitionFunctions.computeStateSuccessors(currentState);

            for (auto transition: transitions) {
                auto successor {currentState};
                transition(successor);

                waiting.push_back(new parent_state<ActorType>{traceState, successor});
            }
        }
    }

    return solution;
}

#endif //PUZZLEENGINE_REACHABILITY_HPP