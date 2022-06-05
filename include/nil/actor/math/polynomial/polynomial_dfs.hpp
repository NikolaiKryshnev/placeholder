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

#ifndef ACTOR_MATH_POLYNOMIAL_POLYNOM_DFT_HPP
#define ACTOR_MATH_POLYNOMIAL_POLYNOM_DFT_HPP

#include <algorithm>
#include <vector>

#include <nil/actor/math/polynomial/basic_operations.hpp>

namespace nil {
    namespace actor {
        namespace math {
            // Optimal val.size must be power of two, if it's not true we have points that we will never use
            template<typename FieldValueType, typename Allocator = std::allocator<FieldValueType>>
            class polynomial_dfs {
                typedef std::vector<FieldValueType, Allocator> container_type;

                container_type val;
                size_t _d;

            public:
                typedef typename container_type::value_type value_type;
                typedef typename container_type::allocator_type allocator_type;
                typedef typename container_type::reference reference;
                typedef typename container_type::const_reference const_reference;
                typedef typename container_type::size_type size_type;
                typedef typename container_type::difference_type difference_type;
                typedef typename container_type::pointer pointer;
                typedef typename container_type::const_pointer const_pointer;
                typedef typename container_type::iterator iterator;
                typedef typename container_type::const_iterator const_iterator;
                typedef typename container_type::reverse_iterator reverse_iterator;
                typedef typename container_type::const_reverse_iterator const_reverse_iterator;

                polynomial_dfs() : val({0}) {
                    _d = 0;
                }

                explicit polynomial_dfs(size_t d, size_type n) : val(n), _d(d) {
                    BOOST_ASSERT_MSG(n == crypto3::math::detail::power_of_two(n), "DFS optimal polynom size must be power of two");
                }

                explicit polynomial_dfs(size_t d, size_type n, const allocator_type& a) : val(n, a), _d(d) {
                    BOOST_ASSERT_MSG(n == crypto3::math::detail::power_of_two(n), "DFS optimal polynom size must be power of two");
                }

                polynomial_dfs(size_t d, size_type n, const value_type& x) : val(n, x), _d(d) {
                    BOOST_ASSERT_MSG(n == crypto3::math::detail::power_of_two(n), "DFS optimal polynom size must be power of two");
                }

                polynomial_dfs(size_t d, size_type n, const value_type& x, const allocator_type& a) :
                    val(n, x, a), _d(d) {
                    BOOST_ASSERT_MSG(n == crypto3::math::detail::power_of_two(n), "DFS optimal polynom size must be power of two");
                }

                template<typename InputIterator>
                polynomial_dfs(size_t d, InputIterator first, InputIterator last) : val(first, last), _d(d) {
                    BOOST_ASSERT_MSG(std::distance(first, last) == crypto3::math::detail::power_of_two(std::distance(first, last)),
                                     "DFS optimal polynom size must be power of two");
                }

                template<typename InputIterator>
                polynomial_dfs(size_t d, InputIterator first, InputIterator last, const allocator_type& a) :
                    val(first, last, a), _d(d) {
                    BOOST_ASSERT_MSG(std::distance(first, last) == crypto3::math::detail::power_of_two(std::distance(first, last)),
                                     "DFS optimal polynom size must be power of two");
                }

                ~polynomial_dfs() = default;

                polynomial_dfs(const polynomial_dfs& x) : val(x.val), _d(x._d) {
                }

                polynomial_dfs(const polynomial_dfs& x, const allocator_type& a) : val(x.val, a), _d(x._d) {
                }

                polynomial_dfs(size_t d, std::initializer_list<value_type> il) : val(il), _d(d) {
                }

                polynomial_dfs(size_t d, std::initializer_list<value_type> il, const allocator_type& a) :
                    val(il, a), _d(d) {
                    BOOST_ASSERT_MSG(val.size() == crypto3::math::detail::power_of_two(val.size()),
                                     "DFS optimal polynom size must be power of two");
                }
                // TODO: add constructor with omega

                polynomial_dfs(polynomial_dfs&& x)
                    BOOST_NOEXCEPT(std::is_nothrow_move_constructible<allocator_type>::value) :
                    val(x.val),
                    _d(x._d) {
                }

