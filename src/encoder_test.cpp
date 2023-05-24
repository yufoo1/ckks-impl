#include <iostream>
#include "../include/Encoder.h"

auto encoder = Encoder(8);

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

int main() {
    basic_test();
    add_test();
    mult_test();
    return 0;
}
