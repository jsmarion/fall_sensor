#include "fall_sensitivity_select.h"

namespace esphome
{
  namespace c1001
  {

    void FallSensitivitySelect::control(const std::string &value)
    {
      // this->publish_state(value);
      // ESP_LOGD(TAG, "Setting fall sensitivity %d", std::stoi(value));
      this->parent_->dmFallConfig(eFallSensitivityC, std::stoi(value));
    }

  } // namespace c1001
} // namespace esphome