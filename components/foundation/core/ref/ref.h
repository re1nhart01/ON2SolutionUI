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
    bool ready = false;

    ~Ref() = default;

    explicit Ref(const std::string& name)
        : name(name) {}

    void set(T* component) {
      if (this->linked != nullptr) return;
      this->linked = component;
      this->ready = true;
    }

    T* get() const {
      return linked;
    }

    bool is_ready()
    {
      return linked != nullptr && ready;
    }

    void unlink()
    {
      this->linked = nullptr;
      this->ready = false;
    }

    template <typename C>
    C* cast() const {
      return dynamic_cast<C*>(linked);
    }
  };
}
