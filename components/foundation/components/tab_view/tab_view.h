#pragma once

#include "tab_view_props.h"
#include "components/view/view.h"

namespace foundation
{
  class TabView final : public Component<TabViewProps> {
  public:
    using Component::props;
    
    std::vector<lv_obj_t*> tab_pages;

    explicit TabView(TabViewProps&& props) : Component(nullptr, nullptr, std::move(props)) {
      this->apply_reactive<TabView>(this, this->props.reactive_delegates);
      if (this->props.ref != nullptr) {
          this->props.ref->set(this);
      }
    };

    ~TabView() override {
      if (this->props.ref != nullptr) {
          this->props.ref->unlink();
      }
      if (!this->props.reactive_link.empty())
      {
        this->detach_reactives<TabView>(this, this->props.reactive_link);
      }
    };

    lv_obj_t* render() override {
      lv_obj_t* parent_obj = this->get_parent();
      if (!parent_obj) return nullptr;

      lv_obj_t* tv = lv_tabview_create(parent_obj, props.tab_pos, props.header_size);
      this->set_component(tv);

      lv_obj_t* tab_btns = lv_tabview_get_tab_btns(tv);
      lv_obj_set_style_bg_opa(tab_btns, LV_OPA_TRANSP, 0);

      if (props.tabs.size() != props.childrens.size())
      {
        return nullptr;
      }

      tab_pages.clear();
      for (int in = 0; in < props.tabs.size(); in++) {

          auto name = props.tabs[in];
          VNode* component = props.childrens[in].get();

          if (component != nullptr)
          {
            lv_obj_t* tab = lv_tabview_add_tab(tv, name.c_str());
            component->set_parent(tab);
            component->render();
            tab_pages.push_back(tab);
            lv_obj_set_style_pad_all(tab, 0, 0);
          }

      }

      lv_obj_clear_flag(lv_tabview_get_content(tv), LV_OBJ_FLAG_SCROLLABLE);

      lv_obj_add_event_cb(tv, [](lv_event_t* e) {
          const auto self = static_cast<TabView*>(lv_event_get_user_data(e));
          lv_obj_t* obj = lv_event_get_target(e);
          uint16_t id = lv_tabview_get_tab_act(obj);
          
          if (self->props.on_change_cb) {
              self->props.on_change_cb(id);
          }
      }, LV_EVENT_VALUE_CHANGED, this);

      this->set_active(this->props.is_visible);
      return tv;
    }

    void set_tab(uint16_t index, lv_anim_enable_t anim = LV_ANIM_ON) const
    {
      lv_tabview_set_act(get_component(), index, anim);
    }

    lv_obj_t* get_tab(size_t index) const
    {
      if (index < tab_pages.size()) {
          return tab_pages[index];
      }
      return nullptr;
    }

    const Styling* styling() const override
    {
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
}