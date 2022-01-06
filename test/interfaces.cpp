//---------------------------------------------------------------------------//
// Copyright (c) 2020-2021 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Alexander Sokolov <asokolov@nil.foundation>
// Copyright (c) 2020-2021 Nikita Kaskov <nbering@nil.foundation>
// Copyright (c) 2021 Aleksei Moskvin <alalmoskvin@gmail.com>
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

#define BOOST_TEST_MODULE hash_pack_test

#include <boost/array.hpp>
#include <boost/cstdint.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <nil/marshalling/types/integral.hpp>
#include <nil/marshalling/types/bitmask_value.hpp>
#include <nil/marshalling/types/enumeration.hpp>
#include <nil/marshalling/types/array_list.hpp>
#include <nil/marshalling/types/string.hpp>
#include <nil/marshalling/types/bitfield.hpp>
#include <nil/marshalling/types/optional.hpp>
#include <nil/marshalling/types/bundle.hpp>
#include <nil/marshalling/types/float_value.hpp>
#include <nil/marshalling/types/no_value.hpp>
#include <nil/marshalling/types/variant.hpp>

#include <nil/marshalling/algorithms/unpack.hpp>
#include <nil/marshalling/algorithms/pack.hpp>
#include <nil/marshalling/algorithms/repack.hpp>

#include <boost/container/static_vector.hpp>

#include <boost/type_traits/is_class.hpp>

#include <cstdio>
#include <variant>

using namespace nil::marshalling;

BOOST_AUTO_TEST_SUITE(pack_interfaces_test_suite)

template<typename Vector>
typename std::enable_if<std::is_class<Vector>::value, void>::type resize(Vector &t, size_t count) {
    t.resize(count);
}

template<typename T>
typename std::enable_if<!std::is_class<T>::value, void>::type resize(T &t, size_t count) {
}

template<typename T, size_t ArraySize>
void resize(std::array<T, ArraySize> &t, size_t count) {
    BOOST_CHECK(ArraySize == count);
}

