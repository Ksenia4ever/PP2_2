#pragma once

#include <functional>
#include <chrono>


class PerformanceBooleanTester
{
public:
    using TAlg = std::function<bool()>;
    using TMcs = unsigned long int;

    void SetCount(int c) { count = c; }
    int GetCount() const { return count; }

    bool Measure(const TAlg& alg);

    TMcs GetMeasureDuration() const { return measureRes; }

private:

    int count = 5;
    TMcs measureRes = 0;
};

