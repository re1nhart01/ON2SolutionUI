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
    virtual bool compare(void* other_val) = 0;
  };

  template<typename T>
  struct TypedValue final : public ValueContainer {
    T data;
    explicit TypedValue(T val) : data(val) {}

    bool compare(void* other_val) override {
      return data == *static_cast<T*>(other_val);
    }
  };

    template <typename T>
    class Reactive : public IReactive
    {
    private:
      struct Binding {
        std::string key;
        void* component;
        Delegate<void(void*, void*), 40> updater;
      };

      std::unique_ptr<ValueContainer> value_store;
      std::vector<Binding> bindings;

    public:
      Reactive() : value_store(nullptr) {}
      ~Reactive() override { bindings.clear(); }

      template<typename TComp>
      void attach(std::string key, TComp* component, Delegate<void(TComp*, T), 40> updater)
      {
        Binding binding;
        binding.key = key;
        binding.component = static_cast<void*>(component);
        binding.updater = [updater](void* comp_ptr, void* val_ptr) {
          updater(static_cast<TComp*>(comp_ptr), *static_cast<T*>(val_ptr));
        };

        this->bindings.push_back(binding);
        ESP_LOGI("REACTIVE", "Attached. Total bindings: %d", this->bindings.size());
      }

      void detach(const void *component) override
      {
        for (auto it = bindings.begin(); it != bindings.end();) {
            if (it->component == component) it = bindings.erase(it);
            else ++it;
        }
      }

      void set(const T& newValue)
      {
        if (value_store && value_store->compare(&newValue)) return;

        value_store = std::make_unique<TypedValue<T>>(newValue);

        for (auto &b : bindings) {
          if (b.component != nullptr)
          {
            b.updater(b.component, &newValue);
          }
        }
      }

      void notify(Delegate<void(std::vector<Binding> bindings), 40> callback)
      {
        if (this->bindings == nullptr) return;
        callback(this->bindings);
      }

      T get() const {
        if (!value_store) return T();
        return static_cast<TypedValue<T>*>(value_store.get())->data;
      }
    };
};