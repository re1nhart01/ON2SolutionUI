

#pragma once

#include "components/view/view_props.h"
#include "core/style_store/style_store.h"
#include "lg/dataset/types.h"
#include "theme.h"

//----------------------------------- Shorthands
//------------------------------------------------

namespace ON2Solutions {
  static Styling okStyle = [] {
    Styling s;
    s.setBackgroundColor(NO_ERROR_COLOR);
    return s;
  }();
  static Styling errStyle = [] {
    Styling s;
    s.setBackgroundColor(ERROR_COLOR);
    return s;
  }();

  inline static auto AlarmTextStyleApply = [](Styling& s) {
    s.setTextColor(ERROR_COLOR);
    s.setTextOpacity(255);
  };

  inline static auto DefaultTextStyleApply = [](Styling& s) {
    s.setTextColor(lv_color_white());
    s.setTextOpacity(255);
  };

  inline foundation::ViewProps& screen_container_props(
      foundation::ViewProps& p) {
    p.w(LV_PCT(100));
    p.h(LV_PCT(100));
    p.justify(LV_FLEX_ALIGN_START);
    p.items(LV_FLEX_ALIGN_CENTER);
    p.track_cross(LV_FLEX_ALIGN_CENTER);
    p.direction(LV_FLEX_FLOW_COLUMN);

    return p;
  }

  inline lv_color_t button_color_by_status(parser::DatasetStatuses status) {
    switch (status) {
      case parser::DatasetStatuses::StandBy:
        return STATUS_STANDBY;

      case parser::DatasetStatuses::BoosterDelay:
        return STATUS_BOOSTER_DELAY;

      case parser::DatasetStatuses::Startup:
        return STATUS_STARTUP;

      case parser::DatasetStatuses::Running:
        return STATUS_RUNNING;

      case parser::DatasetStatuses::Producing:
        return STATUS_PRODUCING;

      case parser::DatasetStatuses::Alarm:
        return STATUS_ALARM;

      case parser::DatasetStatuses::TankFull:
        return STATUS_TANK_FULL;

      case parser::DatasetStatuses::AutoAdjusting:
        return STATUS_AUTO_ADJUSTING;

      default:
        return ACTIVE_INDICATOR;
    }
  }

  // ---------------------------- HEADER MAIN ----------------------------

  inline foundation::ViewProps& header_container_props(
      foundation::ViewProps& p) {
    p.w(LV_PCT(100));
    p.h(45);
    p.justify(LV_FLEX_ALIGN_SPACE_BETWEEN);
    p.items(LV_FLEX_ALIGN_CENTER);
    p.track_cross(LV_FLEX_ALIGN_CENTER);
    p.direction(LV_FLEX_FLOW_ROW);

    return p;
  }

  inline foundation::ViewProps& header_container_left_props(
      foundation::ViewProps& p) {
    p.w(LV_PCT(100));
    p.h(45);
    p.justify(LV_FLEX_ALIGN_START);
    p.items(LV_FLEX_ALIGN_START);
    p.track_cross(LV_FLEX_ALIGN_START);
    p.direction(LV_FLEX_FLOW_ROW);

    return p;
  }

  // ---------------------------- HEADER RIGHT ----------------------------

  inline foundation::ViewProps& header_container_right_props(
      foundation::ViewProps& p) {
    p.w(LV_PCT(40));
    p.h(45);
    p.justify(LV_FLEX_ALIGN_END);
    p.items(LV_FLEX_ALIGN_CENTER);
    p.track_cross(LV_FLEX_ALIGN_CENTER);
    p.direction(LV_FLEX_FLOW_ROW);

    return p;
  }

  // ---------------------------- HEADER LABELS ----------------------------

  inline foundation::ViewProps& header_labels_container_props(
      foundation::ViewProps& p) {
    p.w(LV_PCT(60));
    p.h(LV_PCT(100));
    p.justify(LV_FLEX_ALIGN_SPACE_BETWEEN);
    p.items(LV_FLEX_ALIGN_CENTER);
    p.track_cross(LV_FLEX_ALIGN_CENTER);
    p.direction(LV_FLEX_FLOW_ROW);

    return p;
  }

  inline foundation::ViewProps& pincode_screen_sign_form_props(
      foundation::ViewProps& p) {
    p.w(LV_PCT(100))
        .h(LV_PCT(50))
        .justify(LV_FLEX_ALIGN_CENTER)
        .items(LV_FLEX_ALIGN_CENTER)
        .track_cross(LV_FLEX_ALIGN_CENTER)
        .direction(LV_FLEX_FLOW_COLUMN);

    return p;
  }

