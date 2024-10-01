// #include "esphome/core/log.h"
#include "text_sensor.h"

namespace esphome
{
  namespace c1001
  {

    // static const char *TEXT_TAG = "empty_text_sensor.text_sensor";

    void C1001TextSensor::setup()
    {
      this->publish_state("text_sensor initial state");
      this->parent_->setBodyMovementCallback([this](uint8_t state)-> void {
        // ESP_LOGD(TAG, "Text sensor: Publishing Body movement state %d", state);
        return this->publish_state(std::to_string(state));
      });    
    }

    void C1001TextSensor::dump_config()
    {
      ESP_LOGCONFIG(TAG, "C1001 text sensor");
    }

    void C1001TextSensor::loop() {
      // publish_state("text_sensor state");
    }

    // void C1001TextSensor::update()
    // {
    //   // if (this->messagelength > 0)
    //   // { // if there's a message pending
    //   //   ESP_LOGV(TAG, "processing latest message");
    //   //   this->messagelength = 0;                            // indicate message is parsed
    //   //   this->sensorupdateprogress = SOLIS_S5_SENSOR_COUNT; // start to update sensors
    //   // }
    //   // else
    //   // {
    //   //   ESP_LOGV(TAG, "no data received");
    //   // }
    // }

  } // namespace c1001
} // namespace esphome
