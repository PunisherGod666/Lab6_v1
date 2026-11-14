#pragma once

#include <ostream>

#include "DemoConfig.h"

// Запускає сопрограму-виробника і в кожному раунді:
// відновлює її, показує вміст черги після призупинення, забирає частину значень.
void runDemo(const DemoConfig& config, std::ostream& out);
