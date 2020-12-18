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
    double symbol{0};
    static constexpr double EPS = 0.000001;

    Symbol() = default;

    constexpr explicit Symbol(double val) noexcept: symbol(val) {
    }


    constexpr friend bool operator==(Symbol const &a, Symbol const &b) {
        return a.symbol -  b.symbol < 0.000001;
    }

    [[nodiscard]] constexpr bool is_input() const {
        return this->symbol - 1 < EPS || this->symbol < EPS;
    }

    constexpr Symbol &operator+=(Symbol const &b) {
        assert(is_input() && b.is_input());
        symbol = (int)(symbol + b.symbol) % 2;
        return *(this);
    }

    constexpr Symbol &operator*=(Symbol const &b) {
        assert(is_input() && b.is_input());
        symbol = (symbol * b.symbol);
        return *(this);
    }

    friend std::ostream &operator<<(std::ostream &out, Symbol const &s) {
        out << s.symbol;
        return out;
    }

    constexpr friend Symbol operator+(Symbol const &a, Symbol const &b) {
        assert(a.is_input() && b.is_input());
        assert(a.symbol - 1.0 < EPS || a.symbol < EPS);
        assert(b.symbol - 1.0 < EPS || b.symbol < EPS);
        return Symbol((int)(a.symbol + b.symbol) % 2);
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
    inline constexpr auto ZERO = Symbol(0);

    inline constexpr auto ONE = Symbol(1);

}

using Message = std::vector<Symbol>;

