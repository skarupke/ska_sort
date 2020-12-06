//          Copyright Malte Skarupke 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See http://www.boost.org/LICENSE_1_0.txt)

#include "ska_sort.hpp"

#define FULL_TESTS_SLOW_COMPILE_TIME

#define SKA_SORT_NOINLINE __attribute__((noinline))

template<typename It, typename OutIt, typename ExtractKey>
void counting_sort(It begin, It end, OutIt out_begin, ExtractKey && extract_key)
{
    detail::counting_sort_impl(begin, end, out_begin, extract_key);
}
template<typename It, typename OutIt>
void counting_sort(It begin, It end, OutIt out_begin)
{
    using detail::to_unsigned_or_bool;
    detail::counting_sort_impl(begin, end, out_begin, [](auto && a){ return to_unsigned_or_bool(a); });
}

template<typename It, typename OutIt, typename ExtractKey>
bool radix_sort(It begin, It end, OutIt buffer_begin, ExtractKey && extract_key)
{
    return detail::RadixSorter<typename std::result_of<ExtractKey(decltype(*begin))>::type>::sort(begin, end, buffer_begin, extract_key);
}
template<typename It, typename OutIt>
bool radix_sort(It begin, It end, OutIt buffer_begin)
{
    return detail::RadixSorter<decltype(*begin)>::sort(begin, end, buffer_begin, detail::IdentityFunctor());
}

template<typename It, typename ExtractKey>
static void inplace_radix_sort(It begin, It end, ExtractKey && extract_key)
{
    detail::inplace_radix_sort<1, 1>(begin, end, extract_key);
}

template<typename It>
static void inplace_radix_sort(It begin, It end)
{
    inplace_radix_sort(begin, end, detail::IdentityFunctor());
}

template<typename It, typename ExtractKey>
static void american_flag_sort(It begin, It end, ExtractKey && extract_key)
{
    detail::inplace_radix_sort<1, std::numeric_limits<std::ptrdiff_t>::max()>(begin, end, extract_key);
}

template<typename It>
static void american_flag_sort(It begin, It end)
{
    american_flag_sort(begin, end, detail::IdentityFunctor());
}


#ifdef ENABLE_GTEST

#include <vector>
#include <gtest/gtest.h>

TEST(counting_sort, simple)
{
    std::vector<uint8_t> to_sort = { 5, 6, 19, 2, 5, 0, 7, 23, 6, 8, 99 };
    std::vector<uint8_t> result(to_sort.size());
    counting_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    std::sort(to_sort.begin(), to_sort.end());
    ASSERT_EQ(to_sort, result);
}

TEST(counting_sort, string)
{
    std::string to_sort = "Hello, World!";
    std::string result = to_sort;
    counting_sort(to_sort.begin(), to_sort.end(), result.begin());
    std::sort(to_sort.begin(), to_sort.end());
    ASSERT_EQ(to_sort, result);
}

