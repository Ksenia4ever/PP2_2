#include "SourceDataGenerator.h"

#include <random>

SourceDataGenerator::TValues SourceDataGenerator::GenerateRandomSequence() const
{
    auto gen = std::mt19937(std::random_device{}());
    auto dist = std::uniform_int_distribution<TValue>(1, 100);

    auto values = TValues(GetCount(), 0);
    for (auto& val : values)
    {
        val = dist(gen);
    }

    return values;
}

SourceDataGenerator::TValues SourceDataGenerator::GeneratePerformanceSequence() const
{
    auto gen = std::mt19937(std::random_device{}());
    auto dist = std::uniform_int_distribution<TValue>(1, 50);

    auto values = TValues(GetCount(), 0);
    for (auto& val : values)
    {
        val = dist(gen) * 2;
    }

    return values;
}