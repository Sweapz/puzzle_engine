/**
 * Solution to a frog leap puzzle:
 * http://arcade.modemhelp.net/play-4863.html
 * Author: Marius Mikucionis <marius@cs.aau.dk>
 * Compile and run:
 * g++ -std=c++17 -pedantic -Wall -DNDEBUG -O3 -o frogs frogs.cpp && ./frogs
 */
#include "reachability.hpp" // your header-only library solution

#include <iostream>
#include <list>
//#include <functional> // std::function

enum class frog_t { empty, green, brown };
using stones_t = std::vector<frog_t>;

std::list<std::function<void(stones_t&)>> transitions(const stones_t& stones) {
	auto res = std::list<std::function<void(stones_t&)>>{};
	if (stones.size()<2)
		return res;
	auto i=0u;
	while (i < stones.size() && stones[i]!=frog_t::empty) ++i; // find empty stone
	if (i==stones.size())
		return res;  // did not find empty stone
	// explore moves to fill the empty from left to right (only green can do that):
	if (i > 0 && stones[i-1]==frog_t::green)
		res.push_back([i](stones_t& s){ // green jump to next
						  s[i-1] = frog_t::empty;
						  s[i]   = frog_t::green;
					  });
	if (i > 1 && stones[i-2]==frog_t::green)
		res.push_back([i](stones_t& s){ // green jump over 1
						  s[i-2] = frog_t::empty;
						  s[i]   = frog_t::green;
					  });
	// explore moves to fill the empty from right to left (only brown can do that):
	if (i < stones.size()-1 && stones[i+1]==frog_t::brown) {
		res.push_back([i](stones_t& s){ // brown jump to next
						  s[i+1] = frog_t::empty;
						  s[i]   = frog_t::brown;
					  });
	}
	if (i < stones.size()-2 && stones[i+2]==frog_t::brown) {
		res.push_back([i](stones_t& s){ // brown jump over 1
						  s[i+2]=frog_t::empty;
						  s[i]=frog_t::brown;
					  });
	}
	return res;
}

std::ostream& operator<<(std::ostream& os, const stones_t& stones) {
	for (auto&& stone: stones)
		switch (stone) {
		case frog_t::green: os << "G"; break;
		case frog_t::empty: os << "_"; break;
		case frog_t::brown: os << "B"; break;
		default: os << "?"; break; // something went terribly wrong
		}
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::list<const stones_t*>& trace) {
	for (auto stones: trace)
		os << "State of " << stones->size() << " stones: " << *stones << '\n';
	return os;
}

void show_successors(const stones_t& state, const size_t level=0) {
	// Caution: this function uses recursion, which is not suitable for solving puzzles!!
	// 1) some state spaces can be deeper than stack allows.
	// 2) it can only perform depth-first search
	// 3) it cannot perform breadth-first search, cheapest-first, greatest-first etc.
	auto trans = transitions(state); // compute the transitions
	std::cout << std::string(level*2, ' ')
			  << "state " << state << " has " << trans.size() << " transitions";
	if (trans.empty())
		std::cout << '\n';
	else
		std::cout << ", leading to:\n";
	for (auto& t: trans) {
		auto succ = state; // copy the original state
		t(succ); // apply the transition on the state to compute successor
		show_successors(succ, level+1);
	}
}

void explain(){
	const auto start = stones_t{{ frog_t::green, frog_t::green, frog_t::empty,
								  frog_t::brown, frog_t::brown }};
	std::cout << "Leaping frog puzzle start: " << start << '\n';
	show_successors(start);
	const auto finish = stones_t{{ frog_t::brown, frog_t::brown, frog_t::empty,
								   frog_t::green, frog_t::green }};
	std::cout << "Leaping frog puzzle start: " << start << ", finish: " << finish << '\n';
	auto space = state_space_t<stones_t >(start, successors<stones_t>(transitions));// define state space
	// explore the state space and find the solutions satisfying goal:
	std::cout << "--- Solve with default (breadth-first) search: ---\n";
	auto solutions = space.check([&finish](const stones_t& state){ return state==finish; });
	for (auto&& trace: solutions) { // iterate through solutions:
		std::cout << "Solution: a trace of " << trace.size() << " states\n";
		std::cout << trace; // print solution
	}
}