TEST(radix_sort, uint8)
{
    std::vector<uint8_t> to_sort = { 5, 6, 19, 2, 5, 0, 7, 23, 6, 255, 8, 99 };
    std::vector<uint8_t> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, uint8_256_items)
{
    std::vector<uint8_t> to_sort(256, 254);
    to_sort.back() = 255;
    std::vector<uint8_t> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, int8)
{
    std::vector<int8_t> to_sort = { 5, 6, 19, -4, 2, 5, 0, -55, 7, 23, 6, 8, 127, -128, 99 };
    std::vector<int8_t> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, text)
{
    std::string to_sort = "Hello, World!";
    std::string result(to_sort.size(), ' ');
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto c){ return c; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, u16string)
{
    std::u16string to_sort = u"Hello, World!";
    std::u16string result(to_sort.size(), ' ');
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto c){ return c; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, u32string)
{
    std::u32string to_sort = U"Hello, World!";
    std::u32string result(to_sort.size(), ' ');
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto c){ return c; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, int16)
{
    std::vector<int16_t> to_sort = { 5, 6, 19, -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, -32768, 32767, 99 };
    std::vector<int16_t> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, uint16)
{
    std::vector<uint16_t> to_sort = { 5, 6, 19, 2, 5, 7, 0, 23, 6, 256, 255, 8, 99, 1024, 65535, 65534 };
    std::vector<uint16_t> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, int32)
{
    std::vector<int32_t> to_sort = { 5, 6, 19, -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, 32768, -32769, -32768, 32767, 99, 1000000, -1000001, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max() - 1, std::numeric_limits<int>::lowest() + 1 };
    std::vector<int32_t> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, uint32)
{
    std::vector<uint32_t> to_sort = { 5, 6, 19, 2, 5, 7, 0, 23, 6, 256, 255, 8, 99, 1024, 65536, 65535, 65534, 1000000, std::numeric_limits<unsigned int>::max() };
    std::vector<uint32_t> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, int64)
{
    std::vector<int64_t> to_sort = { 5, 6, 19, std::numeric_limits<std::int32_t>::lowest() - 1, std::numeric_limits<ino64_t>::lowest(), -1000000000000, 1000000000000, std::numeric_limits<int32_t>::max(), std::numeric_limits<int64_t>::max(), -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, 32768, -32769, -32768, 32767, 99, 1000000, -1000001, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max() - 1, std::numeric_limits<int>::lowest() + 1 };
    std::vector<int64_t> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, uint64)
{
    std::vector<uint64_t> to_sort = { 5, 6, 19, 2, 5, 7, 0, std::numeric_limits<uint32_t>::max() + 1, 1000000000000, std::numeric_limits<uint64_t>::max(), 23, 6, 256, 255, 8, 99, 1024, 65536, 65535, 65534, 1000000, std::numeric_limits<unsigned int>::max() };
    std::vector<uint64_t> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, float)
{
    std::vector<float> to_sort = { 5, 6, 19, std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, 32768, -32769, -32768, 32767, 99, 1000000, -1000001, 0.1f, 2.5f, 17.8f, -12.4f, -0.0000002f, -0.0f, -777777777.7f, 444444444444.4f };
    std::vector<float> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, double)
{
    std::vector<double> to_sort = { 5, 6, 19, std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, 32768, -32769, -32768, 32767, 99, 1000000, -1000001, 0.1, 2.5, 17.8, -12.4, -0.0000002, -0.0, -777777777.7, 444444444444.4 };
    std::vector<double> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, pair)
{
    std::vector<std::pair<int, bool>> to_sort = { { 5, true }, { 5, false }, { 6, false }, { 7, true }, { 4, false }, { 4, true } };
    std::vector<std::pair<int, bool>> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, pair_other_direction)
{
    std::vector<std::pair<bool, int>> to_sort = { { true, 5 }, { false, 5 }, { false, 6 }, { true, 7 }, { false, 4 }, { true, 4 } };
    std::vector<std::pair<bool, int>> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, tuple)
{
    std::vector<std::tuple<bool, int, bool>> to_sort = { std::tuple<bool, int, bool>{ true, 5, true }, std::tuple<bool, int, bool>{ true, 5, false }, std::tuple<bool, int, bool>{ false, 6, false }, std::tuple<bool, int, bool>{ true, 7, true }, std::tuple<bool, int, bool>{ true, 4, false }, std::tuple<bool, int, bool>{ false, 4, true }, std::tuple<bool, int, bool>{ false, 5, false } };
    std::vector<std::tuple<bool, int, bool>> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto i){ return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, reference)
{
    std::vector<int> to_sort = { 6, 5, 4, 3, 2, 1 };
    std::vector<int> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](int & i) -> int & { return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, pair_reference)
{
    std::vector<std::pair<int, bool>> to_sort = { { 5, true }, { 5, false }, { 6, false }, { 7, true }, { 4, false }, { 4, true } };
    std::vector<std::pair<int, bool>> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto & i) -> decltype(auto) { return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(to_sort, result);
}
TEST(radix_sort, tuple_reference)
{
    std::vector<std::tuple<bool, int, bool>> to_sort = { std::tuple<bool, int, bool>{ true, 5, true }, std::tuple<bool, int, bool>{ true, 5, false }, std::tuple<bool, int, bool>{ false, 6, false }, std::tuple<bool, int, bool>{ true, 7, true }, std::tuple<bool, int, bool>{ true, 4, false }, std::tuple<bool, int, bool>{ false, 4, true }, std::tuple<bool, int, bool>{ false, 5, false } };
    std::vector<std::tuple<bool, int, bool>> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto & i) -> decltype(auto) { return i; });
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, std_array)
{
    std::vector<std::array<float, 4>> to_sort = { {{ 1.0f, 2.0f, 3.0f, 4.0f }}, {{ 0.0f, 3.0f, 4.0f, 5.0f }}, {{ 1.0f, 1.5f, 2.0f, 2.5f }}, {{ 1.0f, 2.0f, 2.5f, 4.0f }}, {{ 1.0f, 2.0f, 2.5f, 3.5f }}, {{ 0.0f, 3.0f, 4.5f, 4.5f }} };
    std::vector<std::array<float, 4>> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin());
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}
TEST(radix_sort, move_only)
{
    std::vector<std::unique_ptr<int>> to_sort;
    to_sort.push_back(std::make_unique<int>(5));
    to_sort.push_back(std::make_unique<int>(0));
    to_sort.push_back(std::make_unique<int>(1234567));
    to_sort.push_back(std::make_unique<int>(-1000));
    std::vector<int> sorted;
    sorted.reserve(to_sort.size());
    for (const std::unique_ptr<int> & i : to_sort)
        sorted.push_back(*i);
    std::sort(sorted.begin(), sorted.end());
    std::vector<std::unique_ptr<int>> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin(), [](auto & i){ return *i; });
    for (size_t i = 0; i < sorted.size(); ++i)
    {
        if (which_buffer)
            ASSERT_EQ(sorted[i], *result[i]);
        else
            ASSERT_EQ(sorted[i], *to_sort[i]);
    }
}

TEST(radix_sort, vector_bool)
{
    std::vector<bool> to_sort = { true, false, true, true, false, true, true, true, false, true, false, false };
    std::vector<bool> result(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), result.begin());
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}

TEST(linear_sort, tuple)
{
    std::vector<std::tuple<bool, int, bool>> to_sort = { std::tuple<bool, int, bool>{ true, 5, true }, std::tuple<bool, int, bool>{ true, 5, false }, std::tuple<bool, int, bool>{ false, 6, false }, std::tuple<bool, int, bool>{ true, 7, true }, std::tuple<bool, int, bool>{ true, 4, false }, std::tuple<bool, int, bool>{ false, 4, true }, std::tuple<bool, int, bool>{ false, 5, false } };
    std::vector<std::tuple<bool, int, bool>> result = to_sort;
    bool which_buffer = ska_sort_copy(to_sort.begin(), to_sort.end(), result.begin());
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}

/*TEST(linear_sort, string)
{
    std::vector<std::string> to_sort = { "foo", "bar", "baz" };
    std::vector<std::string> result = to_sort;
    bool which_buffer = ska_sort_copy(to_sort.begin(), to_sort.end(), result.begin());
    if (which_buffer)
        std::sort(to_sort.begin(), to_sort.end());
    else
        std::sort(result.begin(), result.end());
    ASSERT_EQ(result, to_sort);
}*/

