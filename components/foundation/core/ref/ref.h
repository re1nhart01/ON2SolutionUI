#pragma once

#include <string>
#include <memory>

namespace foundation {


  struct RefBase {
    virtual ~RefBase() = default;
  };

  class BaseComponent;
  template<typename Props> class Component;

  template <typename T = BaseComponent>
  class Ref : public RefBase {
  public:
    std::string name;
    T* linked = nullptr;

    ~Ref() = default;

    explicit Ref(const std::string& name)
        : name(name) {}

    void set(T* component) {
      if (this->linked != nullptr) return;
      this->linked = component;
    }

    T* get() const {
      return linked;
    }

    template <typename C>
    C* cast() const {
      return dynamic_cast<C*>(linked);
    }
  };
}
