#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"

class PreloaderScreen;
using namespace foundation;

struct PreloaderScreenProps final
    : BaseProps<PreloaderScreenProps, PreloaderScreen> {};

int i = 0;
bool flag = false;

class PreloaderScreen final : public NavigationScreen<PreloaderScreenProps> {
private:
  PreloaderScreenProps props;
  std::shared_ptr<Ref<Text>> label_ref = std::make_shared<Ref<Text>>("label");
  std::shared_ptr<Ref<View>> view_ref = std::make_shared<Ref<View>>("zxc");
public:
  explicit PreloaderScreen(const std::shared_ptr<StackNavigator> &stack, const PreloaderScreenProps &props) : NavigationScreen(stack, props) {
    this->props = props;
  }

  ~PreloaderScreen() override = default;

  void component_did_mount() override
  {
    ESP_LOGI("preloader_screen", "Preloading screen");
  }


  lv_obj_t* render() override {
    auto navigator_ref = this->navigation_ref;

    return this->delegate(
        $View(
            ViewProps::up()
                .set_style(this->styling())
                .set_children(Children{
                    $View(ViewProps::up().w(600).h(240).set_ref(this->view_ref).set_children(Children{})),

                    $Text(
                        TextProps::up()
                            .set_ref(this->label_ref)
                            .value(std::format("count: {}", i))
                    ),

                    $Button(
                        ButtonProps::up()
                            .label("mmm")
                            .click([this](lv_event_t* e) {
                              auto component_f = this->label_ref->get();
                                auto component_g = this->view_ref->get();

                                component_f->set_state([component_f](TextProps& props) {
                                  auto tex = props.text.c_str();

                                    ESP_LOGI("problem", "%s", tex);
                                    props.text = "count: " + std::to_string(i++);
                                });

                              component_g->set_state([](ViewProps& p, VNode* v) {
                                  const auto self = dynamic_cast<View*>(v);

                                  if (flag)
                                  {
                                    p.children.push_back($Text(TextProps::up().value(std::format("Hello", i))));
                                  }
                                  else
                                  {
                                      if (!p.children.empty()) {
                                          p.children.pop_back();
                                      }
                                  }

                                  flag = !flag;

                                  self->refresh_childrens();
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
