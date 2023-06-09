//
// Created by yufoo1 on 2023/5/23.
//

#ifndef CKKS_DEMO_POLYNOMIAL_H
#define CKKS_DEMO_POLYNOMIAL_H
#include "map"
#include "cmath"
#include "complex"
#include "iostream"

class Polynomial {
private:
    std::map<int, std::complex<double>> terms;
public:
    explicit Polynomial() {
        terms.clear();
    }

    explicit Polynomial(std::map<int, std::complex<double>> terms) {
        this->terms = std::move(terms);
    }

    auto add_term(int power, std::complex<double> coefficient) {
        if(terms.count(power)) {
            auto term = terms.at(power) + coefficient;
            terms.erase(power);
            terms.insert({power, term});
        } else {
            terms.insert({power, coefficient});
        }
    }

    auto get_terms() {
        return terms;
    }

    auto add_poly(Polynomial poly) {
        for(auto i : poly.get_terms()) {
            add_term(i.first, i.second);
        }
    }

    auto mult_poly(Polynomial poly) {
        Polynomial new_poly;
        for(auto i : this->get_terms()) {
            for(auto j : poly.get_terms()) {
                new_poly.add_term(i.first + j.first, i.second * j.second);
            }
        }
        this->terms = new_poly.terms;
    }

    auto get_max_power() {
        int max = -1;
        for(auto i : terms) {
            if(max < i.first) {
                max = i.first;
            }
        }
        return max;
    }

    auto mod_poly_power(int power) {
        /**
         * mod x^power + 1
         */
        while(power <= get_max_power()) {
            auto max = get_max_power();
            auto term = terms.at(get_max_power());
            terms.erase(get_max_power());
            std::complex<double> new_term = {-term.real(), -term.imag()};
            add_term(max - power, new_term);
        }
    }

    auto mod_poly_coefficient(int q) {
        std::map<int, std::complex<double>> new_terms;
        for(auto term : terms) {
            int real = int(term.second.real());
            while(real < 0) {
                real += q;
            }
            new_terms.insert({term.first, real % q});
        }
        terms = new_terms;
    }

    auto get_value(std::complex<double> root) {
        std::complex<double> res = 0;
        for(auto term : terms) {
            res += term.second * std::pow(root, term.first);
        }
        return res;
    }

    friend std::ostream &operator<<(std::ostream &output, const Polynomial &poly) {
        output << "poly: " << std::endl;
        for(auto term : poly.terms) {
            output << "    power: " << term.first << ", coefficient: " << term.second << std::endl;
        }
        return output;
    }
};
#endif //CKKS_DEMO_POLYNOMIAL_H
