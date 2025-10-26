// Visual Studio 2022
#include <iostream>
#include <string>
#include <algorithm>
#include <execution>
#include <format>

#include "SourceDataGenerator.h"
#include "PerformanceBooleanTester.h"
#include "ParallelAllOf.h"

bool AllOf(const SourceDataGenerator::TValues& data)
{
    return std::all_of(data.begin(), data.end(), [](auto val) {return val % 2 == 0; });
}

bool AllOfSequential(const SourceDataGenerator::TValues& data)
{
    return std::all_of(std::execution::seq, data.begin(), data.end(), [](auto val) { return val % 2 == 0; });
}

bool AllOfParallel(const SourceDataGenerator::TValues& data)
{
    return std::all_of(std::execution::par, data.begin(), data.end(), [](auto val) { return val % 2 == 0; });
}

bool AllOfUnseq(const SourceDataGenerator::TValues& data)
{
    return std::all_of(std::execution::unseq, data.begin(), data.end(), [](auto val) { return val % 2 == 0; });
}

bool AllOfParallelUnseq(const SourceDataGenerator::TValues& data)
{
    return std::all_of(std::execution::par_unseq, data.begin(), data.end(), [](auto val) { return val % 2 == 0; });
}

bool PAllOf(const SourceDataGenerator::TValues& data, unsigned int k)
{
    ParallelAllOf allOf;
    allOf.SetK(k);
    return allOf.AllOf(data.begin(), data.end(), [](auto val) { return val % 2 == 0; });
}

void Test(PerformanceBooleanTester& tester, const PerformanceBooleanTester::TAlg& alg, const std::string& header)
{
    std::cout << "\t" << header << std::endl;
    const auto res = tester.Measure(alg);
    const auto dur = tester.GetMeasureDuration();
    std::cout << "\tResult: " << (res ? "True" : "False") << std::endl;
    std::cout << "\tDuration: " << dur << " mcs." << std::endl;
    std::cout << std::endl;
}

void RunProgram()
{
    auto gen = SourceDataGenerator();
    for (int i = 0; i < 4; i++)
    {
        gen.SetCount(10000000*(i+1));
        std::cout << "Source data lenght: " << gen.GetCount() << std::endl << std::endl;

        const auto rndSequence = gen.GenerateRandomSequence();
        const auto prfSequence = gen.GeneratePerformanceSequence();

        auto tester = PerformanceBooleanTester();
        tester.SetCount(10);

        Test(tester, [&]() {return AllOf(rndSequence); }, "Random data. all_of without policy.");
        Test(tester, [&]() {return AllOf(prfSequence); }, "Performance data. all_of without policy.");
        Test(tester, [&]() {return AllOfSequential(prfSequence); }, "Performance data. all_of sequential policy.");
        Test(tester, [&]() {return AllOfParallel(prfSequence); }, "Performance data. all_of with parallel policy.");
        Test(tester, [&]() {return AllOfUnseq(prfSequence); }, "Performance data. all_of with vectorized policy.");
        Test(tester, [&]() {return AllOfParallelUnseq(prfSequence); }, "Performance data. all_of with parallel-vectorized policy.");

        for (int k = 1; k <= 16; k++)
        {
            Test(tester, [&]() {return PAllOf(prfSequence, k); }, std::format("Performance data. ParallelAllOf. K={}", k));
        }
    }
}

int main(int argc, char* argv[])
{
    try
    {
        RunProgram();
    }
    catch (std::exception ex)
    {
        std::cout << std::endl;
        std::cout << "Error:" << ex.what();
    }
}