#pragma once

#include <coroutine>
#include <exception>

#include "QueueWaitController.h"

// Тип, що повертає сопрограма-виробник. Володіє кадром сопрограми (RAII):
// кадр знищується в деструкторі, тож витоків пам'яті немає.
//
// Сама сопрограма лише робить co_yield записаного числа. promise_type
// перетворює co_yield на очікування на приєднаному контролері, тому код
// сопрограми нічого не знає про контролер очікування.
class ProducerTask {
public:
    struct promise_type {
        const QueueWaitController* controller = nullptr;
        int lastValue = 0;
        std::exception_ptr exception;

        ProducerTask get_return_object() noexcept;
        std::suspend_always initial_suspend() const noexcept { return {}; }
        std::suspend_always final_suspend() const noexcept { return {}; }
        const QueueWaitController& yield_value(int value) noexcept;
        void return_void() const noexcept {}
        void unhandled_exception() noexcept { exception = std::current_exception(); }
    };

    ProducerTask(ProducerTask&& other) noexcept;
    ProducerTask& operator=(ProducerTask&& other) noexcept;
    ProducerTask(const ProducerTask&) = delete;
    ProducerTask& operator=(const ProducerTask&) = delete;
    ~ProducerTask();

    // Приєднує контролер очікування. Контролер має жити довше за задачу.
    void attach(const QueueWaitController& controller) noexcept;
    void attach(const QueueWaitController&&) = delete;

    // Відновлює сопрограму і повертає керування, коли контролер її призупинить.
    void resume();

    bool isDone() const noexcept;
    int lastValue() const noexcept;

private:
    using Handle = std::coroutine_handle<promise_type>;

    explicit ProducerTask(Handle handle) noexcept;
    void destroy() noexcept;

    Handle handle_;
};