TEST(inplace_radix_sort, uint8)
{
    std::vector<uint8_t> to_sort = { 5, 6, 19, 2, 5, 0, 7, 23, 6, 255, 8, 99 };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, int8)
{
    std::vector<int8_t> to_sort = { 5, 6, 19, -4, 2, 5, 0, -55, 7, 23, 6, 8, 127, -128, 99 };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, text)
{
    std::string to_sort = "Hello, World!";
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, u16string)
{
    std::u16string to_sort = u"Hello, World!";
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, u32string)
{
    std::u32string to_sort = U"Hello, World!";
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, int16)
{
    std::vector<int16_t> to_sort = { 5, 6, 19, -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, -32768, 32767, 99 };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, uint16)
{
    std::vector<uint16_t> to_sort = { 5, 6, 19, 2, 5, 7, 0, 23, 6, 256, 255, 8, 99, 1024, 65535, 65534 };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, int32)
{
    std::vector<int32_t> to_sort = { 5, 6, 19, -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, 32768, -32769, -32768, 32767, 99, 1000000, -1000001, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max() - 1, std::numeric_limits<int>::lowest() + 1 };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, uint32)
{
    std::vector<uint32_t> to_sort = { 5, 6, 19, 2, 5, 7, 0, 23, 6, 256, 255, 8, 99, 1024, 65536, 65535, 65534, 1000000, std::numeric_limits<unsigned int>::max() };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, int64)
{
    std::vector<int64_t> to_sort = { 5, 6, 19, std::numeric_limits<std::int32_t>::lowest() - 1, std::numeric_limits<ino64_t>::lowest(), -1000000000000, 1000000000000, std::numeric_limits<int32_t>::max(), std::numeric_limits<int64_t>::max(), -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, 32768, -32769, -32768, 32767, 99, 1000000, -1000001, std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max() - 1, std::numeric_limits<int>::lowest() + 1 };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, uint64)
{
    std::vector<uint64_t> to_sort = { 5, 6, 19, 2, 5, 7, 0, std::numeric_limits<uint32_t>::max() + 1, 1000000000000, std::numeric_limits<uint64_t>::max(), 23, 6, 256, 255, 8, 99, 1024, 65536, 65535, 65534, 1000000, std::numeric_limits<unsigned int>::max() };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, float)
{
    std::vector<float> to_sort = { 5, 6, 19, std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, 32768, -32769, -32768, 32767, 99, 1000000, -1000001, 0.1f, 2.5f, 17.8f, -12.4f, -0.0000002f, -0.0f, -777777777.7f, 444444444444.4f };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, double)
{
    std::vector<double> to_sort = { 5, 6, 19, std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(), -4, 2, 5, 0, -55, 7, 1000, 23, 6, 8, 127, -128, -129, -256, 32768, -32769, -32768, 32767, 99, 1000000, -1000001, 0.1, 2.5, 17.8, -12.4, -0.0000002, -0.0, -777777777.7, 444444444444.4 };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}

TEST(inplace_radix_sort, error_case)
{
    std::vector<int8_t> data = { 46, 7, 33, -78, -114, -78, 33, 82 };
    std::vector<int8_t> copy = data;
    std::sort(copy.begin(), copy.end());
    inplace_radix_sort(data.begin(), data.end());
    ASSERT_EQ(copy, data);
}

TEST(inplace_radix_sort, another_error_case)
{
    std::vector<int8_t> data = { -104, 50, 108, 105, 112, 53, 47, 102 };
    std::vector<int8_t> copy = data;
    std::sort(copy.begin(), copy.end());
    inplace_radix_sort(data.begin(), data.end());
    ASSERT_EQ(copy, data);
}

