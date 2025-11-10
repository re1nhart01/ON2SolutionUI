#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "styling.hh"

namespace foundation {

  class StyleStorage {
  private:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<Styling>> storage;
  public:
    StyleStorage();
    explicit StyleStorage(std::string name);
    ~StyleStorage() = default;

  public:
    StyleStorage* operator=(const StyleStorage*) const = delete;

    void set(const std::string& name, std::shared_ptr<Styling> style);
    std::shared_ptr<Styling> take(const std::string& name);
    bool del(const std::string& name);
    std::shared_ptr<Styling> get(const std::string& name) const;

#if STYLE_STORE_SINGLETON
    static StyleStorage& instance();
#endif
  };

}
