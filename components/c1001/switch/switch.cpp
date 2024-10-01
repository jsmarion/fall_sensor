// #include "esphome/core/log.h"
#include "switch.h"

namespace esphome
{
  namespace c1001
  {
    void C1001FallLEDSwitch::dump_config()
    {
      // LOG_SWITCH("", TAG, this);
      ESP_LOGCONFIG(TAG, "C1001 Fall LED switch");    
      }

    void C1001FallLEDSwitch::write_state(bool state)
    {
      //   this->parent_->write_array(state);
      this->parent_->configLEDLight(eFALLLed, state);
      // this->publish_state(state);
    }

    void C1001FallLEDSwitch::set_state(bool state)
    {
      //   this->parent_->write_array(state);
      // bool state = this->parent_->getLEDLightState(eFALLLed);
      ESP_LOGD(TAG, "Fall LED calling set_state %d", state);
      this->publish_state(state);
    }

    void C1001FallLEDSwitch::setup()
    {
      //   this->parent_->write_array(state);
      this->parent_->setFallLEDCallback([this](bool state)-> void {
        ESP_LOGD(TAG, "Switch: Publishing Fall LED state %d", state);
        this->publish_state(state);
        // return this->set_state(state);
      });
      bool state = this->parent_->getLEDLightState(eFALLLed);
      this->publish_state(state);
    }

/* --------- */

    void C1001WorkModeSwitch::dump_config()
    {
      // LOG_SWITCH("", TAG, this);
      ESP_LOGCONFIG(TAG, "C1001 Work mode switch");    
      }

    void C1001WorkModeSwitch::write_state(bool state)
    {
      //   this->parent_->write_array(state);
      eWorkMode mode = !state ? eFallingMode : eSleepMode;
      this->parent_->configWorkMode(mode);
      this->publish_state(state);
    }

    void C1001WorkModeSwitch::set_state(bool state)
    {
      //   this->parent_->write_array(state);
      // bool state = this->parent_->getLEDLightState(eFALLLed);
      ESP_LOGD(TAG, "Work mode calling set_state %d", state);
      this->publish_state(state);
    }

    void C1001WorkModeSwitch::setup()
    {
      //   this->parent_->write_array(state);
      this->parent_->setWorkModeCallback([this](bool state)-> void {
        ESP_LOGD(TAG, "Switch: Publishing Work Mode state %d", state);
        this->publish_state(state);
        // return this->set_state(state);
      });
      bool state = this->parent_->getWorkMode();
      this->publish_state(state);
    }
  } // namespace c1001
} // namespace esphome
