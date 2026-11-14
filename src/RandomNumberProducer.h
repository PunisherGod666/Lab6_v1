#pragma once

#include "NumberQueue.h"
#include "ProducerTask.h"
#include "RandomNumberSource.h"

// Сопрограма: нескінченно генерує випадкові числа і записує їх у чергу.
// Після кожного запису виконує co_yield записаного числа; чи буде вона при
// цьому призупинена, вирішує контролер очікування, про який сопрограма не знає.
//
// Черга передається за посиланням і має жити довше за повернуту задачу.
ProducerTask produceRandomNumbers(NumberQueue& queue, RandomNumberSource source);
