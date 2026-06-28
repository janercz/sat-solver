#ifndef PARSER_HPP
#define PARSER_HPP

#include "Types.hpp"
#include <string>
#include <vector>

// Structure for the parsed formula
struct Formula {
    int num_vars;
    std::vector<Clause> clauses;
};

// Function to read DIMACS file
Formula parse_dimacs(const std::string &filename);

#endif