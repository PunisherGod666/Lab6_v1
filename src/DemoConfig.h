#pragma once

#include <cstddef>
#include <cstdint>
#include <istream>
#include <stdexcept>

namespace demo {

inline constexpr int kMinNumber = 1;
inline constexpr int kMaxNumber = 256;
inline constexpr std::size_t kQueueLimit = 32;  // призупинення, коли значень > 32

inline constexpr int kMinRounds = 1;
inline constexpr int kMaxRounds = 100;
inline constexpr std::size_t kMinTake = 1;
inline constexpr std::size_t kMaxTake = kQueueLimit + 1;  // більше в черзі не буває

}  // namespace demo

// Параметри демонстрації (формат вхідних даних описано в README.md).
struct DemoConfig {
    int rounds = 0;
    std::size_t takePerRound = 0;
    std::uint32_t seed = 0;
    bool seedGiven = false;
};

// Помилка у вхідних даних користувача.
class InputError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// Розбирає параметри: "<раунди> <скільки_забирати> [seed]".
// Текст після '#' до кінця рядка вважається коментарем.
// Кидає InputError, якщо дані некоректні.
DemoConfig parseDemoConfig(std::istream& input);