                polynomial_dfs(polynomial_dfs&& x, const allocator_type& a) : val(x.val, a), _d(x._d) {
                }

                polynomial_dfs(size_t d, const container_type& c) : val(c), _d(d) {
                    BOOST_ASSERT_MSG(val.size() == crypto3::math::detail::power_of_two(val.size()),
                                     "DFS optimal polynom size must be power of two");
                }

                polynomial_dfs(size_t d, container_type&& c) : val(c), _d(d) {
                    BOOST_ASSERT_MSG(val.size() == crypto3::math::detail::power_of_two(val.size()),
                                     "DFS optimal polynom size must be power of two");
                }

                polynomial_dfs& operator=(const polynomial_dfs& x) {
                    val = x.val;
                    _d = x._d;
                    return *this;
                }

                polynomial_dfs& operator=(polynomial_dfs&& x) {
                    val = x.val;
                    _d = x._d;
                    return *this;
                }

                //                polynomial_dfs& operator=(const container_type& x) {
                //                    val = x;
                //                    return *this;
                //                }
                //
                //                polynomial_dfs& operator=(container_type&& x) {
                //                    val = x;
                //                    return *this;
                //                }

                //                polynomial_dfs& operator=(std::initializer_list<value_type> il) {
                //                    val.assign(il.begin(), il.end());
                //                    return *this;
                //                }

                bool operator==(const polynomial_dfs& rhs) const {
                    return val == rhs.val && _d == rhs.d;
                }
                bool operator!=(const polynomial_dfs& rhs) const {
                    return !(rhs == *this && _d == rhs.d);
                }

                //                template<typename InputIterator>
                //                typename std::iterator_traits<InputIterator>::reference assign(InputIterator first,
                //                                                                               InputIterator last) {
                //                    return val.assign(first, last);
                //                }
                //
                //                void assign(size_type n, const_reference u) {
                //                    return val.assign(n, u);
                //                }
                //
                //                void assign(std::initializer_list<value_type> il) {
                //                    assign(il.begin(), il.end());
                //                }

                allocator_type get_allocator() const BOOST_NOEXCEPT {
                    return this->val.__alloc();
                }

                iterator begin() BOOST_NOEXCEPT {
                    return val.begin();
                }

                const_iterator begin() const BOOST_NOEXCEPT {
                    return val.begin();
                }
                iterator end() BOOST_NOEXCEPT {
                    return val.end();
                }
                const_iterator end() const BOOST_NOEXCEPT {
                    return val.end();
                }

                reverse_iterator rbegin() BOOST_NOEXCEPT {
                    return val.rbegin();
                }

                const_reverse_iterator rbegin() const BOOST_NOEXCEPT {
                    return val.rbegin();
                }

                reverse_iterator rend() BOOST_NOEXCEPT {
                    return reverse_iterator(begin());
                }

                const_reverse_iterator rend() const BOOST_NOEXCEPT {
                    return const_reverse_iterator(begin());
                }

                const_iterator cbegin() const BOOST_NOEXCEPT {
                    return begin();
                }

                const_iterator cend() const BOOST_NOEXCEPT {
                    return end();
                }

                const_reverse_iterator crbegin() const BOOST_NOEXCEPT {
                    return rbegin();
                }

                const_reverse_iterator crend() const BOOST_NOEXCEPT {
                    return rend();
                }

                size_type size() const BOOST_NOEXCEPT {
                    return val.size();
                }

                size_type degree() const BOOST_NOEXCEPT {
                    return _d;
                }

                size_type max_degree() const BOOST_NOEXCEPT {
                    return this->size();
                }

                size_type capacity() const BOOST_NOEXCEPT {
                    return val.capacity();
                }
                bool empty() const BOOST_NOEXCEPT {
                    return val.empty();
                }
                size_type max_size() const BOOST_NOEXCEPT {
                    return val.max_size();
                }
                void reserve(size_type _n) {
                    return val.reserve(_n);
                }
                void shrink_to_fit() BOOST_NOEXCEPT {
                    return val.shrink_to_fit();
                }

                reference operator[](size_type _n) BOOST_NOEXCEPT {
                    return val[_n];
                }
                const_reference operator[](size_type _n) const BOOST_NOEXCEPT {
                    return val[_n];
                }
                reference at(size_type _n) {
                    return val.at(_n);
                }
                const_reference at(size_type _n) const {
                    return val.at(_n);
                }

