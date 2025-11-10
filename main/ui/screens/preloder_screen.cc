#include "components/application.hh"
#include "components/component.hh"
#include "components/macro.hh"
#include "components/stack_navigator.hh"

using namespace foundation;

struct preloader_screen_props {
  std::shared_ptr<Ref> ref = nullptr;
};

int i = 0;

class PreloaderScreen final : public Component {
private:
  preloader_screen_props props;
  std::shared_ptr<StackNavigator> navigator;
  std::shared_ptr<Ref> label_ref = std::make_shared<Ref>("label");
public:
  explicit PreloaderScreen(std::shared_ptr<StackNavigator> stack, const preloader_screen_props &props) : Component(nullptr, nullptr) {
    this->props = props;
    this->navigator = std::move(stack);
    if (this->props.ref != nullptr) {
        this->props.ref->set(this);
    }
  }

  ~PreloaderScreen() override {
    Component::~Component();
  };

  void component_did_mount() override
  {
    ESP_LOGI("preloader_screen", "Preloading screen");
  }


  lv_obj_t *render() override {
      auto navigator_ref = this->navigator;
      auto label_ref = this->label_ref;

      return this->delegate($view(
        this->parent,
        view_props{
          .ref = nullptr,
          .style = style,
          .children = {
            $label(label_props{
              .ref = this->label_ref,
              .text = std::format("{}", i),
            }),
            $button(button_props{
             .ref = nullptr,
             .style = nullptr,
             .text = "mmm",
              .on_click = [this](lv_event_t *e) {
                auto component = dynamic_cast<Label*>(this->label_ref->linked_component);
                if (component) {
                    i++;
                    component->update(std::format("{}", i));

                }
            },
             .on_long_press = [](lv_event_t *e) { /* ... */ },
             .on_pressed = [](lv_event_t *e) { /* ... */ },
             .on_released = [](lv_event_t *e) { /* ... */ },
             .on_focused = [](lv_event_t *e) { /* ... */ },
             .on_defocused = [](lv_event_t *e) { /* ... */ },
           }),
          },
          .width = LV_PCT(100),
          .height = LV_PCT(100),
          .justify_content = LV_FLEX_ALIGN_SPACE_BETWEEN,
          .align_items = LV_FLEX_ALIGN_CENTER,
          .track_cross_place = LV_FLEX_ALIGN_CENTER,
          .flex_direction = LV_FLEX_FLOW_COLUMN,
        })
    );
  }

  std::shared_ptr<Styling> styling() override {
    this->style = std::make_shared<Styling>();

    return this->style;
  }

  PreloaderScreen *append(lv_obj_t *obj) override {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
};