TEST(inplace_radix_sort, vector_bool)
{
    std::vector<bool> to_sort = { true, false, true, true, false, true, true, true, false, true, false, false };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, pair)
{
    std::vector<std::pair<int, bool>> to_sort =
    {
        { 5, true },
        { 5, false },
        { 6, false },
        { 7, true },
        { 4, false },
        { 4, true }
    };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, pair_other_direction)
{
    std::vector<std::pair<bool, int>> to_sort =
    {
        { true, 5 },
        { false, 5 },
        { false, 6 },
        { true, 7 },
        { false, 4 },
        { true, 4 }
    };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, reference)
{
    std::vector<int> to_sort = { 6, 5, 4, 3, 2, 1 };
    inplace_radix_sort(to_sort.begin(), to_sort.end(), [](int & i) -> int & { return i; });
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, pair_reference)
{
    std::vector<std::pair<int, bool>> to_sort = { { 5, true }, { 5, false }, { 6, false }, { 7, true }, { 4, false }, { 4, true } };
    inplace_radix_sort(to_sort.begin(), to_sort.end(), [](auto & i) -> decltype(auto) { return i; });
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, move_only)
{
    std::vector<std::unique_ptr<int>> to_sort;
    to_sort.push_back(std::make_unique<int>(5));
    to_sort.push_back(std::make_unique<int>(0));
    to_sort.push_back(std::make_unique<int>(1234567));
    to_sort.push_back(std::make_unique<int>(-1000));
    std::vector<int> sorted;
    sorted.reserve(to_sort.size());
    for (const std::unique_ptr<int> & i : to_sort)
        sorted.push_back(*i);
    std::sort(sorted.begin(), sorted.end());
    inplace_radix_sort(to_sort.begin(), to_sort.end(), [](auto & i){ return *i; });
    for (size_t i = 0; i < sorted.size(); ++i)
    {
        ASSERT_EQ(sorted[i], *to_sort[i]);
    }
}
#ifdef FULL_TESTS_SLOW_COMPILE_TIME
TEST(inplace_radix_sort, tuple)
{
    std::vector<std::tuple<bool, int, bool>> to_sort = { std::tuple<bool, int, bool>{ true, 5, true }, std::tuple<bool, int, bool>{ true, 5, false }, std::tuple<bool, int, bool>{ false, 6, false }, std::tuple<bool, int, bool>{ true, 7, true }, std::tuple<bool, int, bool>{ true, 4, false }, std::tuple<bool, int, bool>{ false, 4, true }, std::tuple<bool, int, bool>{ false, 5, false } };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
#endif

#ifdef FULL_TESTS_SLOW_COMPILE_TIME
TEST(inplace_radix_sort, nested_tuple)
{
    std::vector<std::tuple<bool, std::pair<int, std::pair<int, int>>, std::tuple<bool, bool, bool>>> to_sort;
    to_sort.emplace_back(true, std::make_pair(5, std::make_pair(6, 7)), std::make_tuple(true, false, true));
    to_sort.emplace_back(false, std::make_pair(5, std::make_pair(6, 7)), std::make_tuple(true, false, true));
    to_sort.emplace_back(false, std::make_pair(5, std::make_pair(6, 8)), std::make_tuple(true, false, true));
    to_sort.emplace_back(false, std::make_pair(5, std::make_pair(6, 6)), std::make_tuple(true, false, true));
    to_sort.emplace_back(false, std::make_pair(5, std::make_pair(7, 6)), std::make_tuple(true, false, true));
    to_sort.emplace_back(false, std::make_pair(5, std::make_pair(7, 6)), std::make_tuple(true, true, true));
    to_sort.emplace_back(false, std::make_pair(5, std::make_pair(7, 6)), std::make_tuple(true, true, false));
    to_sort.emplace_back(false, std::make_pair(5, std::make_pair(7, 6)), std::make_tuple(false, true, false));
    to_sort.emplace_back(false, std::make_pair(4, std::make_pair(7, 6)), std::make_tuple(false, true, false));
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
#endif

TEST(inplace_radix_sort, string)
{
    std::vector<std::string> to_sort =
    {
        "Hi",
        "There",
        "Hello",
        "World!",
        "Foo",
        "Bar",
        "Baz",
        "",
    };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}

TEST(inplace_radix_sort, vector)
{
    std::vector<std::vector<int>> to_sort =
    {
        { 1, 2, 3 },
        { 1, 2, 2 },
        { 1, 3, 2 },
        { 2, 3, 2 },
        { 2, 3, 2, 4 },
        { 2, 3, 2, 4, 5 },
        { 3, 2, 4, 5 },
        { 1 },
        {},
    };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}

#ifdef FULL_TESTS_SLOW_COMPILE_TIME
TEST(inplace_radix_sort, string_in_vector)
{
    std::vector<std::vector<std::string>> to_sort =
    {
        { "hi", "there", "you" },
        { "are", "probably", "not", "going" },
        { "to", "pass" },
        { "" },
        { },
        { "this", "test", "the", "first" },
        { "time" },
        { "oh it did pass", "n", "e", "a", "t!" },
        { "hi", "there", "I", "added", "more", "tests" },
        { "hi", "there", "needed", "the", "same", "prefix" },
    };
    std::vector<std::vector<std::string>> copy = to_sort;
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
    std::sort(copy.begin(), copy.end());
    ASSERT_EQ(copy, to_sort);
}
#endif

#ifdef FULL_TESTS_SLOW_COMPILE_TIME
TEST(inplace_radix_sort, vector_tuple_string)
{
    std::vector<std::tuple<std::string, std::string>> to_sort;
    to_sort.emplace_back("hi", "there");
    to_sort.emplace_back("you", "are");
    to_sort.emplace_back("probably", "not");
    to_sort.emplace_back("going", "to");
    to_sort.emplace_back("pass", "");
    to_sort.emplace_back("", "");
    to_sort.emplace_back("", "this");
    to_sort.emplace_back("test", "the");
    to_sort.emplace_back("first", "time");
    to_sort.emplace_back("oh it did pass", "n");
    to_sort.emplace_back("e", "a");
    to_sort.emplace_back("t!", "");
    to_sort.emplace_back("hi", "there");
    to_sort.emplace_back("I", "added");
    to_sort.emplace_back("more", "tests");
    to_sort.emplace_back("hi", "there");
    to_sort.emplace_back("needed", "the");
    to_sort.emplace_back("same", "prefix");
    std::vector<std::tuple<std::string, std::string>> copy = to_sort;
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
    std::sort(copy.begin(), copy.end());
    ASSERT_EQ(copy, to_sort);
}
#endif

TEST(inplace_radix_sort, std_array)
{
    std::vector<std::array<float, 4>> to_sort = { {{ 1.0f, 2.0f, 3.0f, 4.0f }}, {{ 0.0f, 3.0f, 4.0f, 5.0f }}, {{ 1.0f, 1.5f, 2.0f, 2.5f }}, {{ 1.0f, 2.0f, 2.5f, 4.0f }}, {{ 1.0f, 2.0f, 2.5f, 3.5f }}, {{ 0.0f, 3.0f, 4.5f, 4.5f }} };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
TEST(inplace_radix_sort, pointers)
{
    int array[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    std::vector<int * > to_sort =
    {
        array + 1,
        array + 3,
        array,
        array + 2,
        array + 7,
        array + 8,
        array + 6,
        array + 4,
        array + 5
    };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
/*#include <list>
TEST(inplace_radix_sort, vector_of_list)
{
    std::vector<std::list<int>> to_sort = { { 1, 2, 3 }, { 1, 2, 2 }, { 0, 1, 2 } };
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    //radix_sort(to_sort.begin(), to_sort.end(), to_sort.begin());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}*/

struct MovableInt
{
    MovableInt()
        : i()
    {
    }

    MovableInt(int i)
        : i(i)
    {
    }
    MovableInt(MovableInt &&) = default;
    MovableInt & operator=(MovableInt &&) = default;

    bool operator==(const MovableInt & other) const
    {
        return i == other.i;
    }
    bool operator<(const MovableInt & other) const
    {
        return i < other.i;
    }

    int i;
};
int to_radix_sort_key(const MovableInt & i)
{
    return i.i;
}

TEST(inplace_radix_sort, vector_of_movables)
{
    std::vector<std::vector<MovableInt>> to_sort;
    to_sort.emplace_back();
    to_sort.back().emplace_back(1); to_sort.back().emplace_back(2); to_sort.back().emplace_back(3);
    to_sort.emplace_back();
    to_sort.back().emplace_back(1); to_sort.back().emplace_back(2); to_sort.back().emplace_back(2);
    to_sort.emplace_back();
    to_sort.back().emplace_back(1); to_sort.back().emplace_back(1); to_sort.back().emplace_back(2);
    to_sort.emplace_back();
    to_sort.back().emplace_back(0); to_sort.back().emplace_back(1); to_sort.back().emplace_back(2);
    to_sort.emplace_back();
    to_sort.back().emplace_back(1); to_sort.back().emplace_back(2); to_sort.back().emplace_back(4);
    to_sort.emplace_back();
    to_sort.back().emplace_back(1); to_sort.back().emplace_back(3); to_sort.back().emplace_back(4);
    to_sort.emplace_back();
    to_sort.back().emplace_back(2); to_sort.back().emplace_back(3); to_sort.back().emplace_back(4);
    to_sort.emplace_back();
    to_sort.back().emplace_back(-2); to_sort.back().emplace_back(-3); to_sort.back().emplace_back(-4);
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}

TEST(radix_sort, movables)
{
    std::vector<MovableInt> to_sort;
    to_sort.emplace_back(1);
    to_sort.emplace_back(2);
    to_sort.emplace_back(0);
    to_sort.emplace_back(-1);
    to_sort.emplace_back(20);
    to_sort.emplace_back(-5);
    std::vector<MovableInt> buffer(to_sort.size());
    bool which_buffer = radix_sort(to_sort.begin(), to_sort.end(), buffer.begin());
    if (which_buffer)
        ASSERT_TRUE(std::is_sorted(buffer.begin(), buffer.end()));
    else
        ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}

#ifdef FULL_TESTS_SLOW_COMPILE_TIME
TEST(inplace_radix_sort, vector_of_vector_of_movables)
{
    std::vector<std::vector<std::vector<MovableInt>>> to_sort;
    to_sort.emplace_back();
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back(1); to_sort.back().back().emplace_back(2);
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back(2); to_sort.back().back().emplace_back(3);
    to_sort.emplace_back();
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back(1); to_sort.back().back().emplace_back(2);
    to_sort.emplace_back();
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back(1); to_sort.back().back().emplace_back(2);
    to_sort.back().back().emplace_back(1); to_sort.back().back().emplace_back(2);
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
#endif

#ifdef FULL_TESTS_SLOW_COMPILE_TIME
TEST(inplace_radix_sort, vector_vector_vector)
{
    std::vector<std::vector<std::vector<std::vector<MovableInt>>>> to_sort;
    to_sort.emplace_back();
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back();
    to_sort.back().back().back().emplace_back(1); to_sort.back().back().back().emplace_back(2);
    to_sort.back().back().emplace_back();
    to_sort.back().back().back().emplace_back(1); to_sort.back().back().back().emplace_back(3);
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back();
    to_sort.back().back().back().emplace_back(1); to_sort.back().back().back().emplace_back(3);
    to_sort.emplace_back();
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back();
    to_sort.emplace_back();
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back();
    to_sort.back().back().back().emplace_back(1); to_sort.back().back().back().emplace_back(3);
    to_sort.back().back().back().emplace_back(1); to_sort.back().back().back().emplace_back(3);
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back();
    to_sort.back().back().back().emplace_back(1); to_sort.back().back().back().emplace_back(2);
    to_sort.back().back().back().emplace_back(2); to_sort.back().back().back().emplace_back(2);
    to_sort.emplace_back();
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back();
    to_sort.back().back().back().emplace_back(1); to_sort.back().back().back().emplace_back(2);
    to_sort.emplace_back();
    to_sort.back().emplace_back();
    to_sort.back().back().emplace_back();
    inplace_radix_sort(to_sort.begin(), to_sort.end());
    to_sort.back().back().back().emplace_back(1); to_sort.back().back().back().emplace_back(2);
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
}
#endif

struct MovableString : private std::string
{
    using std::string::string;
    MovableString(const MovableString &) = delete;
    MovableString & operator=(const MovableString &) = delete;
    MovableString(MovableString &&) = default;
    MovableString & operator=(MovableString &&) = default;

    friend bool operator<(const MovableString & lhs, const MovableString & rhs)
    {
        return static_cast<const std::string &>(lhs) < static_cast<const std::string &>(rhs);
    }

    friend const std::string & to_radix_sort_key(const MovableString & str)
    {
        return static_cast<const std::string &>(str);
    }
};

struct Customer
{
    Customer(MovableString && first, MovableString && second)
        : first_name(std::move(first)), last_name(std::move(second))
    {
    }

    MovableString first_name;
    MovableString last_name;
};

TEST(inplace_radix_sort, no_copy)
{
    // sorting customers by last name then first name
    // I want to return references. I use a MovableString
    // to make it easier to get an error message when copying
    // happens
    std::vector<Customer> to_sort;
    to_sort.emplace_back("a", "b");
    to_sort.emplace_back("foo", "bar");
    to_sort.emplace_back("g", "a");
    to_sort.emplace_back("w", "d");
    to_sort.emplace_back("b", "c");
    std::string a;
    std::string b = a;
    a = b;
    inplace_radix_sort(to_sort.begin(), to_sort.end(), [](const Customer & customer)
    {
        return std::tie(customer.last_name, customer.first_name);
    });
    auto sort_by_last_name = [](const Customer & a, const Customer & b)
    {
        if (a.last_name < b.last_name)
            return true;
        else if (b.last_name < a.last_name)
            return false;
        else
            return a.first_name < b.first_name;
    };
    ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end(), sort_by_last_name));
}

#endif

// benchmarks
#if 0

#include "benchmark/benchmark.h"

#include <random>
#include <deque>

#if 0
//benchmark_inplace_sort/2M    103155817 ns  103115547 ns          7
//benchmark_inplace_sort/2M     73961470 ns   73923293 ns          9
//benchmark_inplace_sort/2M     69165349 ns   69165032 ns         10
//benchmark_ska_sort/2M        68850732 ns   68845594 ns         10
//benchmark_ska_sort/2M        67791104 ns   67794472 ns         10
static std::vector<int32_t> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<int32_t> result;
    result.reserve(size);
    std::uniform_int_distribution<int32_t> distribution;
    for (int i = 0; i < size; ++i)
    {
        result.push_back(distribution(randomness));
    }
    return result;
}

#elif 0

static std::vector<std::pair<bool, float>> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::pair<bool, float>> result;
    result.reserve(size);
    std::uniform_int_distribution<int> int_distribution(0, 1);
    std::uniform_real_distribution<float> real_distribution;
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int_distribution(randomness) != 0, real_distribution(randomness));
    }
    return result;
}
#elif 0
static std::deque<bool> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::deque<bool> result;
    std::uniform_int_distribution<int> int_distribution(0, 1);
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int_distribution(randomness) != 0);
    }
    return result;
}
#elif 0
static std::vector<std::int8_t> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::int8_t> result;
    result.reserve(size);
    std::uniform_int_distribution<std::int8_t> int_distribution(-128, 127);
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int_distribution(randomness));
    }
    return result;
}
#elif 0
static std::vector<std::int8_t> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::int8_t> result;
    result.reserve(size);
    std::uniform_int_distribution<std::int8_t> int_distribution(0, 1);
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int_distribution(randomness));
    }
    return result;
}
#elif 0
static std::vector<std::uint8_t> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::uint8_t> result;
    result.reserve(size);
    std::geometric_distribution<std::uint8_t> int_distribution(0.05);
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int_distribution(randomness));
    }
    return result;
}
#elif 0
// iterating over remaining buckets
//benchmark_ska_sort/2M       227948666 ns  227854867 ns          3
//benchmark_ska_sort/2M       226300240 ns  226282990 ns          3
// iterating over all buckets
//benchmark_ska_sort/2M       227441549 ns  227423700 ns          3
//benchmark_ska_sort/2M       226502577 ns  226484056 ns          3
static std::vector<int> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<int> result;
    result.reserve(size);
    std::geometric_distribution<int> int_distribution(0.001);
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int_distribution(randomness));
    }
    return result;
}
#elif 0
static std::vector<std::int16_t> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::int16_t> result;
    result.reserve(size);
    std::uniform_int_distribution<std::int16_t> int_distribution(-32768, 32767);
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int_distribution(randomness));
    }
    return result;
}
#elif 0
static std::vector<std::int64_t> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::int64_t> result;
    result.reserve(size);
    std::uniform_int_distribution<std::int64_t> int_distribution(std::numeric_limits<int64_t>::lowest(), std::numeric_limits<int64_t>::max());
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int_distribution(randomness));
    }
    return result;
}
#elif 0
static std::vector<std::tuple<std::int64_t, std::int64_t>> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::tuple<std::int64_t, std::int64_t>> result;
    result.reserve(size);
    std::uniform_int_distribution<std::int64_t> int_distribution(std::numeric_limits<int64_t>::lowest(), std::numeric_limits<int64_t>::max());
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int_distribution(randomness), int_distribution(randomness));
    }
    return result;
}
#elif 0
static std::vector<std::tuple<std::int32_t, std::int32_t, std::int64_t>> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::tuple<std::int32_t, std::int32_t, std::int64_t>> result;
    result.reserve(size);
    std::uniform_int_distribution<std::int32_t> int32_distribution(std::numeric_limits<int32_t>::lowest(), std::numeric_limits<int32_t>::max());
    std::uniform_int_distribution<std::int64_t> int64_distribution(std::numeric_limits<int64_t>::lowest(), std::numeric_limits<int64_t>::max());
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(int32_distribution(randomness), int32_distribution(randomness), int64_distribution(randomness));
    }
    return result;
}
#elif 0
static std::vector<std::vector<int>> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::vector<int>> result;
    result.reserve(size);
    std::uniform_int_distribution<int> size_distribution(0, 20);
    std::uniform_int_distribution<int> value_distribution;
    for (int i = 0; i < size; ++i)
    {
        std::vector<int> to_add(size_distribution(randomness));
        std::generate(to_add.begin(), to_add.end(), [&]{ return value_distribution(randomness); });
        result.push_back(std::move(to_add));
    }
    return result;
}
#elif 0
static std::vector<std::vector<std::string>> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::vector<std::string>> result;
    result.reserve(size);
    std::uniform_int_distribution<int> size_distribution(0, 10);
    std::uniform_int_distribution<int> string_length_distribution(0, 5);
    std::uniform_int_distribution<char> string_content_distribution('a', 'c');
    for (int i = 0; i < size; ++i)
    {
        std::vector<std::string> to_add(size_distribution(randomness));
        std::generate(to_add.begin(), to_add.end(), [&]
        {
#if 0
            std::string new_string = "hello";
            for (int i = 0, end = string_length_distribution(randomness); i != end; ++i)
                new_string.push_back('\0');
            std::generate(new_string.begin() + 5, new_string.end(), [&]
            {
                return string_content_distribution(randomness);
            });
#else
            std::string new_string(string_length_distribution(randomness), '\0');
            std::generate(new_string.begin(), new_string.end(), [&]
            {
                return string_content_distribution(randomness);
            });
#endif
            return new_string;
        });
        result.push_back(std::move(to_add));
    }
    return result;
}

