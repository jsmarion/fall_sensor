// #include "esphome/core/component.h"
// #include "esphome/components/text_sensor/text_sensor.h"
#include "sys/time.h"

#include "../c1001.h"

namespace esphome
{
  namespace c1001
  {

    class C1001BinarySensor : public binary_sensor::BinarySensor, public Component
    {
    public:
      void setup() override;
      void dump_config() override;
      void set_parent(C1001Component *parent) { this->parent_ = parent; }
      // void update() override;
      void loop() override;

    protected:
      C1001Component *parent_;
      struct timespec last_loop_time;
    };
  } // namespace c1001
} // namespace esphome
