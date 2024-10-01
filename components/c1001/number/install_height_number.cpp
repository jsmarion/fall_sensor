#include "install_height_number.h"

namespace esphome
{
  namespace c1001
  {

    void InstallHeightNumber::control(float value)
    {
      this->publish_state(value);
      this->parent_->dmInstallHeight((uint32_t)value);
    }

  } // namespace c1001
} // namespace esphome