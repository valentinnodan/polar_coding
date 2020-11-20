//
// Created by valentinnodan on 11/13/20.
//

#pragma once

#include <vector>
#include <Symbol.h>
#include <cassert>

namespace Internal {
    template<bool constant>
    class ChannelBase;

    template<>
    class ChannelBase<true> {
    protected:
        double const *data;
    public:
        explicit ChannelBase(double const *data1) : data(data1) {}

        double const &operator[](Symbol const &a) const {
            assert(a.is_input());
            return data[a.symbol];
        }
    };

    template<>
    class ChannelBase<false> {
    protected:
        double *data;
    public:
        explicit ChannelBase(double *data1) : data(data1) {}

        double const &operator[](Symbol const &a) const {
            assert(a.is_input());
            return data[a.symbol];
        }

        double &operator[](Symbol const &a) {
            assert(a.is_input());
            return data[a.symbol];
        }
    };

    template<bool constant>
    class Channel : public ChannelBase<constant> {
    public:
        using ChannelBase<constant>::ChannelBase;
        using ChannelBase<constant>::data;
        using ChannelBase<constant>::operator[];
    };

    Channel(double *) -> Channel<false>;
    Channel(double const *) -> Channel<true>;
}

class ChannelMatrix {
private:
    double data[input_symbol_num * output_symbol_num]{};
public:

    ChannelMatrix(double p, double q) {
        (*this)[SymbolConsts::ZERO][SymbolConsts::ZERO] = 1 - p - q;
        (*this)[SymbolConsts::EPS][SymbolConsts::ZERO] = q;
        (*this)[SymbolConsts::ONE][SymbolConsts::ZERO] = p;
        (*this)[SymbolConsts::ZERO][SymbolConsts::ONE] = p;
        (*this)[SymbolConsts::EPS][SymbolConsts::ONE] = q;
        (*this)[SymbolConsts::ONE][SymbolConsts::ONE] = 1 - p - q;
    }

    auto operator[](Symbol const &a) -> decltype(Internal::Channel(data + a.symbol * input_symbol_num)) {
        assert(a.is_output());
        return Internal::Channel(data + a.symbol * input_symbol_num);
    }

    auto operator[](Symbol const &a) const -> decltype(Internal::Channel(data + a.symbol * input_symbol_num)) {
        assert(a.is_output());
        return Internal::Channel(data + a.symbol * input_symbol_num);
    }

    [[nodiscard]] double getWN(Message const &y, Message const &c) const;

    [[nodiscard]] double getWNI(Message const &y, Message const &c, Symbol s) const;

    [[nodiscard]] double getEps() const;

    Message BEC(Message const &c);
};