  inline foundation::ViewProps& centered_row(foundation::ViewProps& p) {
    p.justify(LV_FLEX_ALIGN_CENTER)
        .items(LV_FLEX_ALIGN_CENTER)
        .track_cross(LV_FLEX_ALIGN_CENTER)
        .direction(LV_FLEX_FLOW_ROW);

    return p;
  }

  inline static auto NoPaddingApply = [](Styling& style) {
    style.setBackgroundColor(lv_color_hex(0xffffff));
    style.setBorder(lv_color_make(0, 0, 0), 0, LV_OPA_TRANSP);
    style.setBorderRadius(0);
    style.setPadding(0, 0, 0, 0);
  };

  inline static auto HeaderButtonApply = [](Styling& style) {
    style.setBackgroundColor(lv_color_hex(0x5B5AFF));
    style.setBorderRadius(24);
    style.setBorder(lv_color_hex(0x5B5AFF), 0, 0);
    style.setPadding(8, 8, 16, 16);
  };

  inline static auto HeaderContainerApply = [](Styling& style) {
    style.setPadding(0, 0, 16, 16);
    style.setBorderRadius(0);
    style.setBorder(lv_color_make(255, 255, 255), 0, 0);
  };

  inline static auto HeaderBigLabelApply = [](Styling& style) {
    style.setFont(&lv_font_montserrat_16);
  };

  inline static auto HeaderLabelApply = [](Styling& style) {
    style.setFont(&lv_font_montserrat_14);
  };

  inline static auto LabelPaddedApply = [](Styling& style) {
    style.setFont(&lv_font_montserrat_14);
    style.setPadding(4, 4, 0, 0);
  };

  inline static auto HeaderLabelContainerApply = [](Styling& style) {
    style.setFont(&lv_font_montserrat_14);
    style.setPadding(8, 8, 16, 16);
  };

  inline static auto FooterButtonApply = [](Styling& style) {
    style.setFont(&lv_font_montserrat_16);
    style.setBackgroundColor(button_color_by_status(parser::DatasetStatuses::StandBy));
    style.setBorderRadius(12);
    style.setSize(LV_PCT(100), 45);
    style.setBorder(lv_color_hex(0x5B5AFF), 0, 0);
    style.setPadding(8, 8, 16, 16);
  };


  // inline void style_screen_register(foundation::StyleStorage& storage) {
  //   storage.set("header.button", [](Styling& style) {
  //     style.setBackgroundColor(lv_color_hex(0x5B5AFF));
  //     style.setBorderRadius(24);
  //     style.setBorder(lv_color_hex(0x5B5AFF), 0, 0);
  //     style.setPadding(8, 8, 16, 16);
  //   });
  //   storage.set("header.container", [](Styling& style) {
  //     style.setPadding(0, 0, 16, 16);
  //     style.setBorderRadius(0);
  //     style.setBorder(lv_color_make(255, 255, 255), 0, 0);
  //   });
  //   storage.set("header.label",
  //               [](Styling& style) { style.setFont(&lv_font_montserrat_12); });
  //
  //   storage.set("header.labels.container", [](Styling& style) {
  //     style.setFont(&lv_font_montserrat_12);
  //     style.setPadding(8, 8, 16, 16);
  //   });
  //
  //   storage.set("footer.button", [](Styling& style) {
  //     style.setFont(&lv_font_montserrat_16);
  //     style.setBackgroundColor(lv_color_hex(0x000000));
  //     style.setBorderRadius(12);
  //     style.setSize(LV_PCT(100), 45);
  //     style.setBorder(lv_color_hex(0x5B5AFF), 0, 0);
  //     style.setPadding(8, 8, 16, 16);
  //   });
  //
  //   storage.set("common.no_padding", [](Styling& style) {
  //     style.setBackgroundColor(lv_color_hex(0xffffff));
  //     style.setBorder(lv_color_make(0, 0, 0), 0, LV_OPA_TRANSP);
  //     style.setBorderRadius(0);
  //     style.setPadding(0, 0, 0, 0);
  //   });
  //
  //   storage.set("footer.container", [](Styling& style) {
  //     // style.setPadding(0, 0, 16, 16);
  //     // style.setBorderRadius(0);
  //     // style.setBorder(lv_color_make(255, 255, 255), 0, 0);
  //   });
  // }
}  // namespace ON2Solutions