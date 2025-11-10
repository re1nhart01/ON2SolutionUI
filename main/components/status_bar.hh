#pragma once

#include "component.hh"
#include "label.hh"
#include "macro.hh"
#include "ref.hh"
#include "styling.hh"
#include "view.hh"

#include <memory>

namespace foundation {

  struct status_bar_props {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style;
  };

  class StatusBar final : public Component {
  public:
    status_bar_props props;
    std::shared_ptr<Label> label = nullptr;
    explicit StatusBar(const status_bar_props& props);
    ~StatusBar() override = default;

    lv_obj_t* render() override;
    std::shared_ptr<Styling> styling() override;
    StatusBar* append(lv_obj_t* obj) override;

    void update(const char* value) const;
  };

} // namespace foundation
