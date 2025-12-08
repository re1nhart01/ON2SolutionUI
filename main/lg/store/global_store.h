//
// Created by evgeniy on 12/8/25.
//

#ifndef GLOBAL_STORE_H
#define GLOBAL_STORE_H

#endif //GLOBAL_STORE_H


class GlobalStore
{
private:
  static GlobalStore *instance;
  foundation::State<uint32_t, 8> moto_hours_state;

  GlobalStore(const GlobalStore&) = delete;
  GlobalStore& operator=(const GlobalStore&) = delete;
  public:

  explicit GlobalStore() : moto_hours_state(foundation::State<uint32_t, 8>(0)) {};
  ~GlobalStore() = default;

  static GlobalStore* getInstance() {
    if (instance == nullptr) {
        instance = new GlobalStore();
    }
    return instance;
  }

  foundation::State<uint32_t, 8>* getMotoHoursState()
  {
    return &moto_hours_state;
  }

};

GlobalStore* GlobalStore::instance = nullptr;