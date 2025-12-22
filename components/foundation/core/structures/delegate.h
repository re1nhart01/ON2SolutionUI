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
    using InvokeFn = R (*)(void*, Args&&...);

    alignas(void*) unsigned char storage[Capacity];
    InvokeFn invoke = nullptr;
    void (*deleter)(void*) = nullptr;

  public:
    Delegate() = default;
    ~Delegate() { reset(); }

    template <typename Callable>
    Delegate(Callable c) {
      static_assert(sizeof(Callable) <= Capacity);
      new (storage) Callable(std::move(c));
      invoke = [](void* p, Args&&... args) -> R {
        return (*reinterpret_cast<Callable*>(p))(std::forward<Args>(args)...);
      };
      deleter = [](void* p) {
        reinterpret_cast<Callable*>(p)->~Callable();
      };
    }

    void reset() {
      if (deleter) deleter(storage);
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