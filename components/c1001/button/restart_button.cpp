#include "restart_button.h"

namespace esphome
{
    namespace c1001
    {

        void RestartButton::press_action() { this->parent_->sensorRestart(); }

    } // namespace c1001
} // namespace esphome