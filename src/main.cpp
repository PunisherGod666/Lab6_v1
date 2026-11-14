// Лабораторна робота 6, варіант 1.
// Сопрограма генерує випадкові натуральні числа з [1, 256] і записує їх у чергу;
// контролер очікування призупиняє її, коли в черзі більше 32 значень.
//
// Запуск:
//   lab6.exe              - параметри вводяться з клавіатури одним рядком;
//   lab6.exe <файл>       - параметри читаються з текстового файлу.
// Формат параметрів описано в README.md та DemoConfig.h.

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Demo.h"
#include "DemoConfig.h"

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace {

void setUpConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

void printUsage(std::ostream& out) {
    out << "Використання: lab6.exe [файл_параметрів]\n"
        << "Формат параметрів: <раунди " << demo::kMinRounds << ".." << demo::kMaxRounds
        << "> <забирати_за_раунд " << demo::kMinTake << ".." << demo::kMaxTake
        << "> [seed 0..4294967295]\n";
}

DemoConfig readConfigFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw InputError("не вдалося відкрити файл \"" + path + "\"");
    }
    return parseDemoConfig(file);
}

DemoConfig readConfigFromConsole() {
    std::cout << "Введіть <раунди> <забирати_за_раунд> [seed]: ";
    std::string line;
    if (!std::getline(std::cin, line)) {
        throw InputError("вхідні дані відсутні");
    }
    std::istringstream input(line);
    return parseDemoConfig(input);
}

DemoConfig readConfig(int argc, char* argv[]) {
    if (argc > 2) {
        throw InputError("забагато аргументів командного рядка");
    }
    return argc == 2 ? readConfigFromFile(argv[1]) : readConfigFromConsole();
}

}  // namespace

int main(int argc, char* argv[]) {
    setUpConsole();
    try {
        runDemo(readConfig(argc, argv), std::cout);
        return 0;
    } catch (const InputError& error) {
        std::cerr << "Помилка вхідних даних: " << error.what() << '\n';
        printUsage(std::cerr);
        return 1;
    } catch (const std::exception& error) {
        std::cerr << "Внутрішня помилка: " << error.what() << '\n';
        return 2;
    }
}
