#include "QueueWaitController.h"

QueueWaitController::QueueWaitController(const NumberQueue& queue, std::size_t limit) noexcept
    : queue_(&queue), limit_(limit) {}

bool QueueWaitController::await_ready() const noexcept {
    // "Готовність" означає, що чекати не треба і сопрограма працює далі.
    return !isSuspendRequired();
}

bool QueueWaitController::isSuspendRequired() const noexcept {
    return queue_->size() > limit_;
}
