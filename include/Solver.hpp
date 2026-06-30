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

    // Heuristics
    std::vector<double> activity;
    double x = 1.0;

    // Statistics
    long long unit_props = 0;
    long long decision_count = 0;
    double load_time = 0.0;
    double solve_time = 0.0;

  public:
    // Initialize solver
    void load_formula(const Formula &formula);

    // Main solver loop
    bool solve();

    // Getters
    double get_load_time() const { return load_time; }
    double get_solve_time() const { return solve_time; }
    long long get_unit_props() const { return unit_props; }
    long long get_decision_count() const { return decision_count; }
    const std::vector<Value> &get_assigns() const { return assigns; }

  private:
    bool enqueue(Lit p);
    void assume(Lit p);
    bool propagate();
    bool propagate_clause(int cls, Lit p);
    void undo_one();
    bool backtrack();
};

#endif