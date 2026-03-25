#pragma once

#include <core/shortcuts.h>
#include <lg/dataset/store/dataset.store.h>

#include <memory>
#include "core/styling/styling.h"
#include "ui/components/dot_indicator/dot_indicator.h"

#include <constants/assets.h>
#include "constants/theme.h"

using namespace foundation;
using namespace ON2Solutions::parser;
namespace ON2Solutions {
  class CommonHeader;

  struct CommonHeaderProps final
      : foundation::BaseProps<CommonHeaderProps, CommonHeader> {};

  class CommonHeader final : public Component<CommonHeaderProps> {
   public:
    using Component::props;

    explicit CommonHeader(CommonHeaderProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->parent = nullptr;
    };

    ~CommonHeader() override {
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
    };

    lv_obj_t* render() override {
      Component::render();

      return this->delegate($View(
          ViewProps::up()
              .w(LV_PCT(100))
              .h(56)
              .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
              .items(LV_FLEX_ALIGN_CENTER)
              .track_cross(LV_FLEX_ALIGN_CENTER)
              .direction(LV_FLEX_FLOW_ROW)
              .set_style(CommonHeaderWrapperApply)
              .set_children(children(
                  $Image(ImageProps::up()
                             .source(assets::LogoSmall)
                             .width(54)
                             .height(32)),

                  $View(
                      ViewProps::up()
                          .w(320)
                          .h(LV_PCT(100))
                          .direction(LV_FLEX_FLOW_ROW)
                          .items(LV_FLEX_ALIGN_CENTER)
                          .track_cross(LV_FLEX_ALIGN_CENTER)
                          .set_style(NoPaddingApply)
                          .set_children(children(
                              $View(
                                  ViewProps::up()
                                      .w(LV_PCT(40))
                                      .h(LV_PCT(100))
                                      .direction(LV_FLEX_FLOW_ROW)
                                      .items(LV_FLEX_ALIGN_CENTER)
                                      .track_cross(LV_FLEX_ALIGN_CENTER)
                                      .set_style([](Styling& s) {
                                        s.setGap(8, 8);
                                        NoPaddingApply(s);
                                      })
                                      .set_children(children(
                                          $Text(
                                              TextProps::up().value("Inputs:")),
                                          $DotIndicator(
                                              DotIndicatorProps::up()
                                                  .set_dot_amount(8)
                                                  .w(50)
                                                  .h(20)
                                                  .watch<Dataset>(
                                                      DatasetStore::
                                                          getInstance(),
                                                      "inputs",
                                                      [](DotIndicator* self,
                                                         const Dataset& value) {
                                                        self->set_state(
                                                            [v = value
                                                                     .operative_data
                                                                     .inputs](
                                                                DotIndicatorProps&
                                                                    p) {
                                                              p.set_value_hex(
                                                                  v);
                                                            });
                                                      }))))),
                              $View(
                                  ViewProps::up()
                                      .direction(LV_FLEX_FLOW_ROW)
                                      .items(LV_FLEX_ALIGN_CENTER)
                                      .track_cross(LV_FLEX_ALIGN_CENTER)
                                      .w(LV_PCT(60))
                                      .h(LV_PCT(100))
                                      .set_style([](Styling& s) {
                                        s.setGap(8, 8);
                                        NoPaddingApply(s);
                                      })
                                      .set_children(children(
                                          $Text(TextProps::up().value(
                                              "Outputs:")),
                                          $DotIndicator(
                                              DotIndicatorProps::up()
                                                  .set_dot_amount(16)
                                                  .w(100)
                                                  .h(20)
                                                  .watch<Dataset>(
                                                      DatasetStore::
                                                          getInstance(),
                                                      "outputs",
                                                      [](DotIndicator* self,
                                                         const Dataset& value) {
                                                        self->set_state(
                                                            [v = value
                                                                     .operative_data
                                                                     .outputs](
                                                                DotIndicatorProps&
                                                                    p) {
                                                              p.set_value_hex(
                                                                  v);
                                                            });
                                                      }))))))))))));
    };

    const Styling* styling() const override {
      this->style.setTextColor(lv_color_make(255, 255, 255));
      this->style.setPadding(0, 0, 16, 16);  // Padding T R B L
      this->style.setBorder(lv_color_make(0, 0, 0), 0, 0);
      this->style.setFont(&lv_font_montserrat_12);
      this->style.setBorderRadius(0);

      return &this->style;
    };

    CommonHeader* append(lv_obj_t* obj) override {
      lv_obj_set_parent(obj, get_component());
      return this;
    };
  };

  using $$CommonHeader = std::unique_ptr<CommonHeader>;

  inline $$CommonHeader $CommonHeader(CommonHeaderProps&& args) {
    return std::make_unique<CommonHeader>(
        std::forward<CommonHeaderProps>(args));
  }
}  // namespace ON2Solutions