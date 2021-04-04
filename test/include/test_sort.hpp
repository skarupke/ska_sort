//          Copyright Malte Skarupke 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See http://www.boost.org/LICENSE_1_0.txt)

#ifndef TEST_SORT_H
#define TEST_SORT_H

#include <cstddef>
#include <limits>
#include <type_traits>

#include <ska/sort.hpp>

template<typename It, typename OutIt, typename ExtractKey>
void counting_sort(It begin, It end, OutIt out_begin, ExtractKey && extract_key)
{
    ska::detail::counting_sort_impl(begin, end, out_begin, extract_key);
}

template<typename It, typename OutIt>
void counting_sort(It begin, It end, OutIt out_begin)
{
    using ska::detail::to_unsigned_or_bool;
    ska::detail::counting_sort_impl(begin, end, out_begin, [](auto && a){ return to_unsigned_or_bool(a); });
}

template<typename It, typename OutIt, typename ExtractKey>
bool radix_sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
{
    return ska::detail::RadixSorter<typename std::result_of<ExtractKey(decltype(*begin))>::type>::sort(begin, end, buffer_begin, extract_key);
}

template<typename It, typename OutIt>
bool radix_sort(It begin, It end, OutIt buffer_begin)
{
    return ska::detail::RadixSorter<decltype(*begin)>::sort(begin, end, buffer_begin, ska::detail::IdentityFunctor());
}

template<typename It, typename ExtractKey>
void inplace_radix_sort(It begin, It end, ExtractKey && extract_key)
{
    ska::detail::inplace_radix_sort<1, 1>(begin, end, extract_key);
}

template<typename It>
void inplace_radix_sort(It begin, It end)
{
    inplace_radix_sort(begin, end, ska::detail::IdentityFunctor());
}

template<typename It, typename ExtractKey>
void american_flag_sort(It begin, It end, ExtractKey && extract_key)
{
    ska::detail::inplace_radix_sort<1, std::numeric_limits<std::ptrdiff_t>::max()>(begin, end, extract_key);
}

template<typename It>
void american_flag_sort(It begin, It end)
{
    american_flag_sort(begin, end, ska::detail::IdentityFunctor());
}

#endif // TEST_SORT_H
