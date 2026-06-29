#include "Parser.hpp"
#include "Solver.hpp"
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Use: " << argv[0] << " <input.cnf>\n";
        return 1;
    }

    try {
        if (!std::filesystem::exists(argv[1])) {
            throw std::runtime_error("File doesn't exist");
        }

        Formula formula = parse_dimacs(argv[1]);
        Solver solver;
        solver.load_formula(formula);
        bool result = solver.solve();

        if (result) {
            std::cout << "SAT\n";

            auto &assigns = solver.get_assigns();
            for (size_t i = 1; i < assigns.size(); ++i) {
                if (assigns[i] == Value::True) {
                    std::cout << i << " ";
                } else if (assigns[i] == Value::False) {
                    std::cout << "-" << i << " ";
                }
            }
            std::cout << "\n";
        } else {
            std::cout << "UNSAT\n\n";
        }

        std::cout << std::fixed << std::setprecision(6);
        std::cout << solver.get_load_time() << " sec\n";
        std::cout << solver.get_solve_time() << " sec\n";
        std::cout << solver.get_unit_props() << "\n";
        std::cout << solver.get_decision_count() << "\n";

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}