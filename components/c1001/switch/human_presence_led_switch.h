#pragma once

#include "esphome/components/switch/switch.h"
#include "../c1001.h"

namespace esphome
{
  namespace c1001
  {

    class HumanPresenceLEDSwitch : public switch_::Switch, public Parented<C1001Component>
    {
    public:
      HumanPresenceLEDSwitch() = default;

    protected:
      void write_state(bool state) override;
    };

  } // namespace c1001
} // namespace esphome