#elif 0

static std::vector<std::string> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::string> result;
    result.reserve(size);
    std::uniform_int_distribution<int> string_length_distribution(0, 20);
    std::uniform_int_distribution<char> string_content_distribution('a', 'z');
    for (int i = 0; i < size; ++i)
    {
        std::string to_add(string_length_distribution(randomness), '\0');
        std::generate(to_add.begin(), to_add.end(), [&]
        {
            return string_content_distribution(randomness);
        });
        result.push_back(std::move(to_add));
    }
    return result;
}

#elif 1

extern const std::vector<const char *> & get_word_list();
static std::vector<std::string> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    const std::vector<const char *> & words = get_word_list();
    std::vector<std::string> result;
    result.reserve(size);
    std::uniform_int_distribution<int> string_length_distribution(0, 10);
    //std::uniform_int_distribution<int> string_length_distribution(1, 3);
    std::uniform_int_distribution<size_t> word_picker(0, words.size() - 1);
    for (int i = 0; i < size; ++i)
    {
        std::string to_add;
        for (int i = 0, end = string_length_distribution(randomness); i < end; ++i)
        {
            to_add += words[word_picker(randomness)];
        }
        result.push_back(std::move(to_add));
    }
    return result;
}
#elif 1
static std::vector<std::vector<int>> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::vector<int>> result;
    std::uniform_int_distribution<int> random_size(0, 128);
    result.reserve(size);
    for (int i = 0; i < size; ++i)
    {
        std::vector<int> to_add(random_size(randomness));
        std::iota(to_add.begin(), to_add.end(), 0);
        result.push_back(std::move(to_add));
    }
    return result;
}
#elif 1
template<size_t Size>
struct SizedStruct
{
    uint8_t array[Size] = {};
};
template<>
struct SizedStruct<0>
{
};
#define SORT_ON_FIRST_ONLY
typedef std::int64_t benchmark_sort_key;
#define NUM_SORT_KEYS 1
typedef SizedStruct<1016> benchmark_sort_value;
#if NUM_SORT_KEYS == 1
static std::vector<std::tuple<benchmark_sort_key, benchmark_sort_value>> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::tuple<benchmark_sort_key, benchmark_sort_value>> result;
    result.reserve(size);
    std::uniform_int_distribution<benchmark_sort_key> distribution(std::numeric_limits<benchmark_sort_key>::lowest(), std::numeric_limits<benchmark_sort_key>::max());
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(distribution(randomness), benchmark_sort_value());
    }
    return result;
}
#elif NUM_SORT_KEYS == 2
static std::vector<std::tuple<std::pair<benchmark_sort_key, benchmark_sort_key>, benchmark_sort_value>> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::tuple<std::pair<benchmark_sort_key, benchmark_sort_key>, benchmark_sort_value>> result;
    result.reserve(size);
    std::uniform_int_distribution<benchmark_sort_key> distribution(std::numeric_limits<benchmark_sort_key>::lowest(), std::numeric_limits<benchmark_sort_key>::max());
    for (int i = 0; i < size; ++i)
    {
        result.emplace_back(std::make_pair(distribution(randomness), distribution(randomness)), benchmark_sort_value());
    }
    return result;
}
#else
static std::vector<std::tuple<std::array<benchmark_sort_key, NUM_SORT_KEYS>, benchmark_sort_value>> SKA_SORT_NOINLINE create_radix_sort_data(std::mt19937_64 & randomness, int size)
{
    std::vector<std::tuple<std::array<benchmark_sort_key, NUM_SORT_KEYS>, benchmark_sort_value>> result;
    result.reserve(size);
    std::uniform_int_distribution<benchmark_sort_key> distribution(std::numeric_limits<benchmark_sort_key>::lowest(), std::numeric_limits<benchmark_sort_key>::max());
    for (int i = 0; i < size; ++i)
    {
        std::array<benchmark_sort_key, NUM_SORT_KEYS> key;
        for (int i = 0; i < NUM_SORT_KEYS; ++i)
            key[i] = distribution(randomness);
        result.emplace_back(key, benchmark_sort_value());
    }
    return result;
}
#endif
#endif

