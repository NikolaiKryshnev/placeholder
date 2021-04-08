//---------------------------------------------------------------------------//
// Copyright (c) 2020 Mikhail Komarov <nemo@nil.foundation>
// Copyright (c) 2020 Ilias Khairullin <ilias@nil.foundation>
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

#ifndef CRYPTO3_PUBKEY_RECONSTRUCT_HPP
#define CRYPTO3_PUBKEY_RECONSTRUCT_HPP

#include <boost/assert.hpp>
#include <boost/concept_check.hpp>

#include <boost/range/concepts.hpp>

#include <nil/crypto3/pubkey/algorithm/pubkey.hpp>

#include <nil/crypto3/pubkey/scheme_value.hpp>
#include <nil/crypto3/pubkey/scheme_state.hpp>

#include <nil/crypto3/pubkey/no_key_ops.hpp>

namespace nil {
    namespace crypto3 {
        namespace pubkey {
            template<typename Scheme>
            using secret_reconstructing_policy =
                typename pubkey::modes::isomorphic<Scheme, nop_padding>::secret_reconstructing_policy;
        }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam InputIterator
         * @tparam KeyIterator
         * @tparam OutputIterator
         *
         * @param first
         * @param last
         * @param key_first
         * @param key_last
         * @param out
         *
         * @return
         */
        // template<typename Scheme, typename InputIterator, typename KeyInputIterator, typename OutputIterator>
        // OutputIterator reconstruct(InputIterator first, InputIterator last, KeyInputIterator key_first,
        //                     KeyInputIterator key_last, OutputIterator out) {
        //
        //     typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
        //         pubkey::secret_reconstructing_sss_policy<Scheme>>::type ProcessingMode;
        //     typedef typename pubkey::secret_reconstructing_accumulator_set<ProcessingMode> SchemeAccumulator;
        //
        //     typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
        //     typedef pubkey::detail::itr_scheme_impl<StreamSignerImpl, OutputIterator> SignerImpl;
        //
        //     return SignerImpl(first, last, std::move(out),
        //                       SchemeAccumulator(ProcessingMode(Scheme(pubkey::private_key<Scheme>(key_first, key_last)))));
        // }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam InputIterator
         * @tparam KeySinglePassRange
         * @tparam OutputIterator
         *
         * @param first
         * @param last
         * @param key
         * @param out
         *
         * @return
         */
        // template<typename Scheme, typename InputIterator, typename KeySinglePassRange, typename OutputIterator>
        // OutputIterator reconstruct(InputIterator first, InputIterator last, const KeySinglePassRange &key,
        //                     OutputIterator out) {
        //
        //     typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
        //         pubkey::secret_reconstructing_sss_policy<Scheme>>::type ProcessingMode;
        //     typedef typename pubkey::secret_reconstructing_accumulator_set<ProcessingMode> SchemeAccumulator;
        //
        //     typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
        //     typedef pubkey::detail::itr_scheme_impl<StreamSignerImpl, OutputIterator> SignerImpl;
        //
        //     return SignerImpl(first, last, std::move(out),
        //                       SchemeAccumulator(ProcessingMode(Scheme(pubkey::private_key<Scheme>(key)))));
        // }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam InputIterator
         * @tparam KeyIterator
         * @tparam SchemeAccumulator
         *
         * @param first
         * @param last
         * @param key_first
         * @param key_last
         *
         * @return
         */
        // template<typename Scheme, typename InputIterator, typename KeyInputIterator,
        //          typename SchemeAccumulator = typename pubkey::secret_reconstructing_accumulator_set<typename
        //          pubkey::modes::isomorphic<
        //              Scheme, pubkey::nop_padding>::template
        //              bind<pubkey::secret_reconstructing_sss_policy<Scheme>>::type>>
        // pubkey::detail::range_scheme_impl<pubkey::detail::value_scheme_impl<SchemeAccumulator>>
        //     reconstruct(InputIterator first, InputIterator last, KeyInputIterator key_first, KeyInputIterator
        //     key_last) {
        //
        //     typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
        //         pubkey::secret_reconstructing_sss_policy<Scheme>>::type ProcessingMode;
        //
        //     typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
        //     typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;
        //
        //     return SignerImpl(first, last,
        //                       SchemeAccumulator(ProcessingMode(Scheme(pubkey::private_key<Scheme>(key_first, key_last)))));
        // }

