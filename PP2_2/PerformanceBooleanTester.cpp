#include "PerformanceBooleanTester.h"

bool PerformanceBooleanTester::Measure(const TAlg& alg)
{
    const auto clock = std::chrono::high_resolution_clock();
    auto durations = std::vector<TMcs>(GetCount(), 0);
    auto res = false;

    for (int i = 0; i < GetCount(); i++)
    {
        const auto start = clock.now();

        res = alg();

        const auto end = clock.now();
        const auto duration = end - start;

        durations[i] = static_cast<TMcs>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count());
    }

    auto avgDuration = TMcs(0);
    for (auto dur : durations)
    {
        avgDuration += dur;
    }
    avgDuration /= count;
    measureRes = avgDuration;

    return res;
}