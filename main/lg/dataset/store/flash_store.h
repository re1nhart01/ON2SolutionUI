#pragma once

#include "core/state/flash_reader.hh"

namespace ON2Solutions::parser
{
  class FlashStore final : public foundation::FlashReader
  {
  private:
    FlashStore(FlashStore &other) = delete;
    void operator=(const FlashStore &) = delete;

  public:
    FlashStore() :  FlashReader("core_namespace") {}

    void init() {
      this->get_value<std::string>(TEMPERATURE_UNIT, "Celsius");
    }

    static FlashStore* getInstance()
    {
      static FlashStore instance;
      return &instance;
    }
  };

}