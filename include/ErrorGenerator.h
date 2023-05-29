//
// Created by yufoo1 on 2023/5/29.
//

#ifndef CKKS_DEMO_ERRORGENERATOR_H
#define CKKS_DEMO_ERRORGENERATOR_H
#include "random"
#include "../include/Polynomial.h"

class ErrorGenerator {
private:
    int m;
public:
    explicit ErrorGenerator(int m) {
        this->m = m;
    }

    auto error_generate() const {
        auto n = m / 2;
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        auto gen = std::default_random_engine(seed);
        std::normal_distribution<double> dist(0, double(8 / std::sqrt(2 * M_PI)));
        Polynomial error;
        for(int i = 0; i < n; ++i) {
            error.add_term(i, floor(dist(gen)));
        }
        return error;
    }
};
#endif //CKKS_DEMO_ERRORGENERATOR_H
