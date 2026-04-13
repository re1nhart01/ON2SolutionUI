// ui_queue_c.cpp

#include "ui_queue_c.h"
#include "ui_queue.h" // твій C++ клас

extern "C" {
  void ui_queue_process(void) {
    foundation::UIQueue::get_instance().process();
  }

  void ui_queue_clear(void) {
    foundation::UIQueue::get_instance().clear();
  }

}