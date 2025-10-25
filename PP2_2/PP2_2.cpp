// Visual Studio 2022
#include <iostream>
#include <string>
#include <algorithm>

#include "SourceDataGenerator.h"
#include "PerformanceBooleanTester.h"


bool AllOfSequential(const SourceDataGenerator::TValues& data)
{
    return std::all_of(data.begin(), data.end(), [](auto val) {return val % 2 == 0; });
}

void RunProgram()
{
    const auto gen = SourceDataGenerator();
    const auto rndSequence = gen.GenerateRandomSequence();
    const auto prfSequence = gen.GeneratePerformanceSequence();

    auto tester = PerformanceBooleanTester();
    tester.SetCount(5);

    const auto rndRes = tester.Measure([&]() {return AllOfSequential(rndSequence); });
    const auto rndDur = tester.GetMeasureDuration();

    const auto prfRes = tester.Measure([&]() {return AllOfSequential(prfSequence); });
    const auto prfDur = tester.GetMeasureDuration();
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