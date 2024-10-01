// #pragma once

// #include "esphome/core/component.h"
// #include "esphome/components/switch/switch.h"
// #include "esphome/components/uart/uart.h"
#include "../c1001.h"

namespace esphome
{
  namespace c1001
  {

    class C1001FallLEDSwitch : public Component, public switch_::Switch
    {
    public:
      void setup() override;
      void dump_config() override;
      void set_parent(C1001Component *parent) { this->parent_ = parent; }

    protected:
      void write_state(bool) override;
      void set_state(bool);
      C1001Component *parent_;
    };

    class C1001WorkModeSwitch : public Component, public switch_::Switch
    {
    public:
      void setup() override;
      void dump_config() override;
      void set_parent(C1001Component *parent) { this->parent_ = parent; }

    protected:
      void write_state(bool) override;
      void set_state(bool);
      C1001Component *parent_;
    };

  } // namespace c1001
} // namespace esphome
