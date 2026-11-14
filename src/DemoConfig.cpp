#include "DemoConfig.h"

#include <charconv>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace {

constexpr std::string_view kUtf8Bom = "\xEF\xBB\xBF";

// Блокнот та інші редактори можуть додавати BOM на початку UTF-8 файлу.
void skipUtf8Bom(std::string& line) {
    if (line.starts_with(kUtf8Bom)) {
        line.erase(0, kUtf8Bom.size());
    }
}

std::vector<std::string> readTokens(std::istream& input) {
    std::vector<std::string> tokens;
    std::string line;
    for (bool firstLine = true; std::getline(input, line); firstLine = false) {
        if (firstLine) {
            skipUtf8Bom(line);
        }
        if (const auto comment = line.find('#'); comment != std::string::npos) {
            line.erase(comment);
        }
        std::istringstream words(line);
        std::string token;
        while (words >> token) {
            tokens.push_back(token);
        }
    }
    if (input.bad()) {
        throw InputError("не вдалося прочитати вхідні дані");
    }
    return tokens;
}

// Усі межі параметрів (зокрема seed до 2^32 - 1) вміщуються в long long,
// тому розбираємо в нього, а від'ємні числа відсікаються перевіркою меж.
long long parseInteger(const std::string& token, long long min, long long max,
                       const std::string& name) {
    long long value = 0;
    const char* begin = token.data();
    const char* end = begin + token.size();
    const auto [stop, error] = std::from_chars(begin, end, value);

    if (error == std::errc::result_out_of_range) {
        throw InputError(name + ": число \"" + token + "\" занадто велике за модулем");
    }
    if (error != std::errc{} || stop != end) {
        throw InputError(name + ": \"" + token + "\" не є цілим числом");
    }
    if (value < min || value > max) {
        throw InputError(name + ": значення " + token + " поза допустимим проміжком [" +
                         std::to_string(min) + ", " + std::to_string(max) + "]");
    }
    return value;
}

}  // namespace

DemoConfig parseDemoConfig(std::istream& input) {
    const std::vector<std::string> tokens = readTokens(input);
    if (tokens.size() < 2) {
        throw InputError("очікується щонайменше 2 числа (раунди і кількість для забирання), отримано " +
                         std::to_string(tokens.size()));
    }
    if (tokens.size() > 3) {
        throw InputError("зайві дані: \"" + tokens[3] + "\" (очікується не більше 3 чисел)");
    }

    DemoConfig config;
    config.rounds = static_cast<int>(
        parseInteger(tokens[0], demo::kMinRounds, demo::kMaxRounds, "кількість раундів"));
    config.takePerRound = static_cast<std::size_t>(
        parseInteger(tokens[1], demo::kMinTake, demo::kMaxTake, "кількість для забирання за раунд"));
    config.seedGiven = tokens.size() == 3;
    config.seed = config.seedGiven
                      ? static_cast<std::uint32_t>(parseInteger(
                            tokens[2], 0, std::numeric_limits<std::uint32_t>::max(), "seed"))
                      : std::random_device{}();
    return config;
}