                reference front() BOOST_NOEXCEPT {
                    return val.front();
                }
                const_reference front() const BOOST_NOEXCEPT {
                    return val.front();
                }
                reference back() BOOST_NOEXCEPT {
                    return val.back();
                }
                const_reference back() const BOOST_NOEXCEPT {
                    return val.back();
                }

                value_type* data() BOOST_NOEXCEPT {
                    return val.data();
                }

                const value_type* data() const BOOST_NOEXCEPT {
                    return val.data();
                }

                void push_back(const_reference _x) {
                    val.push_back(_x);
                }

                void push_back(value_type&& _x) {
                    val.push_back(_x);
                }

                template<class... Args>
                reference emplace_back(Args&&... _args) {
                    return val.template emplace_back(_args...);
                }

                void pop_back() {
                    val.pop_back();
                }

                iterator insert(const_iterator _position, const_reference _x) {
                    return val.insert(_position, _x);
                }

                iterator insert(const_iterator _position, value_type&& _x) {
                    return val.insert(_position, _x);
                }
                template<class... Args>
                iterator emplace(const_iterator _position, Args&&... _args) {
                    return val.template emplace(_position, _args...);
                }

                iterator insert(const_iterator _position, size_type _n, const_reference _x) {
                    return val.insert(_position, _n, _x);
                }

                template<class InputIterator>
                iterator insert(const_iterator _position, InputIterator _first, InputIterator _last) {
                    return val.insert(_position, _first, _last);
                }

                iterator insert(const_iterator _position, std::initializer_list<value_type> _il) {
                    return insert(_position, _il.begin(), _il.end());
                }

                iterator erase(const_iterator _position) {
                    return val.erase(_position);
                }

                iterator erase(const_iterator _first, const_iterator _last) {
                    return val.erase(_first, _last);
                }

                void clear() BOOST_NOEXCEPT {
                    val.clear();
                }

                future<> resize(size_type _sz) {
                    BOOST_ASSERT_MSG(_sz >= _d, "Can't restore polynomial in the future");
                    typedef typename value_type::field_type FieldType;

                    value_type omega = crypto3::math::unity_root<FieldType>(this->size());

                    detail::basic_radix2_fft<FieldType>(val, omega.inversed()).get();

                    const value_type sconst = value_type(this->size()).inversed();

                    detail::block_execution(
                        this->size(), smp::count, [this, sconst](std::size_t begin, std::size_t end) {
                            for (std::size_t i = begin; i < end; i++) {
                                val[i] *= sconst;
                            }
                        }).get();

                    value_type omega_new = crypto3::math::unity_root<FieldType>(_sz);
                    val.resize(_sz);

                    detail::basic_radix2_fft<FieldType>(val, omega_new).get();

                    return make_ready_future<>();
                }

                //                void resize(size_type _sz, const_reference _x) {
                //                    BOOST_ASSERT_MSG(_sz >= _d, "Can't restore polynomial in the future");
                //                    return val.resize(_sz, _x);
                //                }

                void swap(polynomial_dfs& other) {
                    val.swap(other.val);
                    std::swap(_d, other._d);
                }

                FieldValueType evaluate(const FieldValueType& value) const {

                    std::vector<FieldValueType> tmp = this->coefficients();
                    FieldValueType result = 0;
                    auto end = tmp.end();
                    while (end != tmp.begin()) {
                        result = result * value + *--end;
                    }
                    return result;
                }

                /**
                 * Returns true if polynomial is a zero polynomial.
                 */
                bool is_zero() const {
                    return _d == 0;
                }

                /**
                 * Compute the reverse polynomial up to vector size n (degree n-1).
                 * Below we make use of the reversal endomorphism definition from
                 * [Bostan, Lecerf, & Schost, 2003. Tellegen's Principle in Practice, on page 38].
                 */
                void reverse(std::size_t n) {
                    std::reverse(this->begin(), this->end());
                    this->resize(n);
                }

