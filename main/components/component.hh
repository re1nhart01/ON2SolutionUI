//
// Created by evgeniy on 3/29/25.
//
#pragma once

#include <memory>
#include "ref.hh"
#include "styling.hh"
#include <esp_log.h>

extern "C" {
}

namespace foundation {
  class Component {
    protected:
        std::shared_ptr<Styling> style;
        lv_obj_t* component = nullptr;
        std::shared_ptr<Component> renderer_view = nullptr;
        lv_obj_t* parent = nullptr;
        Ref* ref = nullptr;

    public:
        virtual ~Component() { Component::component_will_unmount(); };

        Component() {
            this->style = std::make_shared<Styling>();
        }

        explicit Component(lv_obj_t* obj, lv_obj_t* parent)
            : component(obj), parent(parent) {
            this->style = std::make_shared<Styling>();

        }

        virtual void component_did_mount()
        {
          ESP_LOGI("VNode", "componentDidMount");
        }
        virtual void component_will_unmount()
        {
          ESP_LOGI("VNode", "componentWillUnmount");
        }
        virtual void component_did_update()
        {
          ESP_LOGI("VNode", "componentDidUpdate");
        }
        virtual void component_did_rebuild()
        {
          ESP_LOGI("VNode", "componentDidRebuild");
        }

        virtual lv_obj_t* render()
        {
          component_did_mount();
          return nullptr;
        };

        virtual void do_rebuild() = 0;

        virtual std::shared_ptr<Styling> styling() = 0;
        virtual Component* append(lv_obj_t* obj) = 0;

        lv_obj_t* get_component() const {
            return this->component;
        }

        lv_obj_t* set_component(lv_obj_t* v) {
            this->component = v;
            return this->component;
        }

        lv_obj_t* get_parent() const {
            return this->parent;
        }

        void set_parent(lv_obj_t* parent_link) {
            this->parent = parent_link;
        }

        void set_active(bool active) const {
            if (this->component != nullptr) {
                if (active)
                    lv_obj_clear_flag(this->component, LV_OBJ_FLAG_HIDDEN);
                else
                    lv_obj_add_flag(this->component, LV_OBJ_FLAG_HIDDEN);
            }
        }

        void set_style(const std::shared_ptr<Styling>& newStyle) {
            this->style = newStyle;
            if (this->component && this->style) {
                lv_obj_add_style(this->component, this->style->getStyle(), 0);
            }
        }

        void forceUpdate() {
          if (component) {
              lv_obj_invalidate(component);
              this->rebuild();
          }

          this->component_did_update();
        }

        void rebuild() {
          if (this->component && this->style)
            {
              this->do_rebuild();
            }
        }

        void UNSAFE_repainting() {
          if (this->component) {
              lv_obj_del(this->component);
              this->component = nullptr;
          }

          this->render();
          this->component_did_rebuild();
        }

        void set_state(const std::function<void()> callback, std::vector<Component*>& nodes) {
          try {
            callback();
          } catch (const std::runtime_error& e) {}

          for (Component* node : nodes)
            {
              if (node != nullptr)
                {
                  node->forceUpdate();
                }
            }
        }

        void set_state(std::function<void()> callback) {
          try {
              callback();
          } catch (const std::runtime_error& e) {}
          this->forceUpdate();
        }

        /*
         *  @delegate
         *  @arg1 = std::shared_ptr<Component>&
         *  this method uses only if you want to pass foundation component (not internal lvgl)
         *
         */
        lv_obj_t* delegate(const std::shared_ptr<Component>& component_view) {
          this->renderer_view = component_view;

          this->renderer_view->set_parent(this->parent);
          this->set_component(this->renderer_view->render());

          return this->renderer_view->get_component();
        }

        std::shared_ptr<Styling> get_style() const {
            return this->style;
        }

        bool is_ready() const {
            return this->component != nullptr;
        }
    };
}
