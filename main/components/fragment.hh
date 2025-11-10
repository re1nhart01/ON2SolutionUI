#pragma once

#include "component.hh"
#include <memory>
#include <string>

namespace foundation {
  class Fragment final : public Component {
  public:
    explicit Fragment();
    ~Fragment() override = default;

    lv_obj_t* render() override;
    std::shared_ptr<Styling> styling() override;
    Fragment* append(lv_obj_t* obj) override;
  };

} // namespace foundation
