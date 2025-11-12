#pragma once


#include <memory>

#include "component.hh"
#include "text.hh"
#include "view.hh"


#ifndef CIRCULAR_PROGRESS_HH
#define CIRCULAR_PROGRESS_HH

namespace foundation {

  struct circular_props {
    std::shared_ptr<Ref> ref = nullptr;
    std::shared_ptr<Styling> style = std::make_shared<Styling>();

    std::string label_symbol = "%";
    bool show_label_default = false;
    short min_dy = 0;
    short max_dy = 100;
    short default_dy = 0;

    short w = 10;
    short h = 10;
  };

  class CircularProgress final : public Component {
  private:
    circular_props props;
    std::shared_ptr<Text> label = nullptr;
    lv_obj_t* arc_reference = nullptr;
    bool is_show_label = false;

  public:
    explicit CircularProgress(circular_props props);
    ~CircularProgress() override = default;

    lv_obj_t* render() override;
    std::shared_ptr<Styling> styling() override;
    CircularProgress* append(lv_obj_t* obj) override;

    void show_label(bool value) const;
    void update(short value) const;
  };

}
#endif //CIRCULAR_PROGRESS_HH
