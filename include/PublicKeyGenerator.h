//
// Created by yufoo1 on 2023/5/23.
//

#ifndef CKKS_DEMO_PUBLICKEYGENERATOR_H
#define CKKS_DEMO_PUBLICKEYGENERATOR_H
#include "Polynomial.h"
#include "ErrorGenerator.h"
#include "random"

class PublicKeyGenerator {
private:
    int m{0}, q{0};
    Polynomial pk1, pk2;

    auto public_key_1_generate(Polynomial sk) {
        auto n = m / 2;
        auto errorGenerator = ErrorGenerator(m);
        auto error = errorGenerator.error_generate();
        for(auto term : pk2.get_terms()) {
            pk1.add_term(term.first, {-term.second.real(), -term.second.imag()});
        }
        pk1.mult_poly(std::move(sk));
        pk1.mod_poly_power(n);
        pk1.add_poly(error);
        pk1.mod_poly_coefficient(q);
    }

    auto public_key_2_generate() {
        auto n = m / 2;
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        auto gen = std::default_random_engine(seed);
        std::uniform_int_distribution<int> dist(0,q - 1);
        for(int i = 0; i < n; ++i) {
            pk2.add_term(i, dist(gen));
        }
        pk2.mod_poly_coefficient(q);
    }

public:
    explicit PublicKeyGenerator() = default;

    explicit PublicKeyGenerator(int m, int q) {
        this->m = m;
        this->q = q;
    }

    auto public_key_generate(Polynomial sk) {
        public_key_2_generate();
        public_key_1_generate(std::move(sk));
    }

    auto get_public_key_1() {
        return pk1;
    }

    auto get_public_key_2() {
        return pk2;
    }
};
#endif //CKKS_DEMO_PUBLICKEYGENERATOR_H
