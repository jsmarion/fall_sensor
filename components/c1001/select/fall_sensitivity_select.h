#pragma once

#include "esphome/components/select/select.h"
#include "../c1001.h"

namespace esphome
{
  namespace c1001
  {

    class FallSensitivitySelect : public select::Select, public Parented<C1001Component>
    {
    public:
      FallSensitivitySelect() = default;

    protected:
      void control(const std::string &value) override;
    };

  } // namespace c1001
} // namespace esphome