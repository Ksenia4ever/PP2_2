#pragma once

#include <algorithm>
#include <iterator>
#include <thread>
#include <vector>
#include <exception>


class ParallelAllOf
{
public:

    void SetK(unsigned int k) { if (k == 0) { throw std::invalid_argument("K>0"); } this->k = k;}
    unsigned int GetK() const { return k; }

    template<typename It, typename Pred>
    bool AllOf(It begin, It end, Pred p)
    {
        auto res = false;

        const auto size = std::distance(begin, end);
        const auto chunkSize = size / GetK();
        if (chunkSize == 0)
        {
            res = std::all_of(begin, end, p);
        }
        else
        {
            auto results = std::vector<bool>(GetK(), false);
            auto threads = std::vector<std::thread>();

            for (auto chunkIndex = 0U; chunkIndex < GetK(); chunkIndex++)
            {
                auto from = begin + (chunkIndex * chunkSize);
                auto to = begin + ((chunkIndex + 1) * chunkSize);
                if (chunkIndex == GetK() - 1)
                {
                    to = end;
                }

                auto th = std::thread([&results, &p, from, to, chunkIndex]()
                {
                    results[chunkIndex] = std::all_of(from, to, p);
                });
                
                threads.push_back(std::move(th));
            }

            for (auto& th : threads)
            {
                th.join();
            }

            res = std::all_of(results.begin(), results.end(), [](const auto& r) {return r; });
        }

        return res;
    }

private:

    unsigned int k = 1;
};