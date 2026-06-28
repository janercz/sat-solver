#include "Parser.hpp"
#include <cmath>
#include <fstream>
#include <sstream>

Formula parse_dimacs(const std::string &filename) {
    std::ifstream file(filename);
    Formula formula;
    std::string line;

    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == 'c') {
            continue;
        }

        // Read header
        if (line[0] == 'p') {
            std::stringstream ss(line);
            std::string temp;
            ss >> temp >> temp >> formula.num_vars;
            continue;
        }

        // Read clauses
        std::stringstream ss(line);
        int lit;
        Clause cls;
        while (ss >> lit && lit != 0) {
            // Convert DIMACS
            bool is_neg = (lit < 0);
            cls.literals.push_back(make_lit(std::abs(lit), is_neg));
        }

        // Add non-empty clauses to formula
        if (!cls.literals.empty()) {
            formula.clauses.push_back(cls);
        }
    }
    return formula;
}