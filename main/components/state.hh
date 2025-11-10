// state.hh

#pragma once
#include <functional>

namespace foundation {

  template <typename T, int MaxListeners = 8>
  class State {
  private:
    T value;
    std::function<void(const T&)> listeners[MaxListeners];
    int count = 0;

  public:
    explicit State(const T& initial) : value(initial) {}

    const T& get() const { return value; }

    void set(const T& newValue) {
      if (newValue != value) {
          value = newValue;
          force_notify();
      }
    }

    int subscribe(std::function<void(const T&)> cb) {
      if (count < MaxListeners) {
          listeners[count] = std::move(cb);
          return count++;
      }
      return -1;
    }

    int subscribe_once(std::function<void(const T&)> cb) {
      if (count < MaxListeners) {
          int id = count;
          listeners[count++] = [this, cb = std::move(cb), id](const T& val) mutable {
            cb(val);
            this->unsubscribe(id);
          };
          return id;
      }
      return -1;
    }

    void unsubscribe(int id) {
      if (id >= 0 && id < count) {
          listeners[id] = nullptr;
      }
    }

    void force_notify() {
      for (int i = 0; i < count; ++i) {
          if (listeners[i])
            listeners[i](value);
      }
    }
  };

}  // namespace foundation
