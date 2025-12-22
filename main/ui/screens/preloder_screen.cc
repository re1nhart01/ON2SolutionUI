#include "../../components/foundation/components/component.h"
#include "../../components/foundation/core/application.h"
#include "../../components/foundation/core/shortcuts.h"
#include "ui/styles/theme.h"

class PreloaderScreen;
using namespace foundation;

struct PreloaderScreenProps final
    : BaseProps<PreloaderScreenProps, PreloaderScreen> {};

class PreloaderScreen final : public NavigationScreen<PreloaderScreenProps> {
private:
  PreloaderScreenProps props;
public:
  explicit PreloaderScreen(const std::shared_ptr<StackNavigator> &stack, const PreloaderScreenProps &props) : NavigationScreen(stack, props) {
    this->props = props;
  }

  ~PreloaderScreen() override = default;

  void component_did_mount() override
  {
    ESP_LOGI("preloader_screen", "Preloading screen");
    this->navigate_after();
  }

  void navigate_after() const
  {
    auto* nav_ptr = new std::shared_ptr(this->navigation_ref);

    TimerHandle_t timeout_handle = xTimerCreate(
        "navigate_timer",
        pdMS_TO_TICKS(4000),
        pdFALSE,
        nav_ptr,
        [](TimerHandle_t timer) {
            lv_async_call(
                [](void* data) {
                    auto* navigator =
                        static_cast<std::shared_ptr<StackNavigator>*>(data);

                    ESP_LOGI("preloader_screen", "Preloading screen after timeout callback");
                    (*navigator)->navigate("/main");

                    delete navigator;
                },
                pvTimerGetTimerID(timer)
            );
        }
    );

    xTimerStart(timeout_handle, 0);
  }

  lv_obj_t* render() override {
    NavigationScreen::render();
    auto navigator_ref = this->navigation_ref;

    return this->delegate(
        $View(
            ViewProps::up()
                .set_style(this->styling())
                .set_children(Children{
                    $Text(TextProps::up().value("LOADING...")),
                    $Activity(ActivityIndicatorProps::up().sz(86).arc(80).set_color(PRIMARY_COLOR))
                })
                .w(LV_PCT(100))
                .h(LV_PCT(100))
                .justify(LV_FLEX_ALIGN_CENTER)
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
