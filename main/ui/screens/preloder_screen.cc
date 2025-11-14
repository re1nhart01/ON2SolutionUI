#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/macro.h"

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


  lv_obj_t* render() override {
    auto navigator_ref = this->navigator;

    return this->delegate(
        $View(
            ViewProps::up()
                .set_style(this->styling())
                .set_children({

                    $Text(
                        TextProps::up()
                            .set_ref(this->label_ref)
                            .value(std::format("{}", i))
                    ),

                    $Button(
                        ButtonProps::up()
                            .label("mmm")
                            .click([this](lv_event_t* e) {
                                auto component = dynamic_cast<Text*>(this->label_ref->linked_component);
                                if (component) {
                                    component->set_state([component]() {
                                        i++;
                                    });
                                }
                            })
                    )

                })
                .w(LV_PCT(100))
                .h(LV_PCT(100))
                .justify(LV_FLEX_ALIGN_SPACE_BETWEEN)
                .items(LV_FLEX_ALIGN_CENTER)
                .track_cross(LV_FLEX_ALIGN_CENTER)
                .direction(LV_FLEX_FLOW_COLUMN)
        )
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
