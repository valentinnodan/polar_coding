//
// Created by valentinnodan on 11/13/20.
//

#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>
#include <cassert>
#include <vector>

class Symbol {
public:
    uint8_t symbol;

    Symbol() = default;

    explicit Symbol(int val) noexcept: symbol(val) {
        assert(*(this) == Symbol::ONE || *(this) == Symbol::ZERO || *(this) == Symbol::EPS);
    }

    static const Symbol ZERO;

    static const Symbol ONE;

    static const Symbol EPS;

    friend bool operator==(Symbol const &a, Symbol const &b) {
        return a.symbol == b.symbol;
    }
};

using Message = std::vector<Symbol>;

static_assert(!std::numeric_limits<decltype(Symbol::symbol)>::is_signed);
inline constexpr std::size_t input_symbol_num = 2;
inline constexpr std::size_t output_symbol_num = 3;

//inline constexpr std::size_t symbol_max = std::numeric_limits<decltype(Symbol::symbol)>::max();
