#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "Parser.hpp"
#include "Types.hpp"
#include <queue>
#include <vector>

class Solver {
  private:
    // Variable count
    int vars;

    // All clauses
    std::vector<Clause> clauses;

    // Watch lists
    std::vector<std::vector<int>> watches;

    // Propagation queue
    std::queue<Lit> prop_q;

    // Variable values
    std::vector<Value> assigns;

    // Decision levels
    std::vector<int> levels;

    // Try counts
    std::vector<int> tries;

    // Assignment stack
    std::vector<Lit> trail;

    // Level boundaries
    std::vector<int> trail_lim;

  public:
    // Initialize solver
    void load_formula(const Formula &formula);

    // Main solver loop
    bool solve();

  private:
    bool enqueue(Lit p);
    void assume(Lit p);
    bool propagate();
    bool propagate_clause(int cls, Lit p);
    void undo_one();
    bool backtrack();
};

#endif