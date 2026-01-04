//
// Created by evgeniy on 1/2/26.
//


#pragma once

#include "core/state/thread_reactive.h"

namespace on2::parser
{
  class DatasetStore final : public foundation::ThreadReactive<Dataset>
  {
  private:
    DatasetStore(DatasetStore &other) = delete;
    void operator=(const DatasetStore &) = delete;

  public:
    DatasetStore() : ThreadReactive<Dataset>() {}

    static DatasetStore* getInstance()
    {
      static DatasetStore instance;
      return &instance;
    }
  };

}