        /*!
         * @brief
         *
         * @tparam Scheme
         * @tparam InputIterator
         * @tparam KeySinglePassRange
         * @tparam SchemeAccumulator
         *
         * @param first
         * @param last
         * @param key
         *
         * @return
         */
        // template<typename Scheme, typename InputIterator, typename KeySinglePassRange,
        //          typename SchemeAccumulator = typename pubkey::secret_reconstructing_accumulator_set<typename
        //          pubkey::modes::isomorphic<
        //              Scheme, pubkey::nop_padding>::template
        //              bind<pubkey::secret_reconstructing_sss_policy<Scheme>>::type>>
        // pubkey::detail::range_scheme_impl<pubkey::detail::value_scheme_impl<SchemeAccumulator>>
        //     reconstruct(InputIterator first, InputIterator last, const KeySinglePassRange &key) {
        //
        //     typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
        //         pubkey::secret_reconstructing_sss_policy<Scheme>>::type ProcessingMode;
        //
        //     typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
        //     typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;
        //
        //     return SignerImpl(first, last, SchemeAccumulator(ProcessingMode(Scheme(pubkey::private_key<Scheme>(key)))));
        // }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam SinglePassRange
         * @tparam KeyRange
         * @tparam OutputIterator
         *
         * @param rng
         * @param key
         * @param out
         *
         * @return
         */
        // template<typename Scheme, typename SinglePassRange, typename KeySinglePassRange, typename OutputIterator>
        // OutputIterator reconstruct(const SinglePassRange &rng, const KeySinglePassRange &key, OutputIterator out) {
        //
        //     typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
        //         pubkey::secret_reconstructing_sss_policy<Scheme>>::type ProcessingMode;
        //     typedef typename pubkey::secret_reconstructing_accumulator_set<ProcessingMode> SchemeAccumulator;
        //
        //     typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
        //     typedef pubkey::detail::itr_scheme_impl<StreamSignerImpl, OutputIterator> SignerImpl;
        //
        //     return SignerImpl(rng, std::move(out),
        //     SchemeAccumulator(ProcessingMode(Scheme(pubkey::private_key<Scheme>(key)))));
        // }

        /*!
         * @brief
         *
         * @tparam Scheme
         * @tparam SinglePassRange
         * @tparam KeySinglePassRange
         * @tparam OutputRange
         *
         * @param rng
         * @param key
         * @param out
         *
         * @return
         */
        // template<typename Scheme, typename SinglePassRange, typename KeySinglePassRange, typename OutputRange>
        // OutputRange &reconstruct(const SinglePassRange &rng, const KeySinglePassRange &key, OutputRange &out) {
        //
        //     typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
        //         pubkey::secret_reconstructing_sss_policy<Scheme>>::type ProcessingMode;
        //     typedef typename pubkey::secret_reconstructing_accumulator_set<ProcessingMode> SchemeAccumulator;
        //
        //     typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
        //     typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;
        //
        //     return SignerImpl(rng, std::move(out),
        //     SchemeAccumulator(ProcessingMode(Scheme(pubkey::private_key<Scheme>(key)))));
        // }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam SinglePassRange
         * @tparam KeyRange
         * @tparam SchemeAccumulator
         *
         * @param r
         * @param key
         *
         * @return
         */
        // template<typename Scheme, typename SinglePassRange, typename KeySinglePassRange,
        //          typename SchemeAccumulator = typename pubkey::secret_reconstructing_accumulator_set<typename
        //          pubkey::modes::isomorphic<
        //              Scheme, pubkey::nop_padding>::template
        //              bind<pubkey::secret_reconstructing_sss_policy<Scheme>>::type>>
        // pubkey::detail::range_scheme_impl<pubkey::detail::value_scheme_impl<SchemeAccumulator>>
        //     reconstruct(const SinglePassRange &r, const KeySinglePassRange &key) {
        //
        //     typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
        //         pubkey::secret_reconstructing_sss_policy<Scheme>>::type ProcessingMode;
        //
        //     typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
        //     typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;
        //
        //     return SignerImpl(r, SchemeAccumulator(ProcessingMode(Scheme(pubkey::private_key<Scheme>(key)))));
        // }

