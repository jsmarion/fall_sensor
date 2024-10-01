#include "fall_time_limit_number.h"

namespace esphome
{
  namespace c1001
  {

    void FallTimeNumber::control(float value)
    {
      this->publish_state(value);
      this->parent_->dmFallTime((uint32_t)value);
    }

  } // namespace c1001
} // namespace esphome