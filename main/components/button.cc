#include "button.hh"

namespace foundation {

static void event_adapter(lv_event_t *event);

Button::Button(const button_props &props)
  : Component(nullptr, nullptr), props(props) {
  if (this->props.ref != nullptr) {
    this->props.ref->set(this);
  }
}

lv_obj_t *Button::render() {
  Component::render();
  lv_obj_t *parent_obj = this->get_parent();
  if (!parent_obj) return nullptr;

  set_component(lv_btn_create(parent_obj));

  auto *obj = this->get_component();

  lv_obj_set_layout(obj, LV_LAYOUT_FLEX);
  lv_obj_set_flex_flow(obj, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(obj, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER,
                        LV_FLEX_ALIGN_CENTER);

  if (this->props.child != nullptr) {
    this->props.child->set_parent(obj);
    this->props.child->set_active(true);
    this->props.child->render();
  }

  if (!this->props.text.empty()) {
    this->label = lv_label_create(obj);
    lv_label_set_text(this->label, props.text.c_str());
  }

  if (const auto style = this->styling(); style != nullptr) {
      lv_obj_add_style(obj, style->getStyle(), LV_PART_MAIN);
  }

  lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);

  lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_CLICKED, this);
  lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_PRESSED, this);
  lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_RELEASED, this);
  lv_obj_add_event_cb(obj, event_adapter, LV_EVENT_LONG_PRESSED, this);

  return obj;
}

  std::shared_ptr<Styling> Button::styling() {
  if (this->props.style) {
      return this->props.style;
  }
  return {};
}

Button *Button::append(lv_obj_t *obj) {
  lv_obj_set_parent(obj, get_component());
  return this;
}

static void event_adapter(lv_event_t *event) {
  auto *instance = static_cast<Button *>(lv_event_get_user_data(event));
  if (!instance) return;

  const auto &events = instance->props;

  switch (event->code) {
  case LV_EVENT_CLICKED:
    if (events.on_click) events.on_click(event);
    break;
  case LV_EVENT_LONG_PRESSED:
    if (events.on_long_press) events.on_long_press(event);
    break;
  case LV_EVENT_PRESSED:
    if (events.on_pressed) events.on_pressed(event);
    break;
  case LV_EVENT_RELEASED:
    if (events.on_released) events.on_released(event);
    break;
  case LV_EVENT_FOCUSED:
    if (events.on_focused) events.on_focused(event);
    break;
  case LV_EVENT_DEFOCUSED:
    if (events.on_defocused) events.on_defocused(event);
    break;
  default:
    ESP_LOGW("button", "Unknown event code %d", event->code);
  }
}

}
