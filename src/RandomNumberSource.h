#pragma once

#include <cstdint>
#include <random>

// Джерело випадкових натуральних чисел, рівномірно розподілених на [min, max].
class RandomNumberSource {
public:
    RandomNumberSource(std::uint32_t seed, int min, int max);

    int next();

private:
    std::mt19937 engine_;
    std::uniform_int_distribution<int> distribution_;
};