#if 1
static constexpr int profile_multiplier = 2;
//static constexpr int max_profile_range = 1 << 24;
static constexpr int max_profile_range = 1 << 20;

#if 1
#if 0
static void benchmark_radix_sort(benchmark::State & state)
{
    std::mt19937_64 randomness(77342348);
    auto buffer = create_radix_sort_data(randomness, state.range(0));
    while (state.KeepRunning())
    {
        auto to_sort = create_radix_sort_data(randomness, state.range(0));
        benchmark::DoNotOptimize(to_sort.data());
        benchmark::DoNotOptimize(buffer.data());
#ifdef SORT_ON_FIRST_ONLY
        radix_sort(to_sort.begin(), to_sort.end(), buffer.begin(), [](auto && a){ return std::get<0>(a); });
#else
        bool which = radix_sort(to_sort.begin(), to_sort.end(), buffer.begin());
        if (which)
            assert(std::is_sorted(buffer.begin(), buffer.end()));
        else
            assert(std::is_sorted(to_sort.begin(), to_sort.end()));
#endif
        benchmark::ClobberMemory();
    }
}
BENCHMARK(benchmark_radix_sort)->RangeMultiplier(profile_multiplier)->Range(profile_multiplier, max_profile_range);
#endif

#if 0
static void benchmark_ska_sort_copy(benchmark::State & state)
{
    std::mt19937_64 randomness(77342348);
    auto buffer = create_radix_sort_data(randomness, state.range(0));
    while (state.KeepRunning())
    {
        auto to_sort = create_radix_sort_data(randomness, state.range(0));
#ifdef SORT_ON_FIRST_ONLY
        ska_sort_copy(to_sort.begin(), to_sort.end(), buffer.begin(), [](auto && a) -> decltype(auto){ return std::get<0>(a); });
#else
        bool which = ska_sort_copy(to_sort.begin(), to_sort.end(), buffer.begin());
        if (which)
            assert(std::is_sorted(buffer.begin(), buffer.end()));
        else
            assert(std::is_sorted(to_sort.begin(), to_sort.end()));
#endif
        benchmark::DoNotOptimize(to_sort.data());
        benchmark::DoNotOptimize(buffer.data());
    }
}
BENCHMARK(benchmark_ska_sort_copy)->RangeMultiplier(profile_multiplier)->Range(profile_multiplier, max_profile_range);
#endif

