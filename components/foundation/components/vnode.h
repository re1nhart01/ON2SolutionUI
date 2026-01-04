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

    mutable Styling style{};
    std::shared_ptr<VNode> renderer_view = nullptr;

  public:
    virtual ~VNode()
    {
      VNode::component_will_unmount();
    }

    VNode() {}
    VNode(lv_obj_t* obj, lv_obj_t* parent) : component(obj), parent(parent) {}

    virtual void component_did_mount()    {  }
    virtual void component_will_unmount() {  }
    virtual void component_did_update()   {  }
    virtual void component_did_rebuild()  {  }

    virtual lv_obj_t* render() { component_did_mount(); return nullptr; }
    virtual void do_rebuild() {}

    virtual void apply_base_style(Styling& s) const {}
    virtual const Styling* styling() const { return &this->style; };
    virtual VNode* append(lv_obj_t* obj) = 0;

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

    template<typename T>
    void detach_reactives(T* instance, std::vector<IReactive*>& states)
    {
      for (auto* state : states) {
          if (state) state->detach(instance);
      }
      states.clear();
    }

    template<typename T>
    void apply_reactive(T* instance, const std::vector<Delegate<void(void*)>>& delegates) {
      for (auto& binder : delegates) {
          if (binder) binder(static_cast<void*>(instance));
      }
    }

    template <typename Fn>
    void set_style(Fn fn) const {
      fn(style);
      if (component)
      {
        lv_obj_add_style(component, style.getStyle(), 0);
      }
    }

    void forceUpdate() {
      if (this->get_component() != nullptr) {
        lv_obj_invalidate(this->get_component());
        rebuild();
      }
      component_did_update();
    }

    void rebuild() {
      auto obj = this->get_component();
      if (obj != nullptr)
      {
        do_rebuild();
      }
    }

    void UNSAFE_repainting() {
      if (component) {
        lv_obj_del(component);
        component = nullptr;
      }
      render();
      component_did_rebuild();
    }

    lv_obj_t* delegate(const std::shared_ptr<VNode>& component_view) {
      this->renderer_view = component_view;
      this->renderer_view->set_parent(this->parent);
      this->set_component(this->renderer_view->render());
      return this->renderer_view->get_component();
    }

    bool is_ready() const { return component != nullptr; }
  };

}
