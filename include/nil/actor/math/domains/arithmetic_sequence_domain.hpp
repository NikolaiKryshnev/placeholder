//---------------------------------------------------------------------------//
// Copyright (c) 2020-2021 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020-2021 Nikita Kaskov <nbering@nil.foundation>
// Copyright (c) 2022 Aleksei Moskvin <alalmoskvin@nil.foundation>
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//---------------------------------------------------------------------------//

#ifndef ACTOR_MATH_ARITHMETIC_SEQUENCE_DOMAIN_HPP
#define ACTOR_MATH_ARITHMETIC_SEQUENCE_DOMAIN_HPP

#include <vector>

#include <nil/crypto3/math/domains/evaluation_domain.hpp>

#include <nil/actor/math/polynomial/basis_change.hpp>

namespace nil {
    namespace actor {
        namespace math {

            using namespace nil::crypto3::algebra;

            template<typename FieldType>
            class arithmetic_sequence_domain : public nil::crypto3::math::evaluation_domain<FieldType> {
                typedef typename FieldType::value_type value_type;

            public:
                typedef FieldType field_type;

                bool precomputation_sentinel;
                std::vector<std::vector<std::vector<value_type>>> subproduct_tree;
                std::vector<value_type> arithmetic_sequence;
                value_type arithmetic_generator;

                future<> do_precomputation() {
                    std::cout << "do_precomputation" << std::endl;
                    compute_subproduct_tree<FieldType>(this->subproduct_tree, log2(this->m));

                    arithmetic_generator = value_type(fields::arithmetic_params<FieldType>::arithmetic_generator);

                    arithmetic_sequence = std::vector<value_type>(this->m);

                    detail::block_execution(this->m, smp::count, [this](std::size_t begin, std::size_t end) {
                        for (std::size_t i = begin; i < end; i++) {
                            arithmetic_sequence[i] = arithmetic_generator * value_type(i);
                        }
                    }).get();

                    precomputation_sentinel = true;

                    return nil::actor::make_ready_future<>();
                }

                arithmetic_sequence_domain(const std::size_t m) : crypto3::math::evaluation_domain<FieldType>(m) {
                    BOOST_ASSERT_MSG(m > 1, "Arithmetic(): expected m > 1");
                    BOOST_ASSERT_MSG(
                        value_type(fields::arithmetic_params<FieldType>::arithmetic_generator).is_zero(),
                        "Arithmetic(): expected arithmetic_params<FieldType>::arithmetic_generator.is_zero() "
                        "!= true");

                    precomputation_sentinel = false;
                }

                future<> fft(std::vector<value_type> &a) {
                    if (a.size() != this->m) {
                        BOOST_ASSERT_MSG(a.size() >= this->m, "Arithmetic: expected a.size() == this->m");

                        a.resize(this->m, value_type(0));
                    }

                    if (!this->precomputation_sentinel) {
                        do_precomputation();
                    }

                    /* Monomial to Newton */
                    monomial_to_newton_basis<FieldType>(a, subproduct_tree, this->m);

                    /* Newton to Evaluation */
                    std::vector<value_type> S(this->m); /* i! * arithmetic_generator */
                    S[0] = value_type::one();

                    value_type factorial = value_type::one();
                    for (std::size_t i = 1; i < this->m; i++) {
                        factorial *= value_type(i);
                        S[i] = (factorial * arithmetic_generator).inversed();
                    }

                    multiplication(a, a, S).get();
                    a.resize(this->m);

                    detail::block_execution(this->m, smp::count, [&a, &S](std::size_t begin, std::size_t end) {
                        for (std::size_t i = begin; i < end; i++) {
                            a[i] *= S[i].inversed();
                        }
                    }).get();

                    return nil::actor::make_ready_future<>();
                }

                void inverse_fft(std::vector<value_type> &a) {
                    if (a.size() != this->m) {
                        BOOST_ASSERT_MSG(a.size() >= this->m, "Arithmetic: expected a.size() == this->m");
                        a.resize(this->m, value_type(0));
                    }

                    if (!this->precomputation_sentinel)
                        do_precomputation();

                    /* Interpolation to Newton */
                    std::vector<value_type> S(this->m); /* i! * arithmetic_generator */
                    S[0] = value_type::one();

                    std::vector<value_type> W(this->m);
                    W[0] = a[0] * S[0];

                    value_type factorial = value_type::one();
                    for (std::size_t i = 1; i < this->m; i++) {
                        factorial *= value_type(i);
                        S[i] = (factorial * arithmetic_generator).inversed();
                        W[i] = a[i] * S[i];
                        if (i % 2 == 1)
                            S[i] = -S[i];
                    }

                    multiplication(a, W, S);
                    a.resize(this->m);

                    /* Newton to Monomial */
                    newton_to_monomial_basis<FieldType>(a, subproduct_tree, this->m);
                }

