#include <Platform.Hashing.h>
#include <benchmark/benchmark.h>

#include <chrono>
#include <random>

namespace Platform::Hashing::Benchmarks::Range
{
    namespace
    {
        constexpr int test_size = 1000000;
        std::mt19937 random(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    }

    static void BasicSpeed(benchmark::State& state)
    {
        std::vector<int> list(test_size);
        for (auto& it : list)
        {
            it = random();
        }

        std::size_t result;
        for (auto _ : state)
        {
            benchmark::DoNotOptimize(result = Hash(list));
        }
    }

    BENCHMARK(BasicSpeed);
}
