//
// Created by yufoo1 on 2023/5/23.
//

#ifndef CKKS_DEMO_ENCODER_H
#define CKKS_DEMO_ENCODER_H
#include "complex"
#include "vector"
#include "Polynomial.h"
#include "iostream"
#include "Eigen/Dense"

class Encoder {
private:
    int m;

public:
    explicit Encoder(int m) {
        this->m = m;
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
};
#endif //CKKS_DEMO_ENCODER_H
