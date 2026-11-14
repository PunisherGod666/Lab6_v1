#include "RandomNumberProducer.h"

ProducerTask produceRandomNumbers(NumberQueue& queue, RandomNumberSource source) {
    while (true) {
        const int number = source.next();
        queue.push(number);
        co_yield number;
    }
}
