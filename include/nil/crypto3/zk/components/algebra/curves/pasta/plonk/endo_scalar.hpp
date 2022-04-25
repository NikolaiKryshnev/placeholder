//---------------------------------------------------------------------------//
// Copyright (c) 2021 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2021 Nikita Kaskov <nbering@nil.foundation>
// Copyright (c) 2022 Ilia Shirobokov <i.shirobokov@nil.foundation>
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
// @file Declaration of interfaces for auxiliary components for the SHA256 component.
//---------------------------------------------------------------------------//

#ifndef CRYPTO3_ZK_BLUEPRINT_PLONK_CURVE_ELEMENT_ENDO_SCALAR_COMPONENT_15_WIRES_HPP
#define CRYPTO3_ZK_BLUEPRINT_PLONK_CURVE_ELEMENT_ENDO_SCALAR_COMPONENT_15_WIRES_HPP

#include <nil/marshalling/algorithms/pack.hpp>

#include <nil/crypto3/zk/snark/arithmetization/plonk/constraint_system.hpp>

#include <nil/crypto3/zk/blueprint/plonk.hpp>
#include <nil/crypto3/zk/assignment/plonk.hpp>

namespace nil {
    namespace crypto3 {
        namespace zk {
            namespace components {

                template<typename ArithmetizationType,
                         typename CurveType,
                         std::size_t... WireIndexes>
                class endo_scalar;

