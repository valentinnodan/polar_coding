//
// Created by valentinnodan on 11/13/20.
//

#pragma once

#include <cassert>
#include <cstdint>
#include <limits>
#include <ostream>
#include <type_traits>
#include <vector>

class Symbol {
public:
    uint8_t symbol{0};

    enum values {
        symbol_zero = 0,
        symbol_one = 1,
        symbol_eps = 2,
    };

    Symbol() = default;

    constexpr explicit Symbol(int val) noexcept: symbol(val) {
        assert(is_valid());
    }

    constexpr friend bool operator==(Symbol const &a, Symbol const &b) {
        return a.symbol == b.symbol;
    }

    [[nodiscard]] constexpr bool is_input() const {
        return this->symbol == symbol_one || this->symbol == symbol_zero;
    }

    [[nodiscard]] constexpr bool is_output() const {
        return this->symbol == symbol_one || this->symbol == symbol_zero || this->symbol == symbol_eps;
    }

    [[nodiscard]] constexpr bool is_valid() const {
        return is_output();
    }

    constexpr Symbol &operator+=(Symbol const &b) {
        assert(is_input() && b.is_input());
        symbol = (symbol + b.symbol) % 2;
        return *(this);
    }

    constexpr Symbol &operator*=(Symbol const &b) {
        assert(is_input() && b.is_input());
        symbol = (symbol * b.symbol);
        return *(this);
    }

    friend std::ostream &operator<<(std::ostream &out, Symbol const &s) {
        out << (int) s.symbol;
        return out;
    }

    constexpr friend Symbol operator+(Symbol const &a, Symbol const &b) {
        assert(a.is_input() && b.is_input());
        return Symbol((a.symbol + b.symbol) % 2);
    }

    constexpr friend Symbol operator*(Symbol const &a, Symbol const &b) {
        assert(a.is_input() && b.is_input());
        return Symbol((a.symbol * b.symbol));
    }

    constexpr explicit operator uint64_t() const {
        return symbol;
    }
};

namespace SymbolConsts {
    inline constexpr auto ZERO = Symbol(Symbol::symbol_zero);

    inline constexpr auto ONE = Symbol(Symbol::symbol_one);

    inline constexpr auto EPS = Symbol(Symbol::symbol_eps);
}

using Message = std::vector<Symbol>;

static_assert(!std::numeric_limits<decltype(Symbol::symbol)>::is_signed);
inline constexpr std::size_t input_symbol_num = 2;
inline constexpr std::size_t output_symbol_num = 3;