        /*!
         * @brief
         *
         * * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam SinglePassRange
         * @tparam OutputRange
         *
         * @param rng
         * @param key
         * @param out
         *
         * @return
         */
        // template<typename Scheme, typename SinglePassRange, typename OutputRange>
        // OutputRange &reconstruct(const SinglePassRange &rng, const pubkey::private_key<Scheme> &key, OutputRange
        // &out) {
        //
        //     typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
        //         pubkey::secret_reconstructing_sss_policy<Scheme>>::type ProcessingMode;
        //     typedef typename pubkey::secret_reconstructing_accumulator_set<ProcessingMode> SchemeAccumulator;
        //
        //     typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
        //     typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;
        //
        //     return SignerImpl(rng, std::move(out), SchemeAccumulator(key));
        // }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam InputIterator
         * @tparam OutputIterator
         *
         * @param first
         * @param last
         * @param key
         * @param out
         *
         * @return
         */
        template<typename Scheme, typename InputIterator, typename OutputIterator>
        typename std::enable_if<!boost::accumulators::detail::is_accumulator_set<OutputIterator>::value,
                                OutputIterator>::type
            reconstruct(InputIterator first, InputIterator last, OutputIterator out) {

            typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                pubkey::secret_reconstructing_policy<Scheme>>::type ProcessingMode;
            typedef typename pubkey::secret_reconstructing_accumulator_set<ProcessingMode> SchemeAccumulator;

            typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
            typedef pubkey::detail::itr_scheme_impl<StreamSignerImpl, OutputIterator> SignerImpl;

            return SignerImpl(first, last, std::move(out), SchemeAccumulator());
        }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam SinglePassRange
         * @tparam OutputIterator
         *
         * @param rng
         * @param key
         * @param out
         *
         * @return
         */
        template<typename Scheme, typename SinglePassRange, typename OutputIterator>
        typename std::enable_if<!boost::accumulators::detail::is_accumulator_set<OutputIterator>::value,
                                OutputIterator>::type
            reconstruct(const SinglePassRange &rng, OutputIterator out) {

            typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                pubkey::secret_reconstructing_policy<Scheme>>::type ProcessingMode;
            typedef typename pubkey::secret_reconstructing_accumulator_set<ProcessingMode> SchemeAccumulator;

            typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
            typedef pubkey::detail::itr_scheme_impl<StreamSignerImpl, OutputIterator> SignerImpl;

            return SignerImpl(rng, std::move(out), SchemeAccumulator());
        }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam InputIterator
         * @tparam OutputAccumulator
         *
         * @param first
         * @param last
         * @param acc
         *
         * @return
         */
        template<typename Scheme, typename InputIterator,
                 typename OutputAccumulator = typename pubkey::secret_reconstructing_accumulator_set<
                     typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                         pubkey::secret_reconstructing_policy<Scheme>>::type>>
        typename std::enable_if<boost::accumulators::detail::is_accumulator_set<OutputAccumulator>::value,
                                OutputAccumulator>::type &
            reconstruct(InputIterator first, InputIterator last, OutputAccumulator &acc) {

            typedef pubkey::detail::ref_scheme_impl<OutputAccumulator> StreamSignerImpl;
            typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;

            return SignerImpl(first, last, std::forward<OutputAccumulator>(acc));
        }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam SinglePassRange
         * @tparam OutputAccumulator
         *
         * @param r
         * @param acc
         *
         * @return
         */
        template<typename Scheme, typename SinglePassRange,
                 typename OutputAccumulator = typename pubkey::secret_reconstructing_accumulator_set<
                     typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                         pubkey::secret_reconstructing_policy<Scheme>>::type>>
        typename std::enable_if<boost::accumulators::detail::is_accumulator_set<OutputAccumulator>::value,
                                OutputAccumulator>::type &
            reconstruct(const SinglePassRange &r, OutputAccumulator &acc) {

            typedef pubkey::detail::ref_scheme_impl<OutputAccumulator> StreamSignerImpl;
            typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;

            return SignerImpl(r, std::forward<OutputAccumulator>(acc));
        }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam InputIterator
         * @tparam KeySinglePassRange
         * @tparam SchemeAccumulator
         *
         * @param first
         * @param last
         * @param key
         *
         * @return
         */
        template<typename Scheme, typename InputIterator,
                 typename SchemeAccumulator = typename pubkey::secret_reconstructing_accumulator_set<
                     typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                         pubkey::secret_reconstructing_policy<Scheme>>::type>>
        pubkey::detail::range_scheme_impl<pubkey::detail::value_scheme_impl<SchemeAccumulator>>
            reconstruct(InputIterator first, InputIterator last) {

            typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                pubkey::secret_reconstructing_policy<Scheme>>::type ProcessingMode;

            typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
            typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;

            return SignerImpl(first, last, SchemeAccumulator());
        }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam SinglePassRange
         * @tparam SchemeAccumulator
         *
         * @param r
         * @param key
         *
         * @return
         */
        template<typename Scheme, typename SinglePassRange,
                 typename SchemeAccumulator = typename pubkey::secret_reconstructing_accumulator_set<
                     typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                         pubkey::secret_reconstructing_policy<Scheme>>::type>>
        pubkey::detail::range_scheme_impl<pubkey::detail::value_scheme_impl<SchemeAccumulator>>
            reconstruct(const SinglePassRange &r) {

            typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                pubkey::secret_reconstructing_policy<Scheme>>::type ProcessingMode;

            typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
            typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;

            return SignerImpl(r, SchemeAccumulator());
        }

        /*!
         * @brief
         *
         * @ingroup pubkey_algorithms
         *
         * @tparam Scheme
         * @tparam InputIterator
         * @tparam KeySinglePassRange
         * @tparam SchemeAccumulator
         *
         * @param first
         * @param last
         * @param key
         *
         * @return
         */
        template<typename Scheme, typename InputIterator1, typename InputIterator2,
            typename SchemeAccumulator = typename pubkey::secret_reconstructing_accumulator_set<
                typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                    pubkey::secret_reconstructing_policy<Scheme>>::type>>
        pubkey::detail::range_scheme_impl<pubkey::detail::value_scheme_impl<SchemeAccumulator>>
        reconstruct(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {

            typedef typename pubkey::modes::isomorphic<Scheme, pubkey::nop_padding>::template bind<
                pubkey::secret_reconstructing_policy<Scheme>>::type ProcessingMode;

            typedef pubkey::detail::value_scheme_impl<SchemeAccumulator> StreamSignerImpl;
            typedef pubkey::detail::range_scheme_impl<StreamSignerImpl> SignerImpl;

            return SignerImpl(first1, last1, first2, last2, SchemeAccumulator());
        }
    }    // namespace crypto3
}    // namespace nil

#endif    // include guard