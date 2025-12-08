#pragma once

#include "core/shortcuts.h"
#include "core/structures/static_hashmap.h"

namespace foundation
{
  template<size_t N>
  class SharedRefStore
  {
    private:
    StaticHashMap<std::string, std::shared_ptr<RefBase>, N> refs;

    public:
    SharedRefStore() = default;
    ~SharedRefStore() = default;

    template<typename C>
    std::shared_ptr<Ref<C>> create(const std::string &key)
    {
      auto base_ref_ptr = refs.get(key);

      if (base_ref_ptr) {
        return std::static_pointer_cast<Ref<C>>(*base_ref_ptr);
      }

      auto new_ref = std::make_shared<Ref<C>>(key);

      refs.put(key, new_ref);

      return new_ref;
    }

    template<typename C>
    std::shared_ptr<Ref<C>> get(const std::string& key) const
    {
      auto base_ref_ptr = refs.get(key);

      if (!base_ref_ptr) return nullptr;

      return std::static_pointer_cast<Ref<C>>(*base_ref_ptr);
    }

    void remove(const std::string& key) {
        refs.remove(key);
    }
  };
}