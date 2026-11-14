#include "RandomNumberSource.h"

#include <stdexcept>

namespace {

std::uniform_int_distribution<int> makeNaturalDistribution(int min, int max) {
    if (min < 1) {
        throw std::invalid_argument("нижня межа проміжку має бути натуральним числом");
    }
    if (min > max) {
        throw std::invalid_argument("нижня межа проміжку більша за верхню");
    }
    return std::uniform_int_distribution<int>(min, max);
}

}  // namespace

RandomNumberSource::RandomNumberSource(std::uint32_t seed, int min, int max)
    : engine_(seed), distribution_(makeNaturalDistribution(min, max)) {}

int RandomNumberSource::next() {
    return distribution_(engine_);
}
