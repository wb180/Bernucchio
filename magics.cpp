#include "bits_functions.h"
#include "magics.hpp"

#include <algorithm>
#include <random>
#include <unordered_map>

uint64_t GenerateMagics(const std::vector<uint64_t> &numbers)
{
    std::mt19937_64 rng;
    rng.seed(std::random_device{}());

    uint64_t magic = 0;
    std::unordered_map<uint64_t, std::size_t> indexes;
    bool is_unique;

    do
    {
        is_unique = true;
        indexes.clear();

        magic = rng() & rng() & rng();

        for(std::size_t i = 0; i < numbers.size(); ++i)
        {
            uint64_t result = (numbers.at(i) * magic) >> (64 - GetMSBPos(numbers.size()));

            if(indexes.count(result))
            {
                is_unique = false;
                magic = 0;
                break;
            }
            else
                indexes[result] = 1;
        }
    }
    while(!is_unique);

    return magic;
}
