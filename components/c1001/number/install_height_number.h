#pragma once

#include "esphome/components/number/number.h"
#include "../c1001.h"

namespace esphome
{
  namespace c1001
  {

    class InstallHeightNumber : public number::Number, public Parented<C1001Component>
    {
    public:
      InstallHeightNumber() = default;

    protected:
      void control(float value) override;
    };

  } // namespace C1001
} // namespace esphome