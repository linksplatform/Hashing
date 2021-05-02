#include <Platform.Hashing.h>
#include <benchmark/benchmark.h>

#include <chrono>
#include <random>

namespace Platform::Hashing::Benchmarks::Any
{
    namespace
    {
        constexpr int test_size = 10000;
        std::mt19937 random(std::chrono::high_resolution_clock::now().time_since_epoch().count());

        std::any random_any()
        {
            static std::any variadic[]
            {
                (short int){},
                (unsigned short int){},
                (int){},
                (unsigned int){},
                (unsigned long int){},
                (long long int){},
                (unsigned long long int){},
                (float){},
                (double){},
                (long double){},
                (std::string){},
            };
            int position = random() % std::size(variadic);

            return variadic[position];
        }
    }

    static void Any_BasicSpeed(benchmark::State& state)
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

    BENCHMARK(Any_BasicSpeed);

    static void Any_SpeedOnList(benchmark::State& state)
    {
        std::vector<std::any> list(test_size);
        for (auto& it : list)
        {
            it = 0;
        }

        std::size_t result;
        for (auto _ : state)
        {
            benchmark::DoNotOptimize(result = Hash(list));
        }
    }

    BENCHMARK(Any_SpeedOnList);

    static void Any_RandomSpeedOnList(benchmark::State& state)
    {
        std::vector<std::any> list(test_size);
        for (auto& it : list)
        {
            it = random_any();
        }

        std::size_t result;
        for (auto _ : state)
        {
            benchmark::DoNotOptimize(result = Hash(list));
        }
    }

    BENCHMARK(Any_RandomSpeedOnList);
}
