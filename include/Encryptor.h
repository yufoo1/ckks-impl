//
// Created by yufoo1 on 2023/5/23.
//

#ifndef CKKS_DEMO_ENCRYPTOR_H
#define CKKS_DEMO_ENCRYPTOR_H
#include <utility>

#include "../include/Polynomial.h"
#include "../include/ErrorGenerator.h"
class Encryptor {
private:
    int m, q;
public:
    explicit Encryptor(int m, int q) {
        this->m = m;
        this->q = q;
    }

    auto encrypt(Polynomial poly, Polynomial pk1, Polynomial pk2) const {
        poly.add_poly(std::move(pk1));
        auto errorGenerator = ErrorGenerator(m);
        poly.add_poly(errorGenerator.error_generate());
        poly.mod_poly_coefficient(q);
        pk2.add_poly(errorGenerator.error_generate());
        pk2.mod_poly_coefficient(q);
        return std::pair<Polynomial, Polynomial>{poly, pk2};
    }

    auto decrypt(Polynomial c1, Polynomial c2, Polynomial sk) const {
        auto n = m / 2;
        c2.mult_poly(std::move(sk));
        c2.mod_poly_power(n);
        c1.add_poly(c2);
        c1.mod_poly_coefficient(q);
        return c1;
    }
};
#endif //CKKS_DEMO_ENCRYPTOR_H
