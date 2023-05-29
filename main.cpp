#include <iostream>
#include "include/Encoder.h"
#include "include/SecretKeyGenerator.h"
#include "include/PublicKeyGenerator.h"

auto encoder = Encoder(8, 64);

void basic_test() {
    std::cout << "basic test" << std::endl;
    std::cout << encoder.get_unit_root() << std::endl;
    std::cout << encoder.get_power_of_unit_root(4) << std::endl;
    auto m = encoder.get_vandermonde();
    for(auto i : m) {
        for(auto j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    auto basis = encoder.get_sigma_R_basis();
    for(const auto& i : basis) {
        for(auto j : i) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}

void add_test() {
    std::cout << "add test" << std::endl;
    std::vector<std::complex<double>> vec1 = {1, 2, 3, 4};
    std::vector<std::complex<double>> vec2 = {1, -2, 3, -4};
    auto poly1 = encoder.sigma_inverse(vec1);
    auto poly2 = encoder.sigma_inverse(vec2);
    poly1.add_poly(poly2);
    auto vec = encoder.sigma(poly1);
    for(auto i : vec) {
        std::cout << i << std::endl;
    }
}

void mult_test() {
    std::cout << "mult test" << std::endl;
    std::vector<std::complex<double>> vec1 = {1, 2, 3, 4};
    std::vector<std::complex<double>> vec2 = {1, -2, 3, -4};
    auto poly1 = encoder.sigma_inverse(vec1);
    auto poly2 = encoder.sigma_inverse(vec2);
    poly1.mult_poly(poly2);
    poly1.mod_poly_power(4);
    auto vec = encoder.sigma(poly1);
    for(auto i : vec) {
        std::cout << i << std::endl;
    }
}

void encode_test() {
    std::cout << "encode test" << std::endl;
    std::vector<std::complex<double>> vec = {{3, 4}, {2, -1}};
    auto poly = encoder.encode(vec);
    std::cout << poly << std::endl;
}

void decode_test() {
    std::cout << "decode test" << std::endl;
    Polynomial poly;
    poly.add_term(0, 160);
    poly.add_term(1, 90);
    poly.add_term(2, 160);
    poly.add_term(3, 45);
    auto vec = encoder.decode(poly);
    for(auto i : vec) {
        std::cout << i << std::endl;
    }
}

void secret_key_test() {
    std::cout << "secret key test" << std::endl;
    SecretKeyGenerator secretKeyGenerator(8);
    secretKeyGenerator.secret_key_generate();
    std::cout << secretKeyGenerator.get_secret_key() << std::endl;
}

void public_key_test() {
    std::cout << "public key test" << std::endl;
    PublicKeyGenerator publicKeyGenerator(8, 67);
    SecretKeyGenerator secretKeyGenerator(8);
    secretKeyGenerator.secret_key_generate();
    publicKeyGenerator.public_key_generate(secretKeyGenerator.get_secret_key());
    std::cout << publicKeyGenerator.get_public_key_1() << std::endl;
    std::cout << publicKeyGenerator.get_public_key_2() << std::endl;
}

int main() {
    basic_test();
    add_test();
    mult_test();
    encode_test();
    decode_test();
    secret_key_test();
    public_key_test();
    return 0;
}