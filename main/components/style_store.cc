#include "style_store.hh"

#include <memory>
#include <utility>

namespace foundation {

  StyleStorage::StyleStorage() : name("STYLE_STORAGE") {}

  StyleStorage::StyleStorage(std::string name) : name(std::move(name)) {}

  void StyleStorage::set(const std::string& name, std::shared_ptr<Styling> style) {
    this->storage[name] = std::move(style);
  }

  std::shared_ptr<Styling> StyleStorage::take(const std::string& name) {
    auto existing = this->get(name);
    if (!existing) {
        auto created = std::make_shared<Styling>();
        this->set(name, created);
        return created;
    }
    return existing;
  }

  bool StyleStorage::del(const std::string& name) {
    return this->storage.erase(name) > 0;
  }

  std::shared_ptr<Styling> StyleStorage::get(const std::string& name) const {
    auto it = this->storage.find(name);
    return it != this->storage.end() ? it->second : nullptr;
  }

#if STYLE_STORE_SINGLETON
  StyleStorage& StyleStorage::instance() {
    static StyleStorage instance;
    return instance;
  }
#endif

}
