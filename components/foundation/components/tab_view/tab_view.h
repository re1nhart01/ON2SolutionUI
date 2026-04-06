#pragma once

#include "components/view/view.h"
#include "tab_view_props.h"

namespace foundation {
  class TabView final : public Component<TabViewProps> {
   public:
    using Component::props;

    std::vector<lv_obj_t*> tab_pages;
    mutable Styling btn_style{};
    lv_obj_t* label_obj;
    std::vector<lv_obj_t*> tab_buttons;
    int last_index = 0;

    explicit TabView(TabViewProps&& props)
        : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<TabView>(this, this->props.reactive_delegates);
      if (this->props.ref != nullptr) {
        this->props.ref->set(this);
      }
    };

    ~TabView() override {
      if (this->props.ref != nullptr) {
        this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty()) {
        this->detach_reactives<TabView>(this, this->props.reactive_link);
      }
    };

    void update_tab_styles() {
      if (props.btn_style_override) {
        props.btn_style_override(btn_style);
      }

      for (size_t i = 0; i < tab_buttons.size(); i++) {
        lv_obj_t* btn = tab_buttons[i];

        lv_obj_add_style(btn, btn_style.getStyle(), LV_PART_MAIN);

        if (btn_style.width > 0) {
          lv_obj_set_width(btn, btn_style.width);
        }

        if (btn_style.height > 0) {
          lv_obj_set_height(btn, btn_style.height);
        }

        if (i == last_index) {
          lv_obj_set_style_bg_color(btn, btn_style.checked_bg, 0);
          lv_obj_set_style_text_color(btn, lv_color_hex(0xFFFFFF), 0);
        } else {
          lv_obj_set_style_bg_color(btn, lv_color_hex(0xffffff), 0);
          lv_obj_set_style_text_color(btn, lv_color_hex(0x000000), 0);
        }
      }
    }

    lv_obj_t* render() override {
      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj)
        return nullptr;

      // 🔥 root
      lv_obj_t* root = lv_obj_create(parent_obj);
      lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
      lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
      lv_obj_set_style_pad_all(root, 0, 0);
      lv_obj_set_style_border_width(root, 0, 0);
      lv_obj_set_style_bg_opa(root, LV_OPA_TRANSP, 0);

      // 🔥 header
      lv_obj_t* header = lv_obj_create(root);
      lv_obj_set_width(header, LV_PCT(100));
      lv_obj_set_height(header, props.header_size);

      lv_obj_set_flex_flow(header, LV_FLEX_FLOW_ROW);
      lv_obj_set_flex_align(header, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                            LV_FLEX_ALIGN_CENTER);

      lv_obj_set_style_pad_all(header, 0, 0);
      lv_obj_set_style_border_width(header, 0, 0);
      lv_obj_set_style_bg_opa(header, LV_OPA_TRANSP, 0);

      if (props.tab_label != nullptr) {
        this->label_obj = lv_label_create(header);
        lv_label_set_text(this->label_obj, props.tab_label);

        lv_obj_set_flex_grow(this->label_obj, 1);

        lv_obj_set_style_text_align(this->label_obj, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_style_pad_left(this->label_obj, 15, 0);
      }

      lv_obj_t* buttons_container = lv_obj_create(header);

      lv_obj_set_flex_grow(buttons_container, 0);
      lv_obj_set_width(buttons_container, LV_SIZE_CONTENT);
      lv_obj_set_height(buttons_container, LV_PCT(100));

      lv_obj_set_flex_flow(buttons_container, LV_FLEX_FLOW_ROW);
      lv_obj_set_flex_align(buttons_container, LV_FLEX_ALIGN_START,
                            LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

      lv_obj_set_style_bg_color(buttons_container, lv_color_hex(0xffffff), 0);
      lv_obj_set_style_bg_opa(buttons_container, LV_OPA_COVER, 0);
      lv_obj_set_style_border_width(buttons_container, 0, 0);
      lv_obj_set_style_pad_all(buttons_container, 0, 0);
      lv_obj_set_style_pad_column(buttons_container, 0, 0);
      lv_obj_set_style_radius(buttons_container, 10, 0);

      lv_obj_t* tv = lv_tabview_create(root, props.tab_pos, props.header_size);
      this->set_component(tv);

      lv_obj_set_flex_grow(tv, 1);

      lv_obj_t* tab_btns = lv_tabview_get_tab_btns(tv);
      lv_obj_add_flag(tab_btns, LV_OBJ_FLAG_HIDDEN);

      if (props.tabs.size() != props.childrens.size())
        return nullptr;

      tab_buttons.clear();

      for (size_t i = 0; i < props.tabs.size(); i++) {
        lv_obj_t* btn = lv_btn_create(buttons_container);

        tab_buttons.push_back(btn);

        if (btn_style.width > 0) {
          lv_obj_set_width(btn, btn_style.width);
        }

        lv_obj_set_height(btn, props.header_size - 6);

        lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);

        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, props.tabs[i].c_str());
        lv_obj_center(label);

        lv_obj_set_user_data(btn, (void*)i);

        lv_obj_add_event_cb(
            btn,
            [](lv_event_t* e) {
              auto self = static_cast<TabView*>(lv_event_get_user_data(e));

              lv_obj_t* btn = lv_event_get_target(e);
              uint32_t index = (uint32_t)lv_obj_get_user_data(btn);

              self->last_index = index;

              lv_tabview_set_act(self->get_component(), index, LV_ANIM_OFF);

              self->update_tab_styles();

              if (self->props.on_change_cb) {
                self->props.on_change_cb(index);
              }
            },
            LV_EVENT_CLICKED, this);
      }

      tab_pages.clear();
      for (size_t i = 0; i < props.tabs.size(); i++) {
        lv_obj_t* tab = lv_tabview_add_tab(tv, props.tabs[i].c_str());

        if (props.childrens[i]) {
          props.childrens[i]->set_parent(tab);
          props.childrens[i]->render();
        }

        tab_pages.push_back(tab);
        lv_obj_set_style_pad_all(tab, 0, 0);
      }

      update_tab_styles();

      lv_obj_clear_flag(lv_tabview_get_content(tv), LV_OBJ_FLAG_SCROLLABLE);

      this->set_active(this->props.is_visible);

      return root;
    }

    void do_rebuild() override {

    };

    void set_tab(uint16_t index, lv_anim_enable_t anim = LV_ANIM_ON) const {
      lv_tabview_set_act(get_component(), index, anim);
    }

    lv_obj_t* get_tab(size_t index) const {
      if (index < tab_pages.size()) {
        return tab_pages[index];
      }
      return nullptr;
    }

    const Styling* styling() const override {
      apply_base_style(style);

      if (props.style_override) {
        props.style_override(style);
      }

      return &style;
    }

    TabView* append(lv_obj_t* obj) override {
      uint16_t act = lv_tabview_get_tab_act(get_component());
      lv_obj_set_parent(obj, get_tab(act));
      return this;
    };
  };
}  // namespace foundation