                /**
                 * Computes the standard polynomial addition, polynomial A + polynomial B, and stores result in
                 * polynomial C.
                 */
                polynomial_dfs operator+(const polynomial_dfs& other) const {
                    polynomial_dfs result(std::max(this->_d, other._d), this->begin(), this->end());
                    if (other.size() > this->size()) {
                        result.resize(other.size()).get();
                    }
                    if (this->size() > other.size()) {
                        polynomial_dfs tmp(other);
                        tmp.resize(this->size()).get();
                        detail::block_execution(
                            result.size(), smp::count, [&result, &tmp](std::size_t begin, std::size_t end) {
                                for (std::size_t i = begin; i < end; i++) {
                                    result[i] += tmp[i];
                                }
                            }).get();
                        return result;
                    }
                    detail::block_execution(
                        result.size(), smp::count, [&result, &other](std::size_t begin, std::size_t end) {
                            for (std::size_t i = begin; i < end; i++) {
                                result[i] += other[i];
                            }
                        }).get();

                    return result;
                }

                polynomial_dfs operator-() const {
                    polynomial_dfs result(this->_d, this->begin(), this->end());
                    detail::block_execution(result.size(), smp::count, [&result](std::size_t begin, std::size_t end) {
                        for (std::size_t i = begin; i < end; i++) {
                            result[i] = -result[i];
                        }
                    }).get();
//                    std::transform(this->begin(), this->end(), result.begin(), std::negate<FieldValueType>());
                    return result;
                }

                /**
                 * Computes the standard polynomial subtraction, polynomial A - polynomial B, and stores result in
                 * polynomial C.
                 */
                polynomial_dfs operator-(const polynomial_dfs& other) const {
                    polynomial_dfs result(std::max(_d, other._d), this->begin(), this->end());
                    if (other.size() > this->size()) {
                        result.resize(other.size()).get();
                    }
                    if (this->size() > other.size()) {
                        polynomial_dfs tmp(other);
                        tmp.resize(this->size()).get();
                        detail::block_execution(
                            result.size(), smp::count, [&result, &tmp](std::size_t begin, std::size_t end) {
                                for (std::size_t i = begin; i < end; i++) {
                                    result[i] -= tmp[i];
                                }
                            }).get();
                        return result;
                    }
                    detail::block_execution(
                        result.size(), smp::count, [&result, &other](std::size_t begin, std::size_t end) {
                            for (std::size_t i = begin; i < end; i++) {
                                result[i] -= other[i];
                            }
                        }).get();
                    return result;
                }

                /**
                 * Perform the multiplication of two polynomials, polynomial A * polynomial B, and stores result in
                 * polynomial C.
                 */
                polynomial_dfs operator*(const polynomial_dfs& other) const {
                    polynomial_dfs result(this->_d + other._d, this->begin(), this->end());
                    size_t polynomial_s =
                        crypto3::math::detail::power_of_two(std::max({this->size(), other.size(), this->_d + other._d + 1}));
                    if (result.size() < polynomial_s) {
                        result.resize(polynomial_s).get();
                    }
                    if (other.size() < polynomial_s) {
                        polynomial_dfs tmp(other);
                        tmp.resize(polynomial_s).get();
                        detail::block_execution(
                            result.size(), smp::count, [&result, &tmp](std::size_t begin, std::size_t end) {
                                for (std::size_t i = begin; i < end; i++) {
                                    result[i] = result[i] * tmp[i];
                                }
                            }).get();
                        return result;
                    }
                    detail::block_execution(
                        result.size(), smp::count, [&result, &other](std::size_t begin, std::size_t end) {
                            for (std::size_t i = begin; i < end; i++) {
                                result[i] = result[i] * other[i];
                            }
                        }).get();
                    return result;
                }