template<typename T, size_t ArraySize>
void resize(boost::array<T, ArraySize> &t, size_t count) {
    BOOST_CHECK(ArraySize == count);
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<TInput>::value, void>::type repack_2(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<TInput>::value, void>::type repack_4(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<T>::value, void>::type repack_5(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<T>::value || !nil::detail::has_begin<TInput>::value, void>::type
    repack_6(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<T>::value, void>::type repack_7(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<T>::value || !nil::detail::has_begin<TInput>::value, void>::type
    repack_8(TInput in) {
}

template<typename T, typename TInput>
void repack_1(TInput in) {
    status_type status;
    T result = repack<option::big_endian, option::big_endian>(in, status);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<TInput>::value, void>::type repack_2(TInput in) {
    status_type status;
    T result = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), status);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
void repack_3(TInput in) {
    status_type status;
    T result;
    status = repack<option::big_endian, option::big_endian>(in, result);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<TInput>::value, void>::type repack_4(TInput in) {
    status_type status;
    T result;
    status = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), result);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<T>::value, void>::type repack_5(TInput in) {
    status_type status;
    T result;
    resize(result, 2);
    typename T::iterator itr = result.begin();
    itr = repack<option::big_endian, option::big_endian>(in, result.begin(), status);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<T>::value && nil::detail::has_begin<TInput>::value, void>::type
    repack_6(TInput in) {
    status_type status;
    T result;
    resize(result, 2);
    typename T::iterator itr = result.begin();
    itr = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), result.begin(), status);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<T>::value, void>::type repack_7(TInput in) {
    status_type status;
    T result;
    resize(result, 2);
    status = repack<option::big_endian, option::big_endian>(in, result.begin());
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<T>::value && nil::detail::has_begin<TInput>::value, void>::type
    repack_8(TInput in) {
    status_type status;
    T result;
    resize(result, 2);
    status = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), result.begin());
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<TInput>::value, void>::type repack_2_unpack(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<TInput>::value, void>::type repack_4_unpack(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<T>::value, void>::type repack_5_unpack(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<T>::value || !nil::detail::has_begin<TInput>::value, void>::type
    repack_6_unpack(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<T>::value, void>::type repack_7_unpack(TInput in) {
}

template<typename T, typename TInput>
typename std::enable_if<!nil::detail::has_begin<T>::value || !nil::detail::has_begin<TInput>::value, void>::type
    repack_8_unpack(TInput in) {
}

template<typename T, typename TInput>
void repack_1_unpack(TInput in) {
    status_type status;
    T result = repack<option::big_endian, option::big_endian>(in, status);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<TInput>::value, void>::type repack_2_unpack(TInput in) {
    status_type status;
    T result = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), status);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
void repack_3_unpack(TInput in) {
    status_type status;
    T result;
    status = repack<option::big_endian, option::big_endian>(in, result);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<TInput>::value, void>::type repack_4_unpack(TInput in) {
    status_type status;
    T result;
    status = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), result);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<T>::value, void>::type repack_5_unpack(TInput in) {
    status_type status;
    T result;
    resize(result, 4);
    typename T::iterator itr = result.begin();
    itr = repack<option::big_endian, option::big_endian>(in, result.begin(), status);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<T>::value && nil::detail::has_begin<TInput>::value, void>::type
    repack_6_unpack(TInput in) {
    status_type status;
    T result;
    resize(result, 4);
    typename T::iterator itr = result.begin();
    itr = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), result.begin(), status);
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<T>::value, void>::type repack_7_unpack(TInput in) {
    status_type status;
    T result;
    resize(result, 4);
    status = repack<option::big_endian, option::big_endian>(in, result.begin());
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename TInput>
typename std::enable_if<nil::detail::has_begin<T>::value && nil::detail::has_begin<TInput>::value, void>::type
    repack_8_unpack(TInput in) {
    status_type status;
    T result;
    resize(result, 4);
    status = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), result.begin());
    BOOST_CHECK(status == status_type::success);
}

template<typename T, typename Tinput>
void test_unpack_marshalling(Tinput in) {
    status_type status;

    T result1 = repack<option::big_endian, option::big_endian>(in, status);
    BOOST_CHECK(status == status_type::success);

    T result3;
    status = repack<option::big_endian, option::big_endian>(in, result3);
    BOOST_CHECK(status == status_type::success);

    T result5;
    resize(result5, 4);
    repack<option::big_endian, option::big_endian>(in, result5.begin(), status);
    BOOST_CHECK(status == status_type::success);

    T result7;
    resize(result7, 4);
    status = repack<option::big_endian, option::big_endian>(in, result7.begin());
    BOOST_CHECK(status == status_type::success);
}

typedef boost::mpl::list<std::vector<uint16_t>,
                         std::array<uint16_t, 2>,
                         boost::container::static_vector<uint16_t, 2>,
                         boost::array<uint16_t, 2>,
                         std::uint32_t,
                         types::integral<field_type<option::big_endian>, std::uint16_t>>
    test_types_repack;

template<typename T, typename TInput>
void call_repack(TInput in) {
    repack_1<T>(in);
    repack_2<T>(in);
    repack_3<T>(in);
    repack_4<T>(in);
    repack_5<T>(in);
    repack_6<T>(in);
    repack_7<T>(in);
    repack_8<T>(in);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(pack_test, T, test_types_repack) {
    std::vector<uint8_t> in1 = {0x12, 0x34, 0x56, 0x78};
    call_repack<T>(in1);

    std::array<uint8_t, 4> in2 = {0x12, 0x34, 0x56, 0x78};
    call_repack<T>(in1);
}

typedef boost::mpl::list<std::vector<uint8_t>,
                         std::array<uint8_t, 4>,
                         boost::container::static_vector<uint8_t, 4>,
                         boost::array<uint8_t, 4>>
    test_types_repack_unpack;

template<typename T, typename TInput>
void call_unpack(TInput in) {
    repack_1_unpack<T>(in);
    repack_2_unpack<T>(in);
    repack_3_unpack<T>(in);
    repack_4_unpack<T>(in);
    repack_5_unpack<T>(in);
    repack_6_unpack<T>(in);
    repack_7_unpack<T>(in);
    repack_8_unpack<T>(in);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(unpack_test, T, test_types_repack_unpack) {
    std::vector<uint16_t> in1 = {{0x1234, 0x5678}};
    call_unpack<T>(in1);

    std::array<uint16_t, 2> in2 = {{0x1234, 0x5678}};
    call_unpack<T>(in2);

    uint32_t in3 = 0x12345678;
    call_unpack<T>(in3);

    using input_type = types::array_list<field_type<option::little_endian>,
                                         types::integral<field_type<option::big_endian>, std::uint8_t>,
                                         option::fixed_size_storage<2>>;
    using input_seed_type = typename input_type::value_type;
    input_type in4;
    std::array<std::uint16_t, 2> inp_seed_blank = {{0x1234, 0x5678}};
    input_seed_type &inp_seed = in4.value();
    for (auto it = inp_seed_blank.begin(); it != inp_seed_blank.end(); ++it) {
        inp_seed.push_back(typename input_seed_type::value_type(*it));
    }
    test_unpack_marshalling<T>(in4);
}

BOOST_AUTO_TEST_CASE(repack_test) {
    using T = std::vector<std::uint16_t>;
    using Tout = std::vector<std::uint32_t>;
    std::vector<std::uint16_t> in = {0x1234, 0x5678, 0x90ab, 0xcdef};
    std::vector<std::uint32_t> res = {{0x12345678, 0x90abcdef}};

    status_type status;

    T result1 = repack<option::big_endian, option::big_endian>(in, status);
    BOOST_CHECK(status == status_type::success);

    T result2 = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), status);
    BOOST_CHECK(status == status_type::success);

    T result3;
    status = repack<option::big_endian, option::big_endian>(in, result3);
    BOOST_CHECK(status == status_type::success);

    T result4;
    status = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), result4);
    BOOST_CHECK(status == status_type::success);

    T result5;
    resize(result5, 2);
    repack<option::big_endian, option::big_endian>(in, result5.begin(), status);
    BOOST_CHECK(status == status_type::success);

    T result6;
    resize(result6, 2);
    repack<option::big_endian, option::big_endian>(in.begin(), in.end(), result6.begin(), status);
    BOOST_CHECK(status == status_type::success);

    T result7;
    resize(result7, 2);
    status = repack<option::big_endian, option::big_endian>(in, result7.begin());
    BOOST_CHECK(status == status_type::success);

    T result8;
    resize(result8, 2);
    status = repack<option::big_endian, option::big_endian>(in.begin(), in.end(), result8.begin());
    BOOST_CHECK(status == status_type::success);
}

BOOST_AUTO_TEST_SUITE_END()
