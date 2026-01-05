#pragma once


#include "core/structures/delegate.h"
#include <memory>
#include <vector>
#include "esp_log.h";

namespace foundation
{

  class IReactive {
  public:
    virtual ~IReactive() = default;
    virtual void detach(const void* component) = 0;
  };

  struct ValueContainer {
    virtual ~ValueContainer() = default;
    virtual bool compare(const void* other_val) const = 0;
  };

  template<typename T>
  struct TypedValue final : public ValueContainer {
    T data;
    explicit TypedValue(T val) : data(val) {}

    bool compare(const void* other_val) const override {
      return data == *static_cast<const T*>(other_val);
    }
  };

    template <typename T>
    class Reactive : public IReactive
    {
    private:
      struct Binding {
        std::string key;
        void* component;
        Delegate<void(void*, const T&), 40> updater;
      };

      std::unique_ptr<ValueContainer> value_store;
      std::vector<Binding> bindings;

    public:
      Reactive() : value_store(std::make_unique<TypedValue<T>>(T{})) {}

      explicit Reactive(const T& default_val)
        : value_store(std::make_unique<TypedValue<T>>(default_val)) {}
      ~Reactive() override { bindings.clear(); }

      template<typename TComp>
      void attach(std::string key,TComp* component, Delegate<void(TComp*, const T&)> fn)
      {
        bindings.push_back(Binding{
          .key = std::move(key),
          .component = component,
          .updater = [fn](void* c, const T& v) {
            fn(static_cast<TComp*>(c), v);
          }
        });
      }

      void detach(const void *component) override
      {
        for (auto it = bindings.begin(); it != bindings.end();) {
            if (it->component == component) it = bindings.erase(it);
            else ++it;
        }
      }

      void set(Delegate<T(const T&)> fn) {
        T prev = get();
        T next = fn(prev);
        set(next);
      }

      void set(const T& newValue)
      {
        if (value_store && value_store->compare(&newValue)) return;

        value_store = std::make_unique<TypedValue<T>>(newValue);

        for (auto &b : bindings) {
          if (b.component != nullptr)
          {
            b.updater(b.component, newValue);
          }
        }
      }

      void notify(Delegate<void(const std::vector<Binding>&), 40> cb)
      {
        cb(bindings);
      }

      const T& get() const {
        static T empty{};
        return value_store
          ? static_cast<TypedValue<T>*>(value_store.get())->data
          : empty;
      }
    };
};