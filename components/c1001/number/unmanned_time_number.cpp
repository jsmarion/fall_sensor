#include "unmanned_time_number.h"

namespace esphome
{
  namespace c1001
  {

    void UnmannedTimeNumber::control(float value)
    {
      this->publish_state(value);
      this->parent_->dmUnmannedTime((uint32_t)value);
    }

  } // namespace c1001
} // namespace esphome