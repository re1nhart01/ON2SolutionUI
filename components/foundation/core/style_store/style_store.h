#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "core/styling/styling.h"

namespace foundation {
  class StyleStorage {
  private:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<Styling>> storage;
  public:
    StyleStorage() : name("STYLE_STORAGE") {};
    explicit StyleStorage(std::string name) : name(std::move(name)) {};

    ~StyleStorage() = default;

  public:
    StyleStorage* operator=(const StyleStorage*) const = delete;

    void set(const std::string& name, std::shared_ptr<Styling> style) {
      this->storage[name] = std::move(style);
    };

    std::shared_ptr<Styling> take(const std::string& name) {
      auto existing = this->get(name);
      if (!existing) {
          auto created = std::make_shared<Styling>();
          this->set(name, created);
          return created;
      }
      return existing;
    };

    bool del(const std::string& name)
    {
      return this->storage.erase(name) > 0;
    };
    std::shared_ptr<Styling> get(const std::string& name) const
    {
      const auto it = this->storage.find(name);
      return it != this->storage.end() ? it->second : nullptr;
    };

#if STYLE_STORE_SINGLETON
    static StyleStorage& instance() {
      static StyleStorage instance;
      return instance;
    }
#endif
  };

}
