#pragma once

#include "core/shortcuts.h"
#include "core/structures/static_hashmap.h"

namespace foundation
{
  template<size_t N>
  class SharedRefStore
  {
    private:
    StaticHashMap<std::string, Ref<VNode>, N> refs;
    public:

    template<typename C>
    std::shared_ptr<Ref<C>> create(std::string k)
    {

      Ref<VNode> base_ref(k);
      base_ref.set(nullptr);

      refs.put(k, base_ref);

      auto* storedRef = refs.get(k);
      auto typedRef = std::make_shared<Ref<C>>();

      typedRef->ptr = static_cast<C*>(storedRef->ptr);

      return typedRef;
    }

    template<typename C>
    void set(const std::string& key, C* node)
    {
      auto* ref = refs.get(key);
      if (ref)
        ref->ptr = static_cast<VNode*>(node);
    }

    Ref<VNode>* get(const std::string& key)
    {
      return refs.get(key);
    }
  };
}