                /**
                 * Perform the standard Euclidean Division algorithm.
                 * Input: Polynomial A, Polynomial B, where A / B
                 * Output: Polynomial Q, such that A = (Q * B) + R.
                 */
                polynomial_dfs operator/(const polynomial_dfs& other) const {
                    std::vector<FieldValueType> x = this->coefficients();
                    std::vector<FieldValueType> y = other.coefficients();

                    std::size_t d = y.size() - 1;           /* Degree of B */
                    FieldValueType c = y.back().inversed(); /* Inverse of Leading Coefficient of B */

                    std::vector<FieldValueType> r(x);
                    std::vector<FieldValueType> q(r.size(), FieldValueType::zero());

                    std::size_t r_deg = r.size() - 1;
                    std::size_t shift;

                    while (r_deg >= d && (r.size() != 0)) {
                        if (r_deg >= d) {
                            shift = r_deg - d;
                        } else {
                            shift = 0;
                        }

                        FieldValueType lead_coeff = r.back() * c;

                        q[shift] += lead_coeff;

                        if (y.size() + shift + 1 > r.size()) {
                            r.resize(y.size() + shift + 1);
                        }
                        auto glambda = [=](const FieldValueType& x, const FieldValueType& y) {
                            return y - (x * lead_coeff);
                        };
                        std::transform(y.begin(), y.end(), r.begin() + shift, r.begin() + shift, glambda);

                        size_t new_s = r.size();
                        while (new_s > 0 && r[new_s - 1] == FieldValueType(0)) {
                            --new_s;
                        }
                        r.resize(new_s);

                        r_deg = r.size() - 1;
                    }
                    size_t new_s = q.size();
                    while (new_s > 0 && q[new_s - 1] == FieldValueType(0)) {
                        --new_s;
                    }
                    q.resize(new_s);

                    typedef typename value_type::field_type FieldType;
                    size_t n = this->size();
                    value_type omega = crypto3::math::unity_root<FieldType>(n);
                    q.resize(n);
                    detail::basic_radix2_fft<FieldType>(q, omega).get();
                    return polynomial_dfs(new_s - 1, q);
                }

                /**
                 * Perform the standard Euclidean Division algorithm.
                 * Input: Polynomial A, Polynomial B, where A / B
                 * Output: Polynomial R, such that A = (Q * B) + R.
                 */
                polynomial_dfs operator%(const polynomial_dfs& other) const {
                    std::vector<FieldValueType> x = this->coefficients();
                    std::vector<FieldValueType> y = other.coefficients();

                    std::size_t d = y.size() - 1;           /* Degree of B */
                    FieldValueType c = y.back().inversed(); /* Inverse of Leading Coefficient of B */

                    std::vector<FieldValueType> r(x);

                    std::size_t r_deg = r.size() - 1;
                    std::size_t shift;

                    while (r_deg >= d && (r.size() != 0)) {
                        if (r_deg >= d) {
                            shift = r_deg - d;
                        } else {
                            shift = 0;
                        }

                        FieldValueType lead_coeff = r.back() * c;

                        if (y.size() + shift + 1 > r.size()) {
                            r.resize(y.size() + shift + 1);
                        }
                        auto glambda = [=](const FieldValueType& x, const FieldValueType& y) {
                            return y - (x * lead_coeff);
                        };
                        std::transform(y.begin(), y.end(), r.begin() + shift, r.begin() + shift, glambda);

                        size_t new_s = r.size();
                        while (new_s > 0 && r[new_s - 1] == FieldValueType(0)) {
                            --new_s;
                        }
                        r.resize(new_s);

                        r_deg = r.size() - 1;
                    }

                    typedef typename value_type::field_type FieldType;
                    size_t n = this->size();
                    value_type omega = crypto3::math::unity_root<FieldType>(n);
                    r.resize(n);
                    detail::basic_radix2_fft<FieldType>(r, omega).get();
                    return polynomial_dfs(r_deg, r);
                }

                void from_coefficients(const container_type &tmp) {
                    typedef typename value_type::field_type FieldType;
                    size_t n = crypto3::math::detail::power_of_two(tmp.size());
                    value_type omega = crypto3::math::unity_root<FieldType>(n);
                    _d = tmp.size() - 1;
                    val.assign(tmp.begin(), tmp.end());
                    val.resize(n, FieldValueType::zero());
                    detail::basic_radix2_fft<FieldType>(val, omega).get();
                }

                std::vector<FieldValueType> coefficients() const {
                    typedef typename value_type::field_type FieldType;

                    value_type omega = crypto3::math::unity_root<FieldType>(this->size());
                    std::vector<FieldValueType> tmp(this->begin(), this->end());

                    detail::basic_radix2_fft<FieldType>(tmp, omega.inversed()).get();

                    const value_type sconst = value_type(this->size()).inversed();
                    detail::block_execution(
                        this->size(), smp::count, [&tmp, sconst](std::size_t begin, std::size_t end) {
                            for (std::size_t i = begin; i < end; i++) {
                                tmp[i] *= sconst;
                            }
                        }).get();
                    size_t r_size = tmp.size();
                    while (r_size > 0 && tmp[r_size - 1] == FieldValueType(0)) {
                        --r_size;
                    }
                    tmp.resize(r_size);
                    return tmp;
                }
            };

