//
// Created by yufoo1 on 2023/5/23.
//

#ifndef CKKS_DEMO_KEYGENERATOR_H
#define CKKS_DEMO_KEYGENERATOR_H
#include "SecretKeyGenerator.h"
#include "PublicKeyGenerator.h"
class KeyGenerator {
private:
    SecretKeyGenerator secretKeyGenerator;
    PublicKeyGenerator publicKeyGenerator;
public:
    explicit KeyGenerator(int m, int q) {
        secretKeyGenerator = SecretKeyGenerator(m);
        secretKeyGenerator.secret_key_generate();
        publicKeyGenerator = PublicKeyGenerator(m, q);
        publicKeyGenerator.public_key_generate(secretKeyGenerator.get_secret_key());
    }

    auto get_secret_key_generator() {
        return secretKeyGenerator;
    }

    auto get_public_key_generator() {
        return publicKeyGenerator;
    }
};
#endif //CKKS_DEMO_KEYGENERATOR_H
