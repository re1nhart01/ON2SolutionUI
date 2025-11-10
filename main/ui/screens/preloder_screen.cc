#include "components/application.hh"
#include "components/component.hh"
#include "components/macro.hh"
#include "components/stack_navigator.hh"

using namespace foundation;

struct preloader_screen_props {
  std::shared_ptr<Ref> ref = nullptr;
};

class PreloaderScreen final : public Component {
private:
  preloader_screen_props props;
  std::shared_ptr<StackNavigator> navigator;
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

  void on_mount() override {
    Component::on_mount();
  };


  lv_obj_t *render() override {
    auto navigator_ref = this->navigator;

    auto renderer = $view(
      this->parent,
      view_props{
        .ref = nullptr,
        .style = style,
        .children = {},
        .width = LV_PCT(100),
        .height = LV_PCT(100),
        .justify_content = LV_FLEX_ALIGN_SPACE_BETWEEN,
        .align_items = LV_FLEX_ALIGN_CENTER,
        .track_cross_place = LV_FLEX_ALIGN_CENTER,
        .flex_direction = LV_FLEX_FLOW_COLUMN,
      });

    this->renderer_view = renderer;
    this->renderer_view->set_parent(this->parent);
    this->set_component(renderer->render());

    return renderer->get_component();
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
