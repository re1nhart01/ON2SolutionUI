#pragma once

#include <cstddef>
#include <cstring>
#include <new>
#include <type_traits>
#include <utility>

namespace foundation {

    template <typename Signature, size_t Capacity = 40>
    class Delegate;

    template <typename R, typename... Args, size_t Capacity>
    class Delegate<R(Args...), Capacity> {

        using InvokeFn = R (*)(void*, Args...);
        using CopyFn   = void (*)(const void*, void*);
        using DeleteFn = void (*)(void*);

        alignas(void*) mutable unsigned char storage[Capacity];
        InvokeFn invoke_ptr = nullptr;
        CopyFn   copy_ptr   = nullptr;
        DeleteFn delete_ptr = nullptr;
        bool     is_dynamic = false;

        struct DynamicHolder {
            void* ptr;
        };

    public:
        Delegate() = default;

        template <typename Callable,
                  typename = std::enable_if_t<!std::is_same_v<std::decay_t<Callable>, Delegate>>>
        Delegate(Callable c) {
            using T = std::decay_t<Callable>;


            if constexpr (requires(Callable cb) { (bool)cb; }) {
                if (!c) {
                    return;
                }
            }

            if constexpr (sizeof(T) <= Capacity && std::is_nothrow_move_constructible_v<T>) {
                new (storage) T(std::move(c));
                is_dynamic = false;

                invoke_ptr = [](void* p, Args... args) -> R {
                    return (*reinterpret_cast<T*>(p))(std::forward<Args>(args)...);
                };
                copy_ptr = [](const void* src, void* dst) {
                    new (dst) T(*reinterpret_cast<const T*>(src));
                };
                delete_ptr = [](void* p) {
                    reinterpret_cast<T*>(p)->~T();
                };
            } else {
                T* heap_ptr = new T(std::move(c));
                reinterpret_cast<DynamicHolder*>(storage)->ptr = heap_ptr;
                is_dynamic = true;

                invoke_ptr = [](void* p, Args... args) -> R {
                    void* actual_ptr = reinterpret_cast<DynamicHolder*>(p)->ptr;
                    return (*reinterpret_cast<T*>(actual_ptr))(std::forward<Args>(args)...);
                };
                copy_ptr = [](const void* src, void* dst) {
                    void* src_ptr = reinterpret_cast<const DynamicHolder*>(src)->ptr;
                    void* new_heap_ptr = new T(*reinterpret_cast<const T*>(src_ptr));
                    reinterpret_cast<DynamicHolder*>(dst)->ptr = new_heap_ptr;
                };
                delete_ptr = [](void* p) {
                    delete reinterpret_cast<T*>(reinterpret_cast<DynamicHolder*>(p)->ptr);
                };
            }
        }

        ~Delegate() { reset(); }

        Delegate(const Delegate& other) : invoke_ptr(other.invoke_ptr), copy_ptr(other.copy_ptr), delete_ptr(other.delete_ptr), is_dynamic(other.is_dynamic) {
            if (other.copy_ptr) {
                other.copy_ptr(other.storage, storage);
            }
        }

        Delegate& operator=(const Delegate& other) {
            if (this != &other) {
                reset();
                invoke_ptr = other.invoke_ptr;
                copy_ptr = other.copy_ptr;
                delete_ptr = other.delete_ptr;
                is_dynamic = other.is_dynamic;
                if (other.copy_ptr) {
                    other.copy_ptr(other.storage, storage);
                }
            }
            return *this;
        }

        Delegate(Delegate&& other) noexcept {
            std::memcpy(this, &other, sizeof(Delegate));
            other.invoke_ptr = nullptr;
            other.copy_ptr = nullptr;
            other.delete_ptr = nullptr;
        }

        void reset() {
            if (delete_ptr) {
                delete_ptr(storage);
                delete_ptr = nullptr;
            }
            invoke_ptr = nullptr;
            copy_ptr = nullptr;
        }

        R operator()(Args... args) const {
            if (!invoke_ptr) {
                if constexpr (!std::is_void_v<R>) return R{};
                else return;
            }
            return invoke_ptr((void*)storage, std::forward<Args>(args)...);
        }

        explicit operator bool() const { return invoke_ptr != nullptr; }
    };
}