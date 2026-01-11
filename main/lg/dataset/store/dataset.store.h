#pragma once

#include "core/state/thread_reactive.h"
#include <lg/dataset/types.h>

namespace ON2Solutions::parser
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