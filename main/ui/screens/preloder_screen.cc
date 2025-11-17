#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"

class PreloaderScreen;
using namespace foundation;

struct PreloaderScreenProps : BaseProps<PreloaderScreenProps, PreloaderScreen> {};

int i = 0;

class PreloaderScreen final : public Component<PreloaderScreenProps> {
private:
  PreloaderScreenProps props;
  std::shared_ptr<StackNavigator> navigator;
  std::shared_ptr<Ref<Text>> label_ref = std::make_shared<Ref<Text>>("label");
public:
  explicit PreloaderScreen(std::shared_ptr<StackNavigator> stack, const PreloaderScreenProps &props) : Component(nullptr, nullptr, props) {
    this->props = props;
    this->navigator = std::move(stack);
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
                .set_children(Children{

                    $Text(
                        TextProps::up()
                            .set_ref(this->label_ref)
                            .value(std::format("count: {}", i))
                    ),

                    $Button(
                        ButtonProps::up()
                            .label("mmm")
                            .click([this](lv_event_t* e) {
                                auto component = this->label_ref->get();
                                component->set_state([component](TextProps& props) {
                                    props.text = "count: " + std::to_string(i++);
                                });
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
