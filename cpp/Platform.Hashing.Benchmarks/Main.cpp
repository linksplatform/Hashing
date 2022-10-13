#include <Platform.Hashing.h>
#include <benchmark/benchmark.h>

#include "Any.cpp"
#include "Ranges.cpp"

#include <chrono>

// BENCHMARK_MAIN();

namespace chrono = std::chrono;
using namespace std::string_view_literals;

using Platform::Hashing::hash_span;

int main() {
    auto start = chrono::steady_clock::now();

    auto text = R"(Bei Nacht im Dorf der Wächter rief: Elfe!
Ein ganz kleines Elfchen im Walde schlief
wohl um die Elfe!
Und meint, es rief ihm aus dem Tal
bei seinem Namen die Nachtigall,
oder Silpelit hätt' ihm gerufen.
Reibt sich der Elf' die Augen aus,
begibt sich vor sein Schneckenhaus
und ist als wie ein trunken Mann,
sein Schläflein war nicht voll getan,
und humpelt also tippe tapp
durch’s Haselholz in’s Tal hinab,
schlupft an der Mauer hin so dicht,
da sitzt der Glühwurm Licht an Licht.
Was sind das helle Fensterlein?
Da drin wird eine Hochzeit sein:
die Kleinen sitzen bei’m Mahle,
und treiben’s in dem Saale.
Da guck' ich wohl ein wenig 'nein!"
Pfui, stößt den Kopf an harten Stein!
Elfe, gelt, du hast genug?
Gukuk!)"sv;

    for (size_t i = 0; i < 1'000'000; i++) {
        benchmark::DoNotOptimize(hash_span(std::span(text)));
    }

    auto end = chrono::steady_clock::now();
    std::cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
}