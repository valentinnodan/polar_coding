//
// Created by valentinnodan on 11/13/20.
//

#pragma once

#include <vector>
#include <Symbol.h>
#include <cassert>

class ChannelMatrix {
    class Channel {
    public:
        explicit Channel(double *other) : data(other) {}

        double operator[](Symbol const &a) {
            assert(a == Symbol::ONE || a == Symbol::ZERO);
            return data[a.symbol];
        }

    private:
        double *data;
    };

public:
    Channel operator[](Symbol const &a) {
        assert(a == Symbol::ONE || a == Symbol::ZERO || a == Symbol::EPS);
        return Channel(data + a.symbol * input_symbol_num);
    }

    double getWN(Message const &y, Message const &c);

    double getWNI(Message const &y, Message const &c, Symbol s);

    Message BEC(Message c);

private:
    double data[input_symbol_num * output_symbol_num];
};
