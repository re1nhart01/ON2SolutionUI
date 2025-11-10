#include "scroll_view.hh"

namespace foundation {
  ScrollView::ScrollView(lv_obj_t* parent, const scroll_view_props& props)
      : Component(this->create_initial(parent), parent), props(props) {
    set_style(props.style);

    if (!props.children.empty()) {
        this->children.insert(this->children.end(), props.children.begin(), props.children.end());
    }

    if (this->props.ref != nullptr) {
        this->props.ref->set(this);
    }
  }

  lv_obj_t* ScrollView::render() {
    Component::render();

    if (get_component() == nullptr || this->parent == nullptr) {
        this->set_component(this->create_initial(this->parent));
    }
    lv_obj_t* comp = get_component();

    lv_obj_set_layout(comp, LV_LAYOUT_FLEX);
    lv_obj_set_size(comp, props.width, props.height);
    lv_obj_set_flex_flow(comp, props.flex_direction);
    lv_obj_set_flex_align(comp, props.justify_content, props.align_items, props.track_cross_place);

    lv_obj_set_scroll_dir(comp, props.horizontal ? LV_DIR_HOR : LV_DIR_VER);
    lv_obj_set_scrollbar_mode(comp, props.disabled ? LV_SCROLLBAR_MODE_OFF : LV_SCROLLBAR_MODE_ACTIVE);

    std::shared_ptr<Styling> style = this->styling();

    for (const auto& child : this->children) {
        if (child != nullptr) {
            child->set_active(true);
            child->set_parent(comp);
            child->render();
            lv_obj_set_parent(child->get_component(), comp);
        }
    }

    if (style != nullptr) {
        lv_obj_add_style(this->get_component(), style->getStyle(), LV_PART_MAIN);
    }

    return comp;
  }

  std::shared_ptr<Styling> ScrollView::styling() {
    if (this->props.style) {
        return this->props.style;
    }
    return {};
  }

  ScrollView* ScrollView::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }

  lv_obj_t* ScrollView::create_initial(lv_obj_t* parental) {
    return lv_obj_create(parental);
  }

} // namespace foundation
