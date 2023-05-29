//
// Created by yufoo1 on 2023/5/23.
//

#ifndef CKKS_DEMO_ENCRYPTOR_H
#define CKKS_DEMO_ENCRYPTOR_H
#include "../include/Polynomial.h"
class Encryptor {
private:
    int m, q;
public:
    explicit Encryptor(int m, int q) {
        this->m = m;
        this->q = q;
    }

    auto Encrypt(Polynomial)
};
#endif //CKKS_DEMO_ENCRYPTOR_H
