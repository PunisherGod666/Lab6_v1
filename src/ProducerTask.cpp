#include "ProducerTask.h"

#include <stdexcept>
#include <utility>

ProducerTask ProducerTask::promise_type::get_return_object() noexcept {
    return ProducerTask(Handle::from_promise(*this));
}

const QueueWaitController& ProducerTask::promise_type::yield_value(int value) noexcept {
    lastValue = value;
    return *controller;  // ProducerTask::resume() гарантує, що контролер приєднано
}

ProducerTask::ProducerTask(Handle handle) noexcept : handle_(handle) {}

ProducerTask::ProducerTask(ProducerTask&& other) noexcept
    : handle_(std::exchange(other.handle_, nullptr)) {}

ProducerTask& ProducerTask::operator=(ProducerTask&& other) noexcept {
    if (this != &other) {
        destroy();
        handle_ = std::exchange(other.handle_, nullptr);
    }
    return *this;
}

ProducerTask::~ProducerTask() {
    destroy();
}

void ProducerTask::attach(const QueueWaitController& controller) noexcept {
    if (handle_) {
        handle_.promise().controller = &controller;
    }
}

void ProducerTask::resume() {
    if (!handle_ || handle_.done()) {
        throw std::logic_error("неможливо відновити завершену сопрограму");
    }
    if (handle_.promise().controller == nullptr) {
        throw std::logic_error("до сопрограми не приєднано контролер очікування");
    }
    handle_.resume();
    if (auto exception = std::exchange(handle_.promise().exception, nullptr)) {
        std::rethrow_exception(exception);
    }
}

bool ProducerTask::isDone() const noexcept {
    return !handle_ || handle_.done();
}

int ProducerTask::lastValue() const noexcept {
    return handle_ ? handle_.promise().lastValue : 0;
}

void ProducerTask::destroy() noexcept {
    if (handle_) {
        handle_.destroy();
        handle_ = nullptr;
    }
}