            template<typename FieldValueType, typename Allocator = std::allocator<FieldValueType>,
                     typename = typename std::enable_if<crypto3::math::detail::is_field_element<FieldValueType>::value>::type>
            polynomial_dfs<FieldValueType, Allocator> operator+(const polynomial_dfs<FieldValueType, Allocator>& A,
                                                            const FieldValueType& B) {

                return A + polynomial_dfs<FieldValueType>(0, A.size(), B);
            }

            template<typename FieldValueType, typename Allocator = std::allocator<FieldValueType>,
                     typename = typename std::enable_if<crypto3::math::detail::is_field_element<FieldValueType>::value>::type>
            polynomial_dfs<FieldValueType, Allocator> operator+(const FieldValueType& A,
                                                            const polynomial_dfs<FieldValueType, Allocator>& B) {

                return polynomial_dfs<FieldValueType>(0, B.size(), A) + B;
            }

            template<typename FieldValueType, typename Allocator = std::allocator<FieldValueType>,
                     typename = typename std::enable_if<crypto3::math::detail::is_field_element<FieldValueType>::value>::type>
            polynomial_dfs<FieldValueType, Allocator> operator-(const polynomial_dfs<FieldValueType, Allocator>& A,
                                                            const FieldValueType& B) {

                return A - polynomial_dfs<FieldValueType>(0, A.size(), B);
            }

            template<typename FieldValueType, typename Allocator = std::allocator<FieldValueType>,
                     typename = typename std::enable_if<crypto3::math::detail::is_field_element<FieldValueType>::value>::type>
            polynomial_dfs<FieldValueType, Allocator> operator-(const FieldValueType& A,
                                                            const polynomial_dfs<FieldValueType, Allocator>& B) {

                return polynomial_dfs<FieldValueType>(0, B.size(), A) - B;
            }

            template<typename FieldValueType, typename Allocator = std::allocator<FieldValueType>,
                     typename = typename std::enable_if<crypto3::math::detail::is_field_element<FieldValueType>::value>::type>
            polynomial_dfs<FieldValueType, Allocator> operator*(const polynomial_dfs<FieldValueType, Allocator>& A,
                                                            const FieldValueType& B) {

                return A * polynomial_dfs<FieldValueType>(0, A.size(), B);
            }

            template<typename FieldValueType, typename Allocator = std::allocator<FieldValueType>,
                     typename = typename std::enable_if<crypto3::math::detail::is_field_element<FieldValueType>::value>::type>
            polynomial_dfs<FieldValueType, Allocator> operator*(const FieldValueType& A,
                                                            const polynomial_dfs<FieldValueType, Allocator>& B) {

                return polynomial_dfs<FieldValueType>(0, B.size(), A) * B;
            }

            template<typename FieldValueType, typename Allocator = std::allocator<FieldValueType>,
                     typename = typename std::enable_if<crypto3::math::detail::is_field_element<FieldValueType>::value>::type>
            polynomial_dfs<FieldValueType, Allocator> operator/(const polynomial_dfs<FieldValueType, Allocator>& A,
                                                            const FieldValueType& B) {

                return A / polynomial_dfs<FieldValueType>(0, A.size(), B);
            }

            template<typename FieldValueType, typename Allocator = std::allocator<FieldValueType>,
                     typename = typename std::enable_if<crypto3::math::detail::is_field_element<FieldValueType>::value>::type>
            polynomial_dfs<FieldValueType, Allocator> operator/(const FieldValueType& A,
                                                            const polynomial_dfs<FieldValueType, Allocator>& B) {

                return polynomial_dfs<FieldValueType>(0, B.size(), A) / B;
            }
        }    // namespace math
    }        // namespace actor
}    // namespace nil

#endif    // ACTOR_MATH_POLYNOMIAL_POLYNOM_DFT_HPP
