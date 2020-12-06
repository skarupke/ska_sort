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
