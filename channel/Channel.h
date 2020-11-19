//
// Created by valentinnodan on 11/13/20.
//

#pragma once

#include <vector>
#include <Symbol.h>
#include <cassert>

namespace {
    template<bool constant>
    class ChannelField;

    template<>
    class ChannelField<true> {
    protected:
        double const *data;
    public:

        explicit ChannelField(double const *data1) : data(data1) {}
    };

    template<>
    class ChannelField<false> {
    protected:
        double *data;
    public:
        explicit ChannelField(double *data1) : data(data1) {}
    };

    template<bool constant>
    class Channel : public ChannelField<constant> {
    public:
        using ChannelField<constant>::ChannelField;
        using ChannelField<constant>::data;

        double &operator[](Symbol const &a) {
            assert(a.is_input());
            return data[a.symbol];
        }

        double &operator[](Symbol const &a) const {
            assert(a.is_input());
            return data[a.symbol];
        }
    };
}

class ChannelMatrix {

public:
    Channel<false> operator[](Symbol const &a) {
        assert(a.is_output());
        return Channel<false>(data + a.symbol * input_symbol_num);
    }

    Channel<true> operator[](Symbol const &a) const {
        assert(a.is_output());
        return Channel<true>(data + a.symbol * input_symbol_num);
    }

    double getWN(Message const &y, Message const &c);

    double getWNI(Message const &y, Message const &c, Symbol s);

    double getEps();

    Message BEC(Message c);

private:
    double data[input_symbol_num * output_symbol_num];
};
