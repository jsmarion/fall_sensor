// #include "esphome/core/log.h"
#include "binary_sensor.h"

namespace esphome
{

  static inline void timespec_diff(struct timespec *a, struct timespec *b, struct timespec *result) {
    result->tv_sec  = a->tv_sec  - b->tv_sec;
    result->tv_nsec = a->tv_nsec - b->tv_nsec;
    if (result->tv_nsec < 0) {
        --result->tv_sec;
        result->tv_nsec += 1000000000L;
    }
}
  namespace c1001
  {
    // static const char *TEX = "empty_binary_sensor.binary_sensor";

    void C1001BinarySensor::setup()
    {
      // uint8_t state = this->parent_->getLEDLightState(eFALLLed);
      // this->publish_initial_state(state);
      this->parent_->setFallLEDStateCallback([this](bool state)-> void {
        ESP_LOGD(TAG, "Sensor: Publishing Fall LED state %d", state);
        this->parent_->getLEDLightState(eFALLLed);
        return this->publish_state(state);
      });
      // this->publish_initial_state(true);
    }

    void C1001BinarySensor::dump_config()
    {
      ESP_LOGCONFIG(TAG, "C1001 binary sensor");
    }

    void C1001BinarySensor::loop() {
      struct timespec now, diff;
      clock_gettime(CLOCK_REALTIME, &now);
      timespec_diff(&now, &last_loop_time, &diff);
      // ESP_LOGD(TAG, "Fall LED time %d", diff.tv_nsec);
      if ((diff.tv_nsec) > 500000000) {
        last_loop_time = now;
        // bool state = this->parent_->getLEDLightState(eFALLLed);
        // ESP_LOGD(TAG, "Fall LED state %d", state);
        // this->publish_state(state);
      }
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
