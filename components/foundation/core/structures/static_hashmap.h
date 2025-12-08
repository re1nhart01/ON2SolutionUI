#pragma once
#include <functional>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <optional>

template <typename Key, typename Value>
struct StaticHashMapNode
{
  Key key;
  Value value;
  bool created = false;
  bool removed = false;
};

template<typename Key, typename Value, size_t N>
class StaticHashMap
{
private:
  StaticHashMapNode<Key, Value> data[N];

public:
  StaticHashMap() {
  };
  ~StaticHashMap() = default;

  size_t hasher(const Key& k) const {
    return std::hash<Key>{}(k) % N;
  }

  void put(const Key& key, const Value& value)
  {
    const size_t initial_hash = hasher(key);
    for (size_t i = 0; i < N; ++i)
    {
      size_t pos = (initial_hash + i) % N;

      if (data[pos].created && data[pos].key == key)
      {
        data[pos].value = value;
        data[pos].removed = false;
        return;
      }

      if (!data[pos].created || data[pos].removed)
      {
        data[pos].value = value;
        data[pos].key = key;
        data[pos].created = true;
        data[pos].removed = false;
        // data[pos].hash = initial_hash;
        return;
      }
    }
  }

  Value* get(const Key& key)
  {
    const size_t initial_hash = hasher(key);
    for (size_t i = 0; i < N; ++i)
    {
      size_t pos = (initial_hash + i) % N;

      if (!data[pos].created && !data[pos].removed)
      {
        return nullptr;
      }

      if (data[pos].created && !data[pos].removed && data[pos].key == key)
      {
        return &data[pos].value;
      }
    }
    return nullptr;
  }

  const Value* get(const Key& key) const
  {
    const size_t initial_hash = hasher(key);
    for (size_t i = 0; i < N; ++i)
    {
      size_t pos = (initial_hash + i) % N;

      if (!data[pos].created && !data[pos].removed)
      {
        return nullptr;
      }

      if (data[pos].created && !data[pos].removed && data[pos].key == key)
      {
        return &data[pos].value;
      }
    }
    return nullptr;
  }

  bool remove(const Key& key)
  {
    const size_t initial_hash = hasher(key);
    for (size_t i = 0; i < N; ++i)
    {
      size_t pos = (initial_hash + i) % N;

      if (!data[pos].created && !data[pos].removed)
      {
        return false;
      }

      if (data[pos].created && !data[pos].removed && data[pos].key == key)
      {
        data[pos].removed = true;
        return true;
      }
    }
    return false;
  }

  bool contains(const Key& key) const {
    return get(key) != nullptr;
  }
};