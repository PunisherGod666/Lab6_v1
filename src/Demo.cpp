#include "Demo.h"

#include <iomanip>
#include <vector>

#include "QueueWaitController.h"
#include "RandomNumberProducer.h"

namespace {

constexpr std::size_t kNumbersPerLine = 16;

void printNumbers(std::ostream& out, const std::vector<int>& numbers) {
    for (std::size_t i = 0; i < numbers.size(); ++i) {
        out << (i % kNumbersPerLine == 0 ? "    " : " ") << std::setw(3) << numbers[i];
        if ((i + 1) % kNumbersPerLine == 0 || i + 1 == numbers.size()) {
            out << '\n';
        }
    }
}

std::vector<int> snapshot(NumberQueue queue) {
    std::vector<int> numbers;
    numbers.reserve(queue.size());
    while (!queue.empty()) {
        numbers.push_back(queue.front());
        queue.pop();
    }
    return numbers;
}

std::vector<int> takeFromQueue(NumberQueue& queue, std::size_t count) {
    std::vector<int> taken;
    while (taken.size() < count && !queue.empty()) {
        taken.push_back(queue.front());
        queue.pop();
    }
    return taken;
}

void printHeader(std::ostream& out, const DemoConfig& config) {
    out << "Сопрограма генерує випадкові числа з [" << demo::kMinNumber << ", " << demo::kMaxNumber
        << "] і записує їх у чергу.\n"
        << "Контролер очікування призупиняє її, коли в черзі більше " << demo::kQueueLimit
        << " значень.\n"
        << "Раундів: " << config.rounds << ", споживач забирає за раунд: " << config.takePerRound
        << ", seed: " << config.seed << (config.seedGiven ? "" : " (випадковий)") << "\n\n";
}

void printRound(std::ostream& out, int round, std::size_t written, const NumberQueue& queue,
                const ProducerTask& producer) {
    out << "Раунд " << round << ": сопрограму відновлено, записано нових чисел: " << written
        << ", останнє: " << producer.lastValue() << ".\n"
        << "  Контролер призупинив сопрограму. Черга (значень: " << queue.size() << "):\n";
    printNumbers(out, snapshot(queue));
}

void printConsumed(std::ostream& out, const std::vector<int>& taken, std::size_t remaining) {
    out << "  Споживач забрав (значень: " << taken.size() << "):\n";
    printNumbers(out, taken);
    out << "  Залишилось у черзі: " << remaining << "\n\n";
}

}  // namespace

void runDemo(const DemoConfig& config, std::ostream& out) {
    NumberQueue queue;
    const QueueWaitController controller(queue, demo::kQueueLimit);
    ProducerTask producer = produceRandomNumbers(
        queue, RandomNumberSource(config.seed, demo::kMinNumber, demo::kMaxNumber));
    producer.attach(controller);

    printHeader(out, config);
    for (int round = 1; round <= config.rounds; ++round) {
        const std::size_t sizeBefore = queue.size();
        producer.resume();
        printRound(out, round, queue.size() - sizeBefore, queue, producer);

        const std::vector<int> taken = takeFromQueue(queue, config.takePerRound);
        printConsumed(out, taken, queue.size());
    }
    out << "Демонстрацію завершено.\n";
}
