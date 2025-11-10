#include "status_bar.hh"

namespace foundation {

  StatusBar::StatusBar(const status_bar_props& props)
      : Component(nullptr, nullptr), props(props) {
    this->parent = nullptr;

    if (this->props.ref != nullptr) {
        this->props.ref->set(this);
    }
  }

  lv_obj_t* StatusBar::render() {
    std::shared_ptr<Styling> styleV = this->styling();

    this->label = $label(label_props{
                  .ref = nullptr,
                  .style = nullptr,
                  .text = "00:00",
                });

      return this->delegate($view(
          this->parent,
          view_props{
              .ref = nullptr,
              .style = styleV,
              .children = {
                  $fragment,
                  this->label,
                  $fragment,
              },
              .width = LV_PCT(100),
              .height = 24,
              .justify_content = LV_FLEX_ALIGN_SPACE_BETWEEN,
              .align_items = LV_FLEX_ALIGN_CENTER,
              .track_cross_place = LV_FLEX_ALIGN_CENTER,
              .flex_direction = LV_FLEX_FLOW_ROW,
          }));
  }

  std::shared_ptr<Styling> StatusBar::styling() {
    this->style = std::make_shared<Styling>();

    this->style->setBackgroundColor(lv_color_hex(0x303030));
    this->style->setTextColor(lv_color_make(255, 255, 255));
    this->style->setPadding(0, 0, 16, 16);
    this->style->setBorder(lv_color_make(0, 0, 0), 0, 0);
    this->style->setFont(&lv_font_montserrat_12);
    this->style->setBorderRadius(0);

    return this->style;
  }

  void StatusBar::update(const char* value) const {
    lv_label_set_text(this->label->get_component(), value);
  }


  StatusBar* StatusBar::append(lv_obj_t* obj) {
    lv_obj_set_parent(obj, get_component());
    return this;
  }

} // namespace foundation
