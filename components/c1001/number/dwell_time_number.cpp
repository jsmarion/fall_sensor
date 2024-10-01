#include "dwell_time_number.h"

namespace esphome
{
  namespace c1001
  {

    void DwellTimeNumber::control(float value)
    {
      this->publish_state(value);
      this->parent_->dmFallConfig(eResidenceTime, (uint32_t)value);
    }

  } // namespace c1001
} // namespace esphome