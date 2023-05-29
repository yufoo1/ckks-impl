#include <iostream>
#include "include/Encoder.h"
#include "include/SecretKeyGenerator.h"
#include "include/PublicKeyGenerator.h"
#include "include/Encryptor.h"

auto encoder = Encoder(8, 64);

auto basic_test() {
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

auto add_test() {
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

auto mult_test() {
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

auto encode_decode_test() {
    std::cout << "encode test" << std::endl;
    std::vector<std::complex<double>> vec = {{3, 4}, {2, -1}};
    auto poly = encoder.encode(vec);
    std::cout << poly << std::endl;
    std::cout << "decode test" << std::endl;
    auto new_vec = encoder.decode(poly);
    for(auto i : new_vec) {
        std::cout << i << std::endl;
    }
}

auto decrypt_encrypt_test() {
    auto m = 8, q = 67;
    std::cout << "secret key test" << std::endl;
    SecretKeyGenerator secretKeyGenerator(m);
    secretKeyGenerator.secret_key_generate();
    std::cout << secretKeyGenerator.get_secret_key() << std::endl;
    std::cout << "public key test" << std::endl;
    PublicKeyGenerator publicKeyGenerator(m, q);
    publicKeyGenerator.public_key_generate(secretKeyGenerator.get_secret_key());
    std::cout << publicKeyGenerator.get_public_key_1() << std::endl;
    std::cout << publicKeyGenerator.get_public_key_2() << std::endl;
    std::cout << "encrypt test" << std::endl;
    std::vector<std::complex<double>> vec = {{3, 4}, {2, -1}};
    auto poly = encoder.encode(vec);
    auto encryptor = Encryptor(m, q);
    auto encrypt_poly = encryptor.encrypt(poly, publicKeyGenerator.get_public_key_1(), publicKeyGenerator.get_public_key_2());
    std::cout << encrypt_poly.first << std::endl;
    std::cout << encrypt_poly.second << std::endl;
    std::cout << "decrypt test" << std::endl;
    auto decrypt_poly = encryptor.decrypt(encrypt_poly.first, encrypt_poly.second, secretKeyGenerator.get_secret_key());
    auto new_vec = encoder.decode(decrypt_poly);
    for(auto i : new_vec) {
        std::cout << i << std::endl;
    }
}

int main() {
    basic_test();
    add_test();
    mult_test();
    encode_decode_test();
    decrypt_encrypt_test();
    return 0;
}