                template<typename BlueprintFieldType,
                         typename ArithmetizationParams,
                         typename CurveType,
                         std::size_t W0,
                         std::size_t W1,
                         std::size_t W2,
                         std::size_t W3,
                         std::size_t W4,
                         std::size_t W5,
                         std::size_t W6,
                         std::size_t W7,
                         std::size_t W8,
                         std::size_t W9,
                         std::size_t W10,
                         std::size_t W11,
                         std::size_t W12,
                         std::size_t W13,
                         std::size_t W14>
                class endo_scalar<
                    snark::plonk_constraint_system<BlueprintFieldType, ArithmetizationParams>,
                    CurveType,
                    W0, W1, W2, W3,
                    W4, W5, W6, W7,
                    W8, W9, W10, W11,
                    W12, W13, W14> {

                    typedef snark::plonk_constraint_system<BlueprintFieldType,
                        ArithmetizationParams> ArithmetizationType;

                    using var = snark::plonk_variable<BlueprintFieldType>;

                    constexpr static const std::size_t selector_seed = 0xff06;

                    template<typename ComponentType, typename ArithmetizationType>
                    friend void generate_circuit(blueprint<ArithmetizationType> &bp,
                        blueprint_public_assignment_table<ArithmetizationType> &assignment,
                        const typename ComponentType::params_type params,
                        const std::size_t start_row_index);

                public:
                    constexpr static const std::size_t rows_amount = 8;

                    struct params_type {
                        var scalar;
                        typename BlueprintFieldType::value_type endo_factor;
                        std::size_t num_bits;
                    };

                    struct result_type {
                        var endo_scalar = var(0, 0, false);
                        result_type(const std::size_t &component_start_row) {
                            endo_scalar = var(W6, component_start_row + rows_amount - 1, false, var::column_type::witness);
                        }
                    };

                    struct allocated_data_type {
                        allocated_data_type() {
                            previously_allocated = false;
                        }

                        // TODO access modifiers
                        bool previously_allocated;
                        std::size_t selector_1;
                        std::size_t selector_2;
                    };

                    static std::size_t allocate_rows (blueprint<ArithmetizationType> &in_bp){
                        return in_bp.allocate_rows(rows_amount);
                    }

                    static result_type generate_circuit(
                        blueprint<ArithmetizationType> &bp,
                        blueprint_assignment_table<ArithmetizationType> &assignment,
                        const params_type &params,
                        allocated_data_type &allocated_data,
                        const std::size_t &component_start_row) {

                        generate_gates(bp, assignment, params, allocated_data, component_start_row);
                        generate_copy_constraints(bp, assignment, params, component_start_row);
                        return result_type(component_start_row);
                    }

                    static result_type generate_assignments(
                            blueprint_assignment_table<ArithmetizationType> &assignment,
                                        const params_type &params,
                                        const std::size_t &component_start_row) {
                            
                            std::size_t row = component_start_row;
                            
                            std::size_t crumbs_per_row = 8;
                            std::size_t bits_per_crumb = 2;
                            std::size_t bits_per_row = bits_per_crumb * crumbs_per_row; // we suppose that params.num_bits % bits_per_row = 0

                            std::vector<typename BlueprintFieldType::value_type> bits_msb(params.num_bits);
                            typename BlueprintFieldType::value_type scalar = assignment.var_value(params.scalar);
                            typename BlueprintFieldType::integral_type integral_scalar = typename  BlueprintFieldType::integral_type(scalar.data);
                            for (std::size_t i = 0; i < params.num_bits; i++) {
                                bits_msb[params.num_bits - 1 - i] = multiprecision::bit_test(integral_scalar, i);
                            }

                            typename BlueprintFieldType::value_type a = 2;
                            typename BlueprintFieldType::value_type b = 2;
                            typename BlueprintFieldType::value_type n = 0;

                            for (std::size_t chunk_start = 0; chunk_start < bits_msb.size(); chunk_start += bits_per_row) {
                                assignment.witness(W0)[row] = n;
                                assignment.witness(W2)[row] = a;
                                assignment.witness(W3)[row] = b;

                                for (std::size_t j = 0; j < crumbs_per_row; j++) {
                                    std::size_t crumb = chunk_start + j * bits_per_crumb;
                                    typename BlueprintFieldType::value_type b0 = bits_msb[crumb + 1];
                                    typename BlueprintFieldType::value_type b1 = bits_msb[crumb + 0];

                                    typename BlueprintFieldType::value_type crumb_value = b0 + b1.doubled();
                                    assignment.witness(W7 + j)[row] = crumb_value;

                                    a = a.doubled();
                                    b = b.doubled();

                                    typename BlueprintFieldType::value_type s = 
                                        (b0 == BlueprintFieldType::value_type::one()) ? 1 : -1;

                                    if (b1 == BlueprintFieldType::value_type::zero()) {
                                        b += s;
                                    } else {
                                        a += s;
                                    }

                                    n = (n.doubled()).doubled();
                                    n += crumb_value;
                                }

                                assignment.witness(W1)[row] = n;
                                assignment.witness(W4)[row] = a;
                                assignment.witness(W5)[row] = b;
                                row++;
                            }
                            auto res = a * params.endo_factor + b;
                            assignment.witness(W6)[row - 1] = res;

                            std::cout<<"circuit result "<<res.data<<std::endl;
                            return result_type(component_start_row);
                    }

                    private:
                    static void generate_gates(blueprint<ArithmetizationType> &bp,
                            blueprint_assignment_table<ArithmetizationType> &assignment,
                            const params_type &params,
                            allocated_data_type &allocated_data,
                        const std::size_t &component_start_row = 0) {

                        const std::size_t &j = component_start_row;
                        using F = typename BlueprintFieldType::value_type;

                        std::size_t selector_index_1;
                        std::size_t selector_index_2;
                        if (!allocated_data.previously_allocated) {
                            selector_index_1 = assignment.add_selector(j, j + rows_amount - 1);
                            selector_index_2 = assignment.add_selector(j + rows_amount - 1);
                            allocated_data.selector_1 = selector_index_1;
                            allocated_data.selector_2 = selector_index_2;
                        } else {
                            selector_index_1 = allocated_data.selector_1;
                            selector_index_2 = allocated_data.selector_2;
                            assignment.enable_selector(selector_index_1, j, j + rows_amount - 1); 
                            assignment.enable_selector(selector_index_2, j + rows_amount - 1); 
                        }

                        auto c_f = [](var x) {
                            return (F(11) * F(6).inversed()) * x 
                                + (-F(5) * F(2).inversed()) * x * x
                                + (F(2) * F(3).inversed()) * x * x * x;
                        };

                        auto d_f = [](var x) {
                            return -F::one() + (F(29) * F(6).inversed()) * x 
                                + (-F(7) * F(2).inversed()) * x * x
                                + (F(2) * F(3).inversed()) * x * x * x;
                        };

                        auto constraint_1 = bp.add_constraint(
                            var(W7, 0) * (var(W7, 0) - 1) * (var(W7, 0) - 2) * (var(W7, 0) - 3));
                        auto constraint_2 = bp.add_constraint(
                            var(W8, 0) * (var(W8, 0) - 1) * (var(W8, 0) - 2) * (var(W8, 0) - 3));
                        auto constraint_3 = bp.add_constraint(
                            var(W9, 0) * (var(W9, 0) - 1) * (var(W9, 0) - 2) * (var(W9, 0) - 3));
                        auto constraint_4 = bp.add_constraint(
                            var(W10, 0) * (var(W10, 0) - 1) * (var(W10, 0) - 2) * (var(W10, 0) - 3));
                        auto constraint_5 = bp.add_constraint(
                            var(W11, 0) * (var(W11, 0) - 1) * (var(W11, 0) - 2) * (var(W11, 0) - 3));
                        auto constraint_6 = bp.add_constraint(
                            var(W12, 0) * (var(W12, 0) - 1) * (var(W12, 0) - 2)* (var(W12, 0) - 3));
                        auto constraint_7 = bp.add_constraint(
                            var(W13, 0) * (var(W13, 0) - 1) * (var(W13, 0) - 2)* (var(W13, 0) - 3));
                        auto constraint_8 = bp.add_constraint(
                            var(W14, 0) * (var(W14, 0) - 1) * (var(W14, 0) - 2)* (var(W14, 0) - 3));
                        auto constraint_9 = bp.add_constraint(
                            var(W4, 0) - (256 * var(W2, 0) + 128 * c_f(var(W7, 0)) +
                            64 * c_f(var(W8, 0)) + 32 * c_f(var(W9, 0)) + 16 * c_f(var(W10, 0)) +
                            8 * c_f(var(W11, 0)) + 4 * c_f(var(W12, 0)) + 2 * c_f(var(W13, 0)) + 
                            c_f(var(W14, 0))));
                        auto constraint_10 = bp.add_constraint(
                            var(W5, 0) - (256 * var(W3, 0) + 128 * d_f(var(W7, 0)) +
                            64 * d_f(var(W8, 0)) + 32 * d_f(var(W9, 0)) + 16 * d_f(var(W10, 0)) +
                            8 * d_f(var(W11, 0)) + 4 * d_f(var(W12, 0)) + 2 * d_f(var(W13, 0)) +
                            d_f(var(W14, 0))));
                        auto constraint_11 = bp.add_constraint(
                            var(W1, 0) - ((1 << 16) * var(W0, 0) + (1 << 14) * var(W7, 0) +
                            (1 << 12) * var(W8, 0) + (1 << 10) * var(W9, 0) + (1 << 8) * var(W10, 0) +
                            (1 << 6) * var(W11, 0) + (1 << 4) * var(W12, 0) + (1 << 2) * var(W13, 0) +
                            var(W14, 0)));

                        auto constraint_12 = bp.add_constraint(var(W6, 0) - 
                            (params.endo_factor * var(W4, 0) + var(W5, 0)));
                        if (!allocated_data.previously_allocated) {
                            bp.add_gate(selector_index_2, {constraint_12});
                            bp.add_gate(selector_index_1, 
                            {constraint_1, constraint_2, constraint_3, constraint_4,
                            constraint_5, constraint_6, constraint_7, constraint_8,
                            constraint_9, constraint_10, constraint_11});
                        }
                        allocated_data.previously_allocated = true;
                    }

                    static void generate_copy_constraints(blueprint<ArithmetizationType> &bp,
                            blueprint_assignment_table<ArithmetizationType> &assignment,
                            const params_type &params,
                            const std::size_t &component_start_row = 0){

                        const std::size_t &j = component_start_row;

                        for (std::size_t z = 1; z < rows_amount; z++){
                            bp.add_copy_constraint({{W0, static_cast<int>(j + z), false}, {W1, static_cast<int>(j + z - 1), false}});
                            bp.add_copy_constraint({{W2, static_cast<int>(j + z), false}, {W4, static_cast<int>(j + z - 1), false}});
                            bp.add_copy_constraint({{W3, static_cast<int>(j + z), false}, {W5, static_cast<int>(j + z - 1), false}});
                        }

                        // check that the recalculated n is equal to the input challenge
                        bp.add_copy_constraint({{W1, static_cast<int>(j + rows_amount - 1), false}, params.scalar});
                    }
                };
            }    // namespace components
        }        // namespace zk
    }            // namespace crypto3
}    // namespace nil

#endif    // CRYPTO3_ZK_BLUEPRINT_PLONK_CURVE_ELEMENT_ENDO_SCALAR_COMPONENT_15_WIRES_HPP
