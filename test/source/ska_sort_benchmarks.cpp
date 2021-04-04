//          Copyright Malte Skarupke 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See http://www.boost.org/LICENSE_1_0.txt)

#include <ska/sort.hpp>

#define SKA_SORT_NOINLINE __attribute__((noinline))

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
