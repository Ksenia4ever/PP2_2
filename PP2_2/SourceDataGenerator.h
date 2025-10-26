#pragma once

#include <vector>

class SourceDataGenerator
{
public:

    using TValue = int;
    using TValues = std::vector<TValue>;

    void SetCount(size_t c) { count = c; }
    size_t GetCount() const { return count; }

    TValues GenerateRandomSequence() const;
    TValues GeneratePerformanceSequence() const;

private:

    size_t count = 1;
};

