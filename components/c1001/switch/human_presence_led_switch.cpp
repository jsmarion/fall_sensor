#include "human_presence_led_switch.h"

namespace esphome
{
  namespace c1001
  {

    void HumanPresenceLEDSwitch::write_state(bool state)
    {
      this->publish_state(state);
      // this->parent_->set_human_presence_led_switch(state);
      this->parent_->configLEDLight(eHPLed, state);
    }

  } // namespace c1001
} // namespace esphome