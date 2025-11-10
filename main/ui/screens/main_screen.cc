#include <utility>

#include "components/component.hh"

#include "components/macro.hh"
#include "components/references.hh"
#include "components/stack_navigator.hh"
#include "components/state.hh"
#include "components/style_store.hh"
#include "ui/styles/main_screen_styles.cc"
#include "ui/localization.hh"

using namespace foundation;

struct main_screen_props {
  std::shared_ptr<Ref> ref = nullptr;
};

class MainScreen final : public Component {
  main_screen_props props;
  std::shared_ptr<StackNavigator> navigator;
  StyleStorage* styles;
  References* references;
public:
  ~MainScreen() override {
    delete this->styles;
    delete this->references;
  };

  explicit MainScreen(std::shared_ptr<StackNavigator> stack, const main_screen_props &props) : Component(nullptr, nullptr) {
    this->props = props;
    this->navigator = std::move(stack);
    this->styles = new StyleStorage();
    this->references = new References();
    if(this->props.ref != nullptr) {
        this->props.ref->set(this);
    }

    style_main_screen_register(*this->styles);
  }

[[nodiscard]] std::shared_ptr<View> render_header() const {
    auto navigator_ref = this->navigator;

    return $view(view_props{
        .ref = nullptr,
        .style = $s("header.container"),
        .children = {
            $view(view_props{
                .ref = nullptr,
                .style = $s("header.labels.container"),
                .children = {
                    $label(label_props{ .ref = nullptr, .text = std::format("Channels: {}", 3) }),
                    $label(label_props{ .ref = nullptr, .text = std::format("Inputs: {}", 3) }),
                    $label(label_props{ .ref = nullptr, .text = std::format("Outputs: {}", 3) }),
                },
                header_labels_container_props
            }),
            $view(view_props{
                .ref = nullptr,
                .style = $s("header.container"),
                .children = {
                    $button(button_props{
                        .ref = nullptr,
                        .style = $s("header.button"),
                        .child = $label(label_props{
                            .ref = nullptr,
                            .style = $s("header.label"),
                            .text = locales::en::header_information,
                        }),
                        .on_click = [navigator_ref](lv_event_t* e) {
                            navigator_ref->navigate("/pincode");
                        }
                    }),
                    $button(button_props{
                        .ref = nullptr,
                        .style = $s("header.button"),
                        .child = $label(label_props{
                            .ref = nullptr,
                            .style = $s("header.label"),
                            .text = locales::en::header_settings,
                        }),
                        .on_click = [navigator_ref](lv_event_t* e) {
                            navigator_ref->navigate("/pin_code");
                        }
                    }),
                },
                header_container_right_props
            })
        },
        header_container_props
    });
}


  [[nodiscard]] std::shared_ptr<Button> render_footer() const {
    return $button(button_props{
        .ref = nullptr,
        .style = $s("footer.button"),
        .child = $label(label_props{
          .ref = nullptr,
          .style = $s("header.label"),
          .text = locales::en::status,
        }),
      });
  }

  [[nodiscard]] std::shared_ptr<View> render_body() const {
    return $view(view_props{
    .ref = nullptr,
    .style = $s("header.container"),
    .children = {
      $label(label_props{
          .ref = nullptr,
          .style = $s("header.label"),
          .text = locales::en::oxygen_level,
      }),
      $view(view_props{
        .ref = nullptr,
        .style = $s("header.labels.container"),
        .children = {
          $circular(circular_props{.ref = nullptr, .label_symbol = "%", .show_label_default = true, .min_dy = 0, .max_dy = 100, .default_dy = 20, .w = 100, .h = 100 }),
          $circular(circular_props{.ref = nullptr, .label_symbol = "%", .show_label_default = true, .min_dy = 0, .max_dy = 100, .default_dy = 20, .w = 100, .h = 100 }),
          $circular(circular_props{.ref = nullptr, .label_symbol = "%", .show_label_default = true, .min_dy = 0, .max_dy = 100, .default_dy = 20, .w = 100, .h = 100 }),
        },
        .width = LV_PCT(100),
        .height = 110,
        .justify_content = LV_FLEX_ALIGN_SPACE_AROUND,
        .align_items = LV_FLEX_ALIGN_CENTER,
        .track_cross_place = LV_FLEX_ALIGN_CENTER,
        .flex_direction = LV_FLEX_FLOW_ROW,
      }),
      $label(label_props{
        .ref = nullptr,
        .style = $s("header.label"),
        .text = locales::en::oxygen_rate,
      }),
      $view(view_props{
      .ref = nullptr,
      .style = $s("header.labels.container"),
      .children = {
        $circular(circular_props{.ref = nullptr, .label_symbol = "%", .show_label_default = true, .min_dy = 0, .max_dy = 100, .default_dy = 20, .w = 100, .h = 100 }),
        $circular(circular_props{.ref = nullptr, .label_symbol = "%", .show_label_default = true, .min_dy = 0, .max_dy = 100, .default_dy = 20, .w = 100, .h = 100 }),
        $circular(circular_props{.ref = nullptr, .label_symbol = "%", .show_label_default = true, .min_dy = 0, .max_dy = 100, .default_dy = 20, .w = 100, .h = 100 }),
      },
      .width = LV_PCT(100),
      .height = 110,
      .justify_content = LV_FLEX_ALIGN_SPACE_AROUND,
      .align_items = LV_FLEX_ALIGN_CENTER,
      .track_cross_place = LV_FLEX_ALIGN_CENTER,
      .flex_direction = LV_FLEX_FLOW_ROW,
    }),
    },
      .width = LV_PCT(100),
      .height = LV_PCT(62),
      .justify_content = LV_FLEX_ALIGN_START,
      .align_items = LV_FLEX_ALIGN_CENTER,
      .track_cross_place = LV_FLEX_ALIGN_START,
      .flex_direction = LV_FLEX_FLOW_COLUMN
  });
  }


  lv_obj_t *render() override {
     return this->delegate($view(
      this->parent,
      view_props{
        .ref = nullptr,
        .style = this->styling(),
        .children = {
          $statusbar(status_bar_props{
            .ref = nullptr,
            .style = nullptr,
          }),
          $rn(render_header),
          $rn(render_body),
          $rn(render_footer),
        },
        screen_container_props,
      }));
  }



  std::shared_ptr<Styling> styling() override {
    this->style = std::make_shared<Styling>();

    this->style->setTextColor(lv_color_make(255, 255, 255));
    this->style->setPadding(0, 0, 0, 0);
    this->style->setBorderRadius(0);
    this->style->setBorder(lv_color_make(255, 255, 255), 0, 0);

    return this->style;
  }

  MainScreen* append(lv_obj_t *obj) override
  {
    lv_obj_set_parent(obj, get_component());
    return this;
  }
};