                future<std::vector<value_type>> evaluate_all_lagrange_polynomials(const value_type &t) {
                    /* Compute Lagrange polynomial of size m, with m+1 points (x_0, y_0), ... ,(x_m, y_m) */
                    /* Evaluate for x = t */
                    /* Return coeffs for each l_j(x) = (l / l_i[j]) * w[j] */

                    if (!precomputation_sentinel)
                        do_precomputation();

                    /**
                     * If t equals one of the arithmetic progression values,
                     * then output 1 at the right place, and 0 elsewhere.
                     */
                    for (std::size_t i = 0; i < this->m; ++i) {
                        if (arithmetic_sequence[i] == t)    // i.e., t equals this->arithmetic_sequence[i]
                        {
                            std::vector<value_type> res(this->m, value_type::zero());
                            res[i] = value_type::one();
                            return res;
                        }
                    }

                    /**
                     * Otherwise, if t does not equal any of the arithmetic progression values,
                     * then compute each Lagrange coefficient.
                     */
                    std::vector<value_type> l(this->m);
                    l[0] = t - this->arithmetic_sequence[0];

                    value_type l_vanish = l[0];
                    value_type g_vanish = value_type::one();

                    detail::block_execution(
                        this->m, smp::count, [t, &l, &l_vanish, &g_vanish, this](std::size_t begin, std::size_t end) {
                            for (std::size_t i = begin; i < end; i++) {
                                l[i] = t - this->arithmetic_sequence[i];
                                l_vanish *= l[i];
                                g_vanish *= -this->arithmetic_sequence[i];
                            }
                        }).get();

                    std::vector<value_type> w(this->m);
                    w[0] = g_vanish.inversed() * (this->arithmetic_generator.pow(this->m - 1));

                    l[0] = l_vanish * l[0].inversed() * w[0];
                    for (std::size_t i = 1; i < this->m; i++) {
                        value_type num = this->arithmetic_sequence[i - 1] - this->arithmetic_sequence[this->m - 1];
                        w[i] = w[i - 1] * num * this->arithmetic_sequence[i].inversed();
                        l[i] = l_vanish * l[i].inversed() * w[i];
                    }

                    //                    return l;
                    return nil::actor::make_ready_future<std::vector<value_type>>(l);
                }

                value_type get_domain_element(const std::size_t idx) {
                    if (!this->precomputation_sentinel)
                        do_precomputation();

                    return this->arithmetic_sequence[idx];
                }

                value_type compute_vanishing_polynomial(const value_type &t) {
                    if (!this->precomputation_sentinel)
                        do_precomputation();

                    /* Notes: Z = prod_{i = 0 to m} (t - a[i]) */
                    value_type Z = value_type::one();
                    for (std::size_t i = 0; i < this->m; i++) {
                        Z *= (t - this->arithmetic_sequence[i]);
                    }
                    return Z;
                }

                future<> add_poly_z(const value_type &coeff, std::vector<value_type> &H) {
                    BOOST_ASSERT_MSG(H.size() == this->m + 1, "Arithmetic: expected H.size() == this->m+1");

                    if (!this->precomputation_sentinel)
                        do_precomputation();

                    std::vector<value_type> x(2, value_type::zero());
                    x[0] = -this->arithmetic_sequence[0];
                    x[1] = value_type::one();

                    std::vector<value_type> t(2, value_type::zero());

                    for (std::size_t i = 1; i < this->m + 1; i++) {
                        t[0] = -this->arithmetic_sequence[i];
                        t[1] = value_type::one();

                        multiplication(x, x, t).get();
                    }

                    detail::block_execution(
                        this->m, smp::count, [&H, &x, coeff, this](std::size_t begin, std::size_t end) {
                            for (std::size_t i = begin; i < end; i++) {
                                H[i] += (x[i] * coeff);
                            }
                        }).get();

                    return nil::actor::make_ready_future<>();
                }
                future<> divide_by_z_on_coset(std::vector<value_type> &P) {
                    const value_type coset = this->arithmetic_generator; /* coset in arithmetic sequence? */
                    const value_type Z_inverse_at_coset = this->compute_vanishing_polynomial(coset).inversed();

                    detail::block_execution(
                        this->m, smp::count, [&P, Z_inverse_at_coset, this](std::size_t begin, std::size_t end) {
                            for (std::size_t i = begin; i < end; i++) {
                                P[i] *= Z_inverse_at_coset;
                            }
                        }).get();

                    return nil::actor::make_ready_future<>();
                }
            };
        }    // namespace math
    }        // namespace actor
}    // namespace nil

#endif    // ALGEBRA_FFT_ARITHMETIC_SEQUENCE_DOMAIN_HPP
