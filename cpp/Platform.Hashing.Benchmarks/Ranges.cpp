#include <Platform.Hashing.h>
#include <benchmark/benchmark.h>

#include <chrono>
#include <random>
#include <ranges>
#include <forward_list>
#include <iostream>
#include <list>

struct wrapper { std::size_t value; };
template<>
struct std::hash<wrapper>
{
    std::size_t operator()(const wrapper& self) const
    {
        return self.value;
    }
};

namespace Platform::Hashing::Benchmarks::Ranges
{
    constexpr int test_size = 1000000;

    template<typename T>
    auto gen_random_list(std::size_t size)
    {
        static std::mt19937_64 random(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::vector<T> list(test_size);
        for (auto& it : list)
        {
            it = T(random());
        }

        return list;
    }

    void Ranges_BasicSpeed(benchmark::State& state)
    {
        auto list = gen_random_list<std::size_t>(test_size);

        for (auto _ : state)
        {
            std::size_t result = Hash(list);
            benchmark::DoNotOptimize(result);
        }
    }
    BENCHMARK(Ranges_BasicSpeed);


    void Ranges_PerfomanceWith_Data(benchmark::State& state)
    {
        auto list = gen_random_list<std::size_t>(test_size);

        for (auto _ : state)
        {
            std::size_t result = Hash(list);
            benchmark::DoNotOptimize(result);
        }
    }
    BENCHMARK(Ranges_PerfomanceWith_Data);

    void Ranges_PerfomanceWithout_NonIntegral_POD(benchmark::State& state)
    {
        struct local_wrapper { std::size_t value; };
        auto list = gen_random_list<local_wrapper>(test_size);

        for (auto _ : state)
        {
            std::size_t result = Hash(list);
            result = result;
            benchmark::DoNotOptimize(result);
        }
    }
    BENCHMARK(Ranges_PerfomanceWithout_NonIntegral_POD);

    void Ranges_PerfomanceWith_Data_NonIntegral(benchmark::State& state)
    {
        auto list = gen_random_list<wrapper>(test_size);

        for (auto _ : state)
        {
            std::size_t result = Hash(list);
            result = result;
            benchmark::DoNotOptimize(result);
        }
    }
    BENCHMARK(Ranges_PerfomanceWith_Data_NonIntegral);

    void Ranges_PerfomanceWithout(benchmark::State& state)
    {
        auto temp = gen_random_list<std::size_t>(test_size);
        auto list = std::forward_list(temp.begin(), temp.end());

        for (auto _ : state)
        {
            std::size_t result = Hash(list);
            result = result;
            benchmark::DoNotOptimize(result);
        }
    }
    BENCHMARK(Ranges_PerfomanceWithout);
}
