//
// Created by yufoo1 on 2023/5/23.
//

#ifndef CKKS_DEMO_SECRETKEYGENERATOR_H
#define CKKS_DEMO_SECRETKEYGENERATOR_H
#include "Polynomial.h"
#include "complex"
#include "random"

class SecretKeyGenerator {
private:
    int m{0};
    Polynomial sk;
public:
    explicit SecretKeyGenerator() = default;

    explicit SecretKeyGenerator(int m) {
        this->m = m;
    }

    auto secret_key_generate() {
        auto n = m / 2;
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        auto gen = std::default_random_engine(seed);
        std::uniform_int_distribution<int> dist(1,120);
        for(int i = 0; i < n; ++i) {
            std::complex<double> coefficient;
            switch (dist(gen) % 3) {
                case 0:
                    coefficient = 0;
                    break;
                case 1:
                    coefficient = 1;
                    break;
                case 2:
                    coefficient = -1;
                    break;
                default:
                    coefficient = 0;
                    break;
            }
            sk.add_term(i, coefficient);
        }
    }

    auto get_secret_key() {
        return sk;
    }
};
#endif //CKKS_DEMO_SECRETKEYGENERATOR_H
