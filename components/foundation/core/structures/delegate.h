#pragma once

#include <cstddef>
#include <cstring>
#include <utility>

namespace foundation
{

  template <typename Signature, size_t Capacity = 32>
class Delegate;

template <typename R, typename... Args, size_t Capacity>
class Delegate<R(Args...), Capacity> {
private:
    using InvokeFn = R (*)(void*, Args&&...);

    alignas(std::max_align_t) unsigned char storage[Capacity];
    InvokeFn invoke = nullptr;
    void (*deleter)(void*) = nullptr;

public:
    Delegate() = default;

    ~Delegate() {
        reset();
    }

    Delegate(const Delegate& other) {
        if (other.invoke) {
            std::memcpy(storage, other.storage, Capacity);
            invoke = other.invoke;
            deleter = other.deleter;
        }
    }

    Delegate& operator=(const Delegate& other) {
        if (this != &other) {
            reset();
            if (other.invoke) {
                std::memcpy(storage, other.storage, Capacity);
                invoke = other.invoke;
                deleter = other.deleter;
            }
        }
        return *this;
    }

    template <typename Callable>
    Delegate(Callable c) {
        static_assert(sizeof(Callable) <= Capacity,
                      "Callable too large for StaticFunction buffer");

        new (storage) Callable(std::move(c));

        invoke = [](void* ptr, Args&&... args) -> R {
            return (*reinterpret_cast<Callable*>(ptr))(
                std::forward<Args>(args)...);
        };

        deleter = [](void* ptr) {
            reinterpret_cast<Callable*>(ptr)->~Callable();
        };
    }

    template <typename Callable>
    Delegate& operator=(Callable c) {
        reset();

        static_assert(sizeof(Callable) <= Capacity,
                      "Callable too large for StaticFunction buffer");

        new (storage) Callable(std::move(c));

        invoke = [](void* ptr, Args&&... args) -> R {
            return (*reinterpret_cast<Callable*>(ptr))(
                std::forward<Args>(args)...);
        };

        deleter = [](void* ptr) {
            reinterpret_cast<Callable*>(ptr)->~Callable();
        };

        return *this;
    }

    void reset() {
        if (deleter) {
            deleter(storage);
        }
        invoke = nullptr;
        deleter = nullptr;
    }

    R operator()(Args... args) {
        return invoke(storage, std::forward<Args>(args)...);
    }

    explicit operator bool() const { return invoke != nullptr; }
};
}


/**
 *
 *
    int x = 10;
    StaticFunction<void()> fn = [x]() {
        printf("x = %d\n", x);
    };

    fn(); // x = 10
--------

    void hello() { printf("hello\n"); }

    StaticFunction<void()> fn = hello;
    fn(); // hello

--------

    struct A {
        void operator()(int v) { printf("%d\n", v); }
    };

    StaticFunction<void(int)> fn = A{};
    fn(123);

 *
 */