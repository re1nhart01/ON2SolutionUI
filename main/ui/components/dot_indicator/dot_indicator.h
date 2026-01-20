#pragma once

#include <core/shortcuts.h>

#include "core/shortcuts.h"
#include "core/styling/styling.h"
#include "ui/localization.h"

#include <lg/helpers/utils.h>
#include <ui/styles/theme.h>

using namespace foundation;
namespace ON2Solutions {
  class DotIndicator;

  struct DotIndicatorProps final
      : foundation::BaseProps<DotIndicatorProps, DotIndicator> {
    uint32_t error_hex = 0;
    short dot_amount = 4;
    short width = 40;
    short height = 10;

    DotIndicatorProps&& set_value_hex(const uint32_t v) {
      error_hex = v;
      return std::move(*this);
    }

    DotIndicatorProps&& set_dot_amount(const short v) {
      dot_amount = v;
      return std::move(*this);
    }

    DotIndicatorProps&& w(const short v) {
      width = v;
      return std::move(*this);
    }

    DotIndicatorProps&& h(const short v) {
      height = v;
      return std::move(*this);
    }
  };

  using $$DotIndicator = std::unique_ptr<DotIndicator>;

  inline $$DotIndicator $DotIndicator(DotIndicatorProps&& args) {
    return std::make_unique<DotIndicator>(
        std::forward<DotIndicatorProps>(args));
  }

  class DotIndicator final : public Component<DotIndicatorProps> {
   private:
    std::vector<View*> dots_ref;

   public:
    using Component::props;

    explicit DotIndicator(DotIndicatorProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->parent = nullptr;
      this->apply_reactive<DotIndicator>(this, this->props.reactive_delegates);
    };

    ~DotIndicator() override {
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty()) {
        this->detach_reactives<DotIndicator>(this, this->props.reactive_link);
      }
    };

    inline std::vector<std::unique_ptr<VNode>> render_rows() {
      Children nodes;
      nodes.reserve(this->props.dot_amount);
      this->dots_ref.clear();

      for (int i = 0; i < this->props.dot_amount; i++) {
        auto dot =
            $View(ViewProps::up().w(8).h(8).set_style([](Styling& style) {
              style.setBorderRadius(100);
              style.setBackgroundColor(lv_color_hex(0x000000));
              style.setBorder(lv_color_make(0, 0, 0), 0, LV_OPA_TRANSP);
              style.setPadding(0, 0, 0, 0);
            }));

        this->dots_ref.push_back(dot.get());
        nodes.push_back(std::move(dot));
      }

      return nodes;
    }

    void do_rebuild() override {
      for (size_t i = 0; i < dots_ref.size(); ++i) {
        bool is_enabled = is_up_bit_pos(this->props.error_hex, i);

        dots_ref[i]->set_state([is_enabled](ViewProps& props) {
          props.set_style([is_enabled](Styling& style) {
            style.setBackgroundColor(is_enabled ? ACTIVE_INDICATOR
                                                : NOT_ACTIVE_INDICATOR);
          });
        });
      }
    };

    lv_obj_t* render() override {
      Component::render();

      return this->delegate(
          $View(ViewProps::up()
                    .w(this->props.width)
                    .h(this->props.height)
                    .set_style([](Styling& style) {
                      style.setGap(4, 4);
                      NoPaddingApply(style);
                    })
                    .set_children(std::move(this->render_rows()))
                    .direction(LV_FLEX_FLOW_ROW_WRAP)
                    .items(LV_FLEX_ALIGN_START)
                    .justify(LV_FLEX_ALIGN_START)
                    .track_cross(LV_FLEX_ALIGN_START)));
    };

    const Styling* styling() const override {
      this->style.setTextColor(lv_color_make(255, 255, 255));
      this->style.setPadding(0, 0, 16, 16);  // Padding T R B L
      this->style.setBorder(lv_color_make(0, 0, 0), 0, 0);
      this->style.setFont(&lv_font_montserrat_12);
      this->style.setBorderRadius(0);

      return &this->style;
    };

    DotIndicator* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };
}  // namespace ON2Solutions