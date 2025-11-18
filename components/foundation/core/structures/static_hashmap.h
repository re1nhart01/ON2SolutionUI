#pragma once

template<typename Key, typename Value>
struct StaticHashMapNode
{
  Key key;
  Value value;
  bool created;
  size_t hash;
};

template<typename Key, typename Value, size_t N>
class StaticHashMap
{
  StaticHashMapNode<Key, Value> data[N];

  public:
  StaticHashMap() {};
  ~StaticHashMap() {};

  size_t hasher(const Key& k) const {
    return std::hash<Key>{}(k) % N;
  }

  void put(const Key& key, const Value& value)
  {
    const size_t hash = hasher(key);
    for (size_t i = 0; i < N; i++)
      {
        size_t pos = (hash + i) % N;
        if (!data[pos].created && data[pos].key != key)
        {
          data[pos].value = value;
          data[pos].key = key;
          data[pos].created = true;
          data[pos].hash = hash;
        }
      }
  }


  Value& get(const Key& key)
  {
    const size_t hash = hasher(key);
    for (size_t i = 0; i < N; i++)
      {
        size_t pos = (hash + i) % N;
        if (data[pos].created && data[pos].key == key)
          {
            return data[pos].value;
          }
      }
    return nullptr;
  }
};
