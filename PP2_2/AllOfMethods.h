#pragma once

#include <algorithm>
#include <execution>

#include "SourceDataGenerator.h"
#include "ParallelAllOf.h"

bool IsEven(SourceDataGenerator::TValue value)
{
    return value % 2 == 0;
}

bool AllOf(const SourceDataGenerator::TValues& data)
{
    return std::all_of(data.begin(), data.end(), IsEven);
}

bool AllOfSequential(const SourceDataGenerator::TValues& data)
{
    return std::all_of(std::execution::seq, data.begin(), data.end(), IsEven);
}

bool AllOfParallel(const SourceDataGenerator::TValues& data)
{
    return std::all_of(std::execution::par, data.begin(), data.end(), IsEven);
}

bool AllOfUnseq(const SourceDataGenerator::TValues& data)
{
    return std::all_of(std::execution::unseq, data.begin(), data.end(), IsEven);
}

bool AllOfParallelUnseq(const SourceDataGenerator::TValues& data)
{
    return std::all_of(std::execution::par_unseq, data.begin(), data.end(), IsEven);
}

bool PAllOf(const SourceDataGenerator::TValues& data, unsigned int k)
{
    ParallelAllOf allOf;
    allOf.SetK(k);
    return allOf.AllOf(data.begin(), data.end(), IsEven);
}