#if 1

static void benchmark_std_sort(benchmark::State & state)
{
    std::mt19937_64 randomness(77342348);
    create_radix_sort_data(randomness, state.range(0));
    while (state.KeepRunning())
    {
        auto to_sort = create_radix_sort_data(randomness, state.range(0));
        benchmark::DoNotOptimize(to_sort.data());
#ifdef SORT_ON_FIRST_ONLY
        std::sort(to_sort.begin(), to_sort.end(), [](auto && l, auto && r){ return std::get<0>(l) < std::get<0>(r); });
#else
        std::sort(to_sort.begin(), to_sort.end());
        assert(std::is_sorted(to_sort.begin(), to_sort.end()));
#endif
        benchmark::ClobberMemory();
    }
}
BENCHMARK(benchmark_std_sort)->RangeMultiplier(profile_multiplier)->Range(profile_multiplier, max_profile_range);

#endif

#if 0
static void benchmark_american_flag_sort(benchmark::State & state)
{
    std::mt19937_64 randomness(77342348);
    create_radix_sort_data(randomness, state.range(0));
    while (state.KeepRunning())
    {
        auto to_sort = create_radix_sort_data(randomness, state.range(0));
        benchmark::DoNotOptimize(to_sort.data());
#ifdef SORT_ON_FIRST_ONLY
        american_flag_sort(to_sort.begin(), to_sort.end(), [](auto && a) -> decltype(auto){ return std::get<0>(a); });
#else
        american_flag_sort(to_sort.begin(), to_sort.end());
        assert(std::is_sorted(to_sort.begin(), to_sort.end()));
#endif
        benchmark::ClobberMemory();
    }
}
BENCHMARK(benchmark_american_flag_sort)->RangeMultiplier(profile_multiplier)->Range(profile_multiplier, max_profile_range);
#endif
#endif