void solve(size_t frogs, search_order_t order = search_order_t::breadth_first){
	const auto stones = frogs*2+1; // frogs on either side and 1 empty in the middle
	auto start = stones_t(stones, frog_t::empty);  // initially all empty
	auto finish = stones_t(stones, frog_t::empty); // initially all empty
	while (frogs-->0) { // count down from frogs-1 to 0 and put frogs into positions:
		start[frogs] = frog_t::green;                  // green on left
		start[start.size()-frogs-1] = frog_t::brown;   // brown on right
		finish[frogs] = frog_t::brown;                 // brown on left
		finish[finish.size()-frogs-1] = frog_t::green; // green on right
	}
	std::cout << "Leaping frog puzzle start: " << start << ", finish: " << finish << '\n';
	auto space = state_space_t<stones_t >(std::move(start), successors<stones_t>(transitions));
	auto solutions = space.check(
		[finish=std::move(finish)](const stones_t& state){ return state==finish; },
		order);
    std::cout << "Solution: trace of " << solutions.size() << " states\n";
    for (auto&& trace: solutions) {
		std::cout << trace << std::endl;
	}
}

int main(){
    //explain();
	std::cout << "--- Solve with depth-first search: ---\n";
	solve(2, search_order_t::depth_first);
    std::cout << "--- Solve with breadth-first search: ---\n";
    solve(2); // 20 frogs may take >5.8GB of memory
}
/** Sample output:
Leaping frog puzzle start: GG_BB
state GG_BB has 4 transitions, leading to:
  state G_GBB has 2 transitions, leading to:
    state _GGBB has 0 transitions
    state GBG_B has 2 transitions, leading to:
      state GB_GB has 2 transitions, leading to:
        state _BGGB has 1 transitions, leading to:
          state B_GGB has 0 transitions
        state GBBG_ has 1 transitions, leading to:
          state GBB_G has 0 transitions
      state GBGB_ has 1 transitions, leading to:
        state GB_BG has 2 transitions, leading to:
          state _BGBG has 1 transitions, leading to:
            state B_GBG has 1 transitions, leading to:
              state BBG_G has 1 transitions, leading to:
                state BB_GG has 0 transitions
          state GBB_G has 0 transitions
  state _GGBB has 0 transitions
  state GGB_B has 2 transitions, leading to:
    state G_BGB has 2 transitions, leading to:
      state _GBGB has 1 transitions, leading to:
        state BG_GB has 2 transitions, leading to:
          state B_GGB has 0 transitions
          state BGBG_ has 1 transitions, leading to:
            state BGB_G has 1 transitions, leading to:
              state B_BGG has 1 transitions, leading to:
                state BB_GG has 0 transitions
      state GB_GB has 2 transitions, leading to:
        state _BGGB has 1 transitions, leading to:
          state B_GGB has 0 transitions
        state GBBG_ has 1 transitions, leading to:
          state GBB_G has 0 transitions
    state GGBB_ has 0 transitions
  state GGBB_ has 0 transitions
Leaping frog puzzle start: GG_BB, finish: BB_GG
--- Solve with default (breadth-first) search: ---
Solution: a trace of 9 states
State of 5 stones: GG_BB
State of 5 stones: G_GBB
State of 5 stones: GBG_B
State of 5 stones: GBGB_
State of 5 stones: GB_BG
State of 5 stones: _BGBG
State of 5 stones: B_GBG
State of 5 stones: BBG_G
State of 5 stones: BB_GG
--- Solve with depth-first search: ---
Leaping frog puzzle start: GG_BB, finish: BB_GG
Solution: trace of 9 states
State of 5 stones: GG_BB
State of 5 stones: GGB_B
State of 5 stones: G_BGB
State of 5 stones: _GBGB
State of 5 stones: BG_GB
State of 5 stones: BGBG_
State of 5 stones: BGB_G
State of 5 stones: B_BGG
State of 5 stones: BB_GG
Leaping frog puzzle start: GGGG_BBBB, finish: BBBB_GGGG
Solution: trace of 25 states
State of 9 stones: GGGG_BBBB
State of 9 stones: GGG_GBBBB
State of 9 stones: GGGBG_BBB
State of 9 stones: GGGBGB_BB
State of 9 stones: GGGB_BGBB
State of 9 stones: GG_BGBGBB
State of 9 stones: G_GBGBGBB
State of 9 stones: GBG_GBGBB
State of 9 stones: GBGBG_GBB
State of 9 stones: GBGBGBG_B
State of 9 stones: GBGBGBGB_
State of 9 stones: GBGBGB_BG
State of 9 stones: GBGB_BGBG
State of 9 stones: GB_BGBGBG
State of 9 stones: _BGBGBGBG
State of 9 stones: B_GBGBGBG
State of 9 stones: BBG_GBGBG
State of 9 stones: BBGBG_GBG
State of 9 stones: BBGBGBG_G
State of 9 stones: BBGBGB_GG
State of 9 stones: BBGB_BGGG
State of 9 stones: BB_BGBGGG
State of 9 stones: BBB_GBGGG
State of 9 stones: BBBBG_GGG
State of 9 stones: BBBB_GGGG

*/
