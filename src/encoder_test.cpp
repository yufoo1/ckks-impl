#include <iostream>
#include "../include/Encoder.h"

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
    poly1.mod_poly(4);
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

int main() {
    basic_test();
    add_test();
    mult_test();
    encode_test();
    decode_test();
    return 0;
}