static void benchmark_ska_sort(benchmark::State & state)
{
    std::mt19937_64 randomness(77342348);
    create_radix_sort_data(randomness, state.range(0));
    while (state.KeepRunning())
    {
        auto to_sort = create_radix_sort_data(randomness, state.range(0));
        benchmark::DoNotOptimize(to_sort.data());
#ifdef SORT_ON_FIRST_ONLY
        ska_sort(to_sort.begin(), to_sort.end(), [](auto && a) -> decltype(auto){ return std::get<0>(a); });
#else
        ska_sort(to_sort.begin(), to_sort.end());
        assert(std::is_sorted(to_sort.begin(), to_sort.end()));
#endif
        benchmark::ClobberMemory();
    }
}
BENCHMARK(benchmark_ska_sort)->RangeMultiplier(profile_multiplier)->Range(profile_multiplier, max_profile_range);

#if 0

static void benchmark_inplace_radix_sort(benchmark::State & state)
{
    std::mt19937_64 randomness(77342348);
    create_radix_sort_data(randomness, state.range(0));
    while (state.KeepRunning())
    {
        auto to_sort = create_radix_sort_data(randomness, state.range(0));
        benchmark::DoNotOptimize(to_sort.data());
#ifdef SORT_ON_FIRST_ONLY
        inplace_radix_sort(to_sort.begin(), to_sort.end(), [](auto && a) -> decltype(auto){ return std::get<0>(a); });
#else
        inplace_radix_sort(to_sort.begin(), to_sort.end());
        assert(std::is_sorted(to_sort.begin(), to_sort.end()));
#endif
        benchmark::ClobberMemory();
    }
}
BENCHMARK(benchmark_inplace_radix_sort)->RangeMultiplier(profile_multiplier)->Range(profile_multiplier, max_profile_range);

#endif

static void benchmark_generation(benchmark::State & state)
{
    std::mt19937_64 randomness(77342348);
    create_radix_sort_data(randomness, state.range(0));
    while (state.KeepRunning())
    {
        auto to_sort = create_radix_sort_data(randomness, state.range(0));
        benchmark::DoNotOptimize(to_sort.data());
        benchmark::ClobberMemory();
    }
}
BENCHMARK(benchmark_generation)->RangeMultiplier(profile_multiplier)->Range(profile_multiplier, max_profile_range);

#endif


#if 0
static std::vector<std::int8_t> SKA_SORT_NOINLINE create_limited_radix_sort_data(std::mt19937_64 & randomness, int8_t range_end)
{
    int8_t permutation[256];
    std::iota(permutation, permutation + 256, -128);
    std::shuffle(permutation, permutation + 256, randomness);
    std::vector<std::int8_t> result;
    size_t size = 2 * 1024 * 1024;
    result.reserve(size);
    std::uniform_int_distribution<std::int8_t> int_distribution(-128, range_end);
    for (size_t i = 0; i < size; ++i)
    {
        result.emplace_back(permutation[detail::to_unsigned_or_bool(int_distribution(randomness))]);
    }
    return result;
}
static void benchmark_limited_generation(benchmark::State & state)
{
    std::mt19937_64 randomness(77342348);
    while (state.KeepRunning())
    {
        auto to_sort = create_limited_radix_sort_data(randomness, state.range(0));
        benchmark::DoNotOptimize(to_sort.data());
        benchmark::ClobberMemory();
    }
}

#define LIMITED_RANGE() Arg(-128)->Arg(-127)->Arg(-120)->Arg(-96)->Arg(-64)->Arg(-32)->Arg(0)->Arg(32)->Arg(64)->Arg(96)->Arg(127)
BENCHMARK(benchmark_limited_generation)->LIMITED_RANGE();

static void benchmark_limited_inplace_sort(benchmark::State & state)
{
    std::mt19937_64 randomness(77342348);
    while (state.KeepRunning())
    {
        auto to_sort = create_limited_radix_sort_data(randomness, state.range(0));
        benchmark::DoNotOptimize(to_sort.data());
        ska_sort(to_sort.begin(), to_sort.end());
        assert(std::is_sorted(to_sort.begin(), to_sort.end()));
        benchmark::ClobberMemory();
    }
}
BENCHMARK(benchmark_limited_inplace_sort)->LIMITED_RANGE();
#endif

#if 0
static constexpr int sort_amount = 1024 * 1024;
TEST(benchmark, american_flag_sort)
{
    std::mt19937_64 randomness(77342348);
    for (int i = 0; i < 100; ++i)
    {
        auto to_sort = create_radix_sort_data(randomness, sort_amount);
        american_flag_sort(to_sort.begin(), to_sort.end());
        ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
    }
}
TEST(benchmark, inplace_faster)
{
    std::mt19937_64 randomness(77342348);
    for (int i = 0; i < 100; ++i)
    {
        auto to_sort = create_radix_sort_data(randomness, sort_amount);
        ska_sort(to_sort.begin(), to_sort.end());
        ASSERT_TRUE(std::is_sorted(to_sort.begin(), to_sort.end()));
    }
}
#endif

#endif
