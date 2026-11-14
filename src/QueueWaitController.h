#pragma once

#include <coroutine>
#include <cstddef>

#include "NumberQueue.h"

// Контролер очікування (awaiter) для сопрограми-виробника.
//
// Уся логіка призупинення зосереджена тут: після кожного запису в чергу
// сопрограма потрапляє в точку co_yield, і саме контролер вирішує, чи
// продовжити виконання (await_ready() == true), чи призупинити сопрограму.
// Призупинення відбувається, коли черга містить більше ніж limit значень.
class QueueWaitController {
public:
    QueueWaitController(const NumberQueue& queue, std::size_t limit) noexcept;

    // Інтерфейс awaiter-а, який використовує компілятор у co_await / co_yield.
    bool await_ready() const noexcept;
    void await_suspend(std::coroutine_handle<>) const noexcept {}
    void await_resume() const noexcept {}

    bool isSuspendRequired() const noexcept;

private:
    const NumberQueue* queue_;
    std::size_t limit_;
};
