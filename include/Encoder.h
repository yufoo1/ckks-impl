//
// Created by yufoo1 on 2023/5/23.
//

#ifndef CKKS_DEMO_ENCODER_H
#define CKKS_DEMO_ENCODER_H
#include <utility>

#include "complex"
#include "vector"
#include "Polynomial.h"
#include "iostream"
#include "Eigen/Dense"
#include "random"

class Encoder {
private:
    int m;
    int scale;

public:
    explicit Encoder(int m, int scale) {
        this->m = m;
        this->scale = scale;
    }

    auto get_unit_root() const {
        return std::complex<double>(std::cos(2 * M_PI / m), std::sin(2 * M_PI / m));
    }

    auto get_power_of_unit_root(int power) const {
        return std::pow(get_unit_root(), power);
    }

    auto get_vandermonde() const {
        auto n = m / 2;
        auto matrix = std::vector<std::vector<std::complex<double>>>();
        for(int i = 0; i < n; ++i) {
            auto row = std::vector<std::complex<double>>();
            for(int j = 0; j < n; ++j) {
                row.push_back(std::pow(get_power_of_unit_root(2 * i + 1), j));
            }
            matrix.push_back(row);
        }
        return matrix;
    }

    auto sigma(Polynomial poly) const {
        auto n = m / 2;
        std::vector<std::complex<double>> vec;
        for(int i = 0; i < n; ++i) {
            vec.push_back(poly.get_value(get_power_of_unit_root(2 * i + 1)));
        }
        return vec;
    }

    auto sigma_inverse(std::vector<std::complex<double>> vec) const {
        auto n = m / 2;
        auto matrix = get_vandermonde();
        Eigen::MatrixXcd eigen_matrix(n, n), eigen_vec(n, 1);
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                eigen_matrix(i, j) = matrix.at(i).at(j);
            }
        }
        for(int i = 0; i < n; ++i) {
            eigen_vec(i) = vec.at(i);
        }
        auto eigen_res = eigen_matrix.inverse() * eigen_vec;
        Polynomial poly;
        for(int i = 0; i < n; ++i) {
            poly.add_term(i, eigen_res(i));
        }
        return poly;
    }

    auto pi(std::vector<std::complex<double>> vec) const {
        auto n = m / 4;
        std::vector<std::complex<double>> new_vec;
        for(int i = 0; i < n; ++i) {
            new_vec.push_back(vec.at(i));
        }
        return new_vec;
    }

    auto pi_inverse(std::vector<std::complex<double>> vec) const {
        auto n = m / 4;
        std::vector<std::complex<double>> new_vec;
        for(int i = 0; i < n; ++i) {
            new_vec.push_back(vec.at(i));
        }
        for(int i = n - 1; i >= 0; --i) {
            new_vec.push_back(conj(vec.at(i)));
        }
        return new_vec;
    }

    auto get_sigma_R_basis() const {
        auto n = m / 2;
        auto matrix = get_vandermonde();
        std::vector<std::vector<std::complex<double>>> basis;
        for(int i = 0; i < n; ++i) {
            std::vector<std::complex<double>> vec(n);
            for(int j = 0; j < n; ++j) {
                vec.at(j) = matrix.at(j).at(i);
            }
            basis.push_back(vec);
        }
        return basis;
    }

    auto vdot(std::vector<std::complex<double>> a, std::vector<std::complex<double>> b) {
        assert(a.size() == b.size());
        std::complex<double> complex = 0;
        for(int i = 0; i < a.size(); ++i) {
            complex += conj(a.at(i)) * b.at(i);
        }
        return complex.real();
    }

    auto get_basis_coordinates(std::vector<std::complex<double>> vec) {
        std::vector<double> new_vec;
        auto basis = get_sigma_R_basis();
        for(auto i : basis) {
            new_vec.push_back(vdot(vec, i) / vdot(i, i));
        }
        return new_vec;
    }

    auto coordinate_wise_random_rounding(std::vector<double> vec) {
        std::vector<int> new_vec;
        std::random_device rd;
        auto gen = std::default_random_engine(rd());
        std::uniform_int_distribution<int> dist(1,100);
        for(auto i : vec) {
            if(dist(gen) % 2) {
                new_vec.push_back(std::floor(i));
            } else {
                new_vec.push_back(std::ceil(i));
            }
        }
        return new_vec;
    }

    auto sigma_R_discrete(std::vector<std::complex<double>> z) {
        auto coordinates = get_basis_coordinates(z);
        auto rounded_coordinates = coordinate_wise_random_rounding(coordinates);
        auto matrix = get_vandermonde();
        std::vector<std::complex<double>> vec;
        for(auto & i : matrix) {
            std::complex<double> complex = 0;
            for(int j = 0; j < rounded_coordinates.size(); j++) {
                complex += std::complex<double>{i.at(j).real() * rounded_coordinates.at(j), i.at(j).imag() * rounded_coordinates.at(j)};
            }
            vec.emplace_back(complex);
        }
        return vec;
    }

    auto encode(std::vector<std::complex<double>> z) {
        auto pi_z = pi_inverse(std::move(z));
        std::vector<std::complex<double>> scaled_pi_z;
        for(auto i : pi_z) {
            scaled_pi_z.emplace_back(i.real() * scale, i.imag() * scale);
        }
        auto rounded_scaled_pi_z = sigma_R_discrete(scaled_pi_z);
        auto poly = sigma_inverse(rounded_scaled_pi_z);
        Polynomial new_poly;
        for(auto term : poly.get_terms()) {
            new_poly.add_term(term.first, int(term.second.real()));
        }
        return new_poly;
    }

    auto decode(Polynomial poly) {
        Polynomial new_poly;
        for(auto term : poly.get_terms()) {
            new_poly.add_term(term.first, {term.second.real() / scale, term.second.imag() / scale});
        }
        auto vec = sigma(new_poly);
        return pi(vec);
    }
};
#endif //CKKS_DEMO_ENCODER_H
