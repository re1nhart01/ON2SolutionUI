#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include "core/styling/styling.h"

namespace foundation {

  class StyleStorage {
  public:
    using StyleFn = std::function<void(Styling&)>;

  private:
    std::string name;
    std::unordered_map<std::string, StyleFn> storage;

  public:
    StyleStorage()
      : name("STYLE_STORAGE") {}

    explicit StyleStorage(std::string name)
      : name(std::move(name)) {}

    StyleStorage(const StyleStorage&) = delete;
    StyleStorage& operator=(const StyleStorage&) = delete;

    // === API ===

    void set(const std::string& key, StyleFn fn)
    {
      storage[key] = std::move(fn);
    }

    bool del(const std::string& key)
    {
      return storage.erase(key) > 0;
    }

    StyleFn get(const std::string& key) const
    {
      auto it = storage.find(key);
      return it != storage.end() ? it->second : StyleFn{};
    }

#if STYLE_STORE_SINGLETON
    static StyleStorage& instance()
    {
      static StyleStorage instance;
      return instance;
    }
#endif
  };

}
