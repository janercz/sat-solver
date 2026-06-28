#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>
#include <vector>

enum class Value : int8_t {
    True = 1,
    False = 0,
    None = -1
};

using Var = int; // 1, 2 ... n
using Lit = int; // 2k or 2k+1

// Convert literal to variable
inline Var lit_to_var(Lit l) {
    return l >> 1; // l/2
}

// Negate literal
inline Lit negate_lit(Lit l) {
    return l ^ 1; // l xor 1
}

// Check if literal is positive
inline bool is_positive(Lit l) {
    return (l & 1) == 0; // last bit 0
}

// Create literal from variable and sign
inline Lit make_lit(Var v, bool is_negative) {
    return (v << 1) | (is_negative ? 1 : 0); // 2*Var + sgn
}

struct Clause {
    std::vector<Lit> literals;
};

#endif