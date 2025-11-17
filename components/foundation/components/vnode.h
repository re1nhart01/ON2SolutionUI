#pragma once

#include "core/ref/ref.h"
#include "core/styling/styling.h"

#include <memory>
#include <esp_log.h>

namespace foundation {
  class VNode {
  protected:
    lv_obj_t* component = nullptr;
    lv_obj_t* parent = nullptr;

    std::shared_ptr<Styling> style = nullptr;
    std::shared_ptr<VNode> renderer_view = nullptr;

  public:
    virtual ~VNode() { VNode::component_will_unmount(); }

    VNode() {
      this->style = std::make_shared<Styling>();
    }

    VNode(lv_obj_t* obj, lv_obj_t* parent)
      : component(obj), parent(parent) {
      this->style = std::make_shared<Styling>();
    }

    // ---- Lifecycle ----
    virtual void component_did_mount()    { ESP_LOGI("VNode", "componentDidMount"); }
    virtual void component_will_unmount() { ESP_LOGI("VNode", "componentWillUnmount"); }
    virtual void component_did_update()   { ESP_LOGI("VNode", "componentDidUpdate"); }
    virtual void component_did_rebuild()  { ESP_LOGI("VNode", "componentDidRebuild"); }

    // ---- Rendering ----
    virtual lv_obj_t* render() { component_did_mount(); return nullptr; }
    virtual void do_rebuild() {}

    // ---- Pure virtual ----
    virtual std::shared_ptr<Styling> styling() = 0;
    virtual VNode* append(lv_obj_t* obj) = 0;

    // ---- utils ----
    lv_obj_t* get_component() const { return component; }
    lv_obj_t* get_parent() const { return parent; }

    void set_parent(lv_obj_t* p) { parent = p; }

    lv_obj_t* set_component(lv_obj_t* v) {
      component = v;
      return v;
    }

    void set_active(bool active) const {
      if (!component) return;
      if (active) lv_obj_clear_flag(component, LV_OBJ_FLAG_HIDDEN);
      else        lv_obj_add_flag(component, LV_OBJ_FLAG_HIDDEN);
    }

    void set_style(const std::shared_ptr<Styling>& newStyle) {
      style = newStyle;
      if (component && style)
        lv_obj_add_style(component, style->getStyle(), 0);
    }

    void forceUpdate() {
      if (component) {
        lv_obj_invalidate(component);
        rebuild();
      }
      component_did_update();
    }

    void rebuild() {
      if (component && style)
        do_rebuild();
    }

    void UNSAFE_repainting() {
      if (component) {
        lv_obj_del(component);
        component = nullptr;
      }
      render();
      component_did_rebuild();
    }

    // Delegate foundation → LVGL
    lv_obj_t* delegate(const std::shared_ptr<VNode>& component_view) {
      this->renderer_view = component_view;
      this->renderer_view->set_parent(this->parent);
      this->set_component(this->renderer_view->render());
      return this->renderer_view->get_component();
    }

    bool is_ready() const { return component != nullptr; }
  };

}
