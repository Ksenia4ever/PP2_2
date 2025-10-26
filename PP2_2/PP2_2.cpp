// Visual Studio 2022
#include <iostream>
#include <string>
#include <algorithm>
#include <execution>
#include <format>

#include "SourceDataGenerator.h"
#include "PerformanceBooleanTester.h"
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

template<typename Pred>
void Test(Pred p, const SourceDataGenerator::TValues& data, const std::string& header)
{
    auto tester = PerformanceBooleanTester();

    std::cout << "\t" << header << std::endl;

    const auto res = tester.Measure([&]() { return p(data); });
    const auto dur = tester.GetMeasureDuration();
    std::cout << "\tDuration (mcs) " << dur << std::endl;
    std::cout << "\tResult         " << (res ? "True" : "False") << std::endl;
    std::cout << std::endl;
}

template<typename Pred>
void TestK(Pred p, const SourceDataGenerator::TValues& data, unsigned int k, const std::string& header)
{
    auto tester = PerformanceBooleanTester();
    auto measures = std::vector<PerformanceBooleanTester::TMcs>(k, 0);

    std::cout << "\t" << header << std::endl;
    std::cout << "\t" << "K   Duration (mcs)  Result" << std::endl;
    for (unsigned int numK = 1; numK <= k; numK++)
    {
        const auto res = tester.Measure([&]() { return p(data, numK); });
        const auto dur = tester.GetMeasureDuration();
        std::cout << "\t" << std::format("{:<5}", numK) << std::format("{:<16}", dur) << (res ? "True" : "False") << std::endl;

        measures[numK - 1] = dur;
    }

    const auto it = std::min_element(measures.begin(), measures.end());
    const auto minK = std::distance(measures.begin(), it) + 1;
    std::cout << "\tBest K = " << minK << " Duration: " << *it << std::endl;

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

        Test(AllOf, rndSequence, "all_of without policy (random data).");
        Test(AllOf, prfSequence, "std::all_of without policy.");
        Test(AllOfSequential, prfSequence, "std::all_of sequential policy.");
        Test(AllOfParallel, prfSequence, "std::all_of with parallel policy.");
        Test(AllOfUnseq, prfSequence, "std::all_of with vectorized policy.");
        Test(AllOfParallelUnseq, prfSequence, "std::all_of with parallel-vectorized policy.");

        TestK(PAllOf, prfSequence, 16, "ParallelAllOf.");
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