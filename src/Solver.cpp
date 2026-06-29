#include "Solver.hpp"
#include <algorithm>
#include <chrono>

static Value get_literal_value(Lit p, const std::vector<Value> &assigns) {
    Var v = lit_to_var(p);
    if (assigns[v] == Value::None) {
        return Value::None;
    }

    bool is_pos = is_positive(p);
    if (assigns[v] == Value::True) {
        return is_pos ? Value::True : Value::False;
    }
    return is_pos ? Value::False : Value::True;
}

void Solver::load_formula(const Formula &formula) {
    auto start = std::chrono::high_resolution_clock::now();

    vars = formula.num_vars;
    clauses = formula.clauses;

    watches.assign(2 * vars + 2, std::vector<int>());
    assigns.assign(vars + 1, Value::None);
    levels.assign(vars + 1, -1);
    tries.assign(vars + 1, 0);

    for (size_t i = 0; i < clauses.size(); ++i) {
        if (clauses[i].literals.size() >= 2) {
            watches[negate_lit(clauses[i].literals[0])].push_back(i);
            watches[negate_lit(clauses[i].literals[1])].push_back(i);
        } else if (clauses[i].literals.size() == 1) {
            enqueue(clauses[i].literals[0]);
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    load_time = std::chrono::duration<double>(stop - start).count();
}

bool Solver::solve() {
    auto start = std::chrono::high_resolution_clock::now();
    bool result;
    while (true) {
        if (!propagate()) {
            if (!backtrack()) {
                result = false; // UNSAT
                break;
            }
        } else {
            if (trail.size() == static_cast<size_t>(vars)) {
                result = true; // SAT
                break;
            } else {
                // Pick a literal
                Lit next_p = -1;
                for (int i = 1; i <= vars; ++i) {
                    if (assigns[i] == Value::None) {
                        next_p = make_lit(i, false);
                        break;
                    }
                }
                if (next_p != -1) {
                    assume(next_p);
                }
            }
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    solve_time = std::chrono::duration<double>(stop - start).count();

    return result;
}

bool Solver::enqueue(Lit p) {
    Value val = get_literal_value(p, assigns);
    if (val == Value::True) {
        return true;
    }

    if (val == Value::False) {
        return false;
    }

    Var v = lit_to_var(p);
    assigns[v] = is_positive(p) ? Value::True : Value::False;
    levels[v] = trail_lim.size();
    tries[v]++;

    prop_q.push(p);
    trail.push_back(p);

    unit_props++;
    return true;
}

void Solver::assume(Lit p) {
    trail_lim.push_back(trail.size());
    enqueue(p);

    decision_count++;
}

bool Solver::propagate() {
    while (!prop_q.empty()) {
        Lit p = prop_q.front();
        prop_q.pop();

        std::vector<int> tmp = watches[p];
        watches[p].clear();

        for (size_t i = 0; i < tmp.size(); ++i) {
            if (!propagate_clause(tmp[i], p)) {
                for (size_t k = i + 1; k < tmp.size(); ++k) {
                    watches[p].push_back(tmp[k]);
                }
                while (!prop_q.empty())
                    prop_q.pop();
                return false;
            }
        }
    }
    return true;
}

bool Solver::propagate_clause(int cls, Lit p) {
    Clause &cl = clauses[cls];

    if (cl.literals[1] != negate_lit(p)) {
        std::swap(cl.literals[0], cl.literals[1]);
    }

    if (get_literal_value(cl.literals[0], assigns) == Value::True) {
        watches[p].push_back(cls);
        return true;
    }

    for (size_t i = 2; i < cl.literals.size(); ++i) {
        if (get_literal_value(cl.literals[i], assigns) != Value::False) {
            std::swap(cl.literals[1], cl.literals[i]);
            watches[negate_lit(cl.literals[1])].push_back(cls);
            return true;
        }
    }

    watches[p].push_back(cls);
    return enqueue(cl.literals[0]);
}

void Solver::undo_one() {
    Lit p = trail.back();
    Var v = lit_to_var(p);

    if (!trail_lim.empty() && trail_lim.back() == static_cast<int>(trail.size() - 1)) {
        trail_lim.pop_back();
    }
    trail.pop_back();

    assigns[v] = Value::None;
    levels[v] = -1;
    tries[v] = 0;
}

bool Solver::backtrack() {
    while (true) {
        if (trail_lim.empty()) {
            return false;
        }

        while (!trail.empty()) {
            bool is_decision = (!trail_lim.empty() && trail_lim.back() == static_cast<int>(trail.size() - 1));
            if (is_decision) {
                break;
            }
            undo_one();
        }

        Lit p = trail.back();
        Var v = lit_to_var(p);

        if (tries[v] < 2) {
            int saved_tries = tries[v];
            undo_one();
            tries[v] = saved_tries;

            if (enqueue(negate_lit(p))) {
                return true;
            }
        } else {
            undo_one();
        }
    }
}