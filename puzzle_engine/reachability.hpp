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
struct state_number {
    ActorType parentState;
    ActorType selfState;
    bool compare_parent(ActorType expectedParent);
};

template<class ActorType>
bool state_number<ActorType>::compare_parent(ActorType expectedParent) {
    if(selfState == expectedParent){
        return true;
    } else{
        return false;
    }
}

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
    state_number<ActorType> traceState;
    std::list<ActorType> waiting, passed, solution;
    std::list<state_number<ActorType>> solutionTrace;

    waiting.push_back(startState);
    solutionTrace.push_back({currentState, startState});

    while (!waiting.empty()) {
        currentState = waiting.back();
        std::cout << currentState << std::endl;
        waiting.pop_back();

        if (isGoalState(currentState)) {
            solution.push_front(solutionTrace.back().selfState);
            traceState = solutionTrace.back();
            solutionTrace.pop_back();

            do {

                if(solutionTrace.back().compare_parent(traceState.parentState)){
                    traceState = solutionTrace.back();
                    solution.push_front(traceState.selfState);
                }
                solutionTrace.pop_back();

            } while(!solutionTrace.empty());

            return solution;
        }
        if (!(std::find(passed.begin(), passed.end(), currentState) != passed.end())) {
            passed.push_back(currentState);
            auto transitions = transitionFunctions.computeStateSuccessors(currentState);

            for (auto transition: transitions) {
                auto successor = currentState;

                transition(successor);
                waiting.push_back(successor);

                solutionTrace.push_back({currentState, successor});
            }
        }
    }

    return solution;
}

template<class ActorType>
template<class ValidationFunction>
std::list<ActorType> state_space_t<ActorType>::findSolutionBreadthFirst(ValidationFunction isGoalState) {
    ActorType currentState;
    state_number<ActorType> traceState;
    std::list<ActorType> waiting, passed, solution;
    std::list<state_number<ActorType>> solutionTrace;

    waiting.push_back(startState);
    solutionTrace.push_back({currentState, startState});

    while (!waiting.empty()) {
        currentState = waiting.front();
        std::cout << currentState << std::endl;
        waiting.pop_front();

        if (isGoalState(currentState)) {
            solution.push_front(solutionTrace.back().selfState);
            traceState = solutionTrace.back();
            solutionTrace.pop_back();

            do {
                if(solutionTrace.back().compare_parent(traceState.parentState)){
                    traceState = solutionTrace.back();
                    solution.push_front(traceState.selfState);
                }
                solutionTrace.pop_back();

            } while(!solutionTrace.empty());

            return solution;
        }
        if (!(std::find(passed.begin(), passed.end(), currentState) != passed.end())) {
            passed.push_back(currentState);
            auto transitions = transitionFunctions.computeStateSuccessors(currentState);

            for (auto transition: transitions) {
                auto successor = currentState;

                transition(successor);
                waiting.push_back(successor);

                solutionTrace.push_back({currentState, successor});
            }
        }
    }

    return solution;
}

#endif //PUZZLEENGINE_REACHABILITY_HPP