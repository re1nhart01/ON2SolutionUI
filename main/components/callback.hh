//
// Created by evgeniy on 3/31/25.
//

#ifndef CALLBACK_HH
#define CALLBACK_HH

#endif //CALLBACK_HH
#pragma once

#include <memory>

template<typename T>
class Callback {
    struct Base {
        virtual void call(const T&) = 0;
        virtual ~Base() = default;
    };

    template<typename F>
    struct Impl : Base {
        F fn;
        explicit Impl(F&& f) : fn(std::move(f)) {}
        void call(const T& val) override { fn(val); }
    };

    Base* impl;

public:
    template<typename F>
    Callback(F&& f) : impl(new Impl<F>(std::forward<F>(f))) {}

    Callback(const Callback&) = delete;
    Callback& operator=(const Callback&) = delete;

    Callback(Callback&& other) noexcept : impl(other.impl) {
        other.impl = nullptr;
    }

    Callback& operator=(Callback&& other) noexcept {
        if (this != &other) {
            delete impl;
            impl = other.impl;
            other.impl = nullptr;
        }
        return *this;
    }

    void operator()(const T& val) const {
        if (impl) impl->call(val);
    }

    ~Callback() {
        delete impl;